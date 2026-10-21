# SPDX-License-Identifier: Apache-2.0
# A dependency-aware, clean-room delivery harness for this native adapter.
[CmdletBinding()]
param(
    [ValidateSet('status', 'next', 'verify')]
    [string]$Command = 'status',
    [ValidateSet('dev', 'qa', 'maintainer')]
    [string]$Role = 'dev',
    [switch]$RequireNative,
    [string]$BuildDirectory = 'factory-build'
)

$ErrorActionPreference = 'Stop'
$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$storiesPath = Join-Path $repositoryRoot 'factory/stories.json'
$incidentsPath = Join-Path $repositoryRoot 'factory/incidents.jsonl'
$validStatuses = @('todo', 'claimed', 'in_review', 'done', 'blocked')
$validRoles = @('dev', 'qa', 'maintainer')

function Get-Factory {
    return Get-Content -Raw $storiesPath | ConvertFrom-Json
}

function Test-FactoryState($factory) {
    $errors = [System.Collections.Generic.List[string]]::new()
    if ($factory.schemaVersion -ne 1 -or $null -eq $factory.stories) {
        $errors.Add('stories.json must contain schemaVersion 1 and a stories array.')
        return $errors
    }
    $ids = @{}
    foreach ($story in $factory.stories) {
        if ([string]::IsNullOrWhiteSpace($story.id) -or $story.id -notmatch '^CAM-\d{3}$' -or $ids.ContainsKey($story.id)) {
            $errors.Add("Invalid or duplicate story id: $($story.id)")
        }
        $ids[$story.id] = $true
        if ($story.status -notin $validStatuses) { $errors.Add("$($story.id): invalid status") }
        if ($null -eq $story.roles -or $null -eq $story.dependsOn -or $null -eq $story.acceptanceEvidence) {
            $errors.Add("$($story.id): roles, dependsOn, and acceptanceEvidence are required arrays")
        } elseif (@($story.roles | Where-Object { $_ -notin $validRoles }).Count -gt 0) {
            $errors.Add("$($story.id): invalid role")
        }
        if ($story.status -eq 'done' -and ($null -eq $story.qa -or [string]::IsNullOrWhiteSpace($story.qa.reviewer) -or [string]::IsNullOrWhiteSpace($story.qa.verifiedAt) -or @($story.acceptanceEvidence).Count -eq 0)) {
            $errors.Add("$($story.id): done stories require QA reviewer, date, and evidence")
        }
    }
    foreach ($story in $factory.stories) {
        foreach ($dependency in @($story.dependsOn)) {
            if (-not $ids.ContainsKey($dependency)) { $errors.Add("$($story.id): unknown dependency $dependency") }
        }
    }
    return $errors
}

function Test-Incidents {
    $errors = [System.Collections.Generic.List[string]]::new()
    $lineNumber = 0
    foreach ($line in Get-Content $incidentsPath) {
        $lineNumber++
        if ([string]::IsNullOrWhiteSpace($line) -or $line.TrimStart().StartsWith('#')) { continue }
        try { $incident = $line | ConvertFrom-Json } catch { $errors.Add("incident line ${lineNumber}: invalid JSON"); continue }
        if ($incident.status -ne 'closed') { $errors.Add("incident line ${lineNumber}: $($incident.id) is not closed") }
        if ([string]::IsNullOrWhiteSpace($incident.id) -or [string]::IsNullOrWhiteSpace($incident.summary) -or [string]::IsNullOrWhiteSpace($incident.preventionCheck) -or [string]::IsNullOrWhiteSpace($incident.closedAt)) {
            $errors.Add("incident line ${lineNumber}: id, summary, preventionCheck, and closedAt are required")
        }
    }
    return $errors
}

function Invoke-Checked([string]$Description, [scriptblock]$Action) {
    Write-Host "==> $Description"
    & $Action
    if ($LASTEXITCODE -ne 0) { throw "$Description failed with exit code $LASTEXITCODE." }
}

$factory = Get-Factory
$errors = @(Test-FactoryState $factory)
if ($Command -eq 'status') {
    if ($errors.Count) { throw ($errors -join [Environment]::NewLine) }
    $factory.stories | Group-Object stage | Sort-Object { [int]$_.Name } | ForEach-Object {
        "$($_.Name): " + (($_.Group | ForEach-Object { "$($_.id)=$($_.status)" }) -join ', ')
    }
    exit 0
}

if ($Command -eq 'next') {
    if ($errors.Count) { throw ($errors -join [Environment]::NewLine) }
    $ready = @($factory.stories | Where-Object {
        $_.status -eq 'todo' -and $_.roles -contains $Role -and @($_.dependsOn | Where-Object {
            ($factory.stories | Where-Object id -eq $_).status -ne 'done'
        }).Count -eq 0
    })
    if ($ready.Count -eq 0) { Write-Host "No ready stories for $Role." } else { $ready | ForEach-Object { Write-Host "$($_.id) [stage $($_.stage)] $($_.title)" } }
    exit 0
}

$errors += @(Test-Incidents)
if ($errors.Count) { throw "Factory validation failed:`n$($errors | ForEach-Object { "- $_" } | Out-String)" }
Invoke-Checked 'Whitespace check' { git -C $repositoryRoot diff --check }
Invoke-Checked 'Clean-room source audit' { & (Join-Path $repositoryRoot 'scripts/audit-clean-room-source.ps1') }

$nativeInputs = @($env:MMCOREANDDEVICES_ROOT, $env:LOUPECAM_ROOT, $env:LOUPECAM_LIBRARY_DIR)
if ($nativeInputs -contains $null -or $nativeInputs -contains '') {
    $message = 'Native build skipped: set MMCOREANDDEVICES_ROOT, LOUPECAM_ROOT, and LOUPECAM_LIBRARY_DIR to run it.'
    if ($RequireNative) { throw $message }
    Write-Warning $message
    Write-Host 'Source-only factory verification passed; native build evidence was not produced.'
    exit 0
}

$buildPath = if ([System.IO.Path]::IsPathRooted($BuildDirectory)) { $BuildDirectory } else { Join-Path $repositoryRoot $BuildDirectory }
Invoke-Checked 'Configure native adapter' {
    cmake -S $repositoryRoot -B $buildPath -A x64 "-DMMCOREANDDEVICES_ROOT=$env:MMCOREANDDEVICES_ROOT" "-DLOUPECAM_ROOT=$env:LOUPECAM_ROOT" "-DLOUPECAM_LIBRARY_DIR=$env:LOUPECAM_LIBRARY_DIR"
}
Invoke-Checked 'Build native adapter' { cmake --build $buildPath --config Release --parallel }
Invoke-Checked 'Run simulated adapter smoke test' { ctest --test-dir $buildPath -C Release --output-on-failure }
Write-Host 'Native factory verification passed.'
