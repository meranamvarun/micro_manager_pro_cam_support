# SPDX-License-Identifier: Apache-2.0
# Primary researcher: Malavika S

[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$trackedFiles = git -C $repositoryRoot ls-files
if ($LASTEXITCODE -ne 0) {
    throw 'Unable to enumerate tracked files.'
}

$textExtensions = @(
    '.cpp', '.h', '.md', '.ps1', '.txt', '.yml', '.yaml', '.json', '.cmake'
)
$findings = [System.Collections.Generic.List[string]]::new()

foreach ($relativePath in $trackedFiles) {
    $extension = [System.IO.Path]::GetExtension($relativePath).ToLowerInvariant()
    if ($extension -notin $textExtensions -and
        [System.IO.Path]::GetFileName($relativePath) -ne 'CMakeLists.txt') {
        continue
    }

    $absolutePath = Join-Path $repositoryRoot $relativePath
    $lineNumber = 0
    foreach ($line in [System.IO.File]::ReadLines($absolutePath)) {
        $lineNumber++
        if ($line -match '[\u3400-\u4DBF\u4E00-\u9FFF]') {
            $findings.Add("${relativePath}:${lineNumber}")
        }
    }
}

if ($findings.Count -gt 0) {
    $findings | ForEach-Object { Write-Error "CJK ideograph found at $_" }
    throw 'The tracked source-text audit failed.'
}

Write-Host "Tracked source-text audit passed ($($trackedFiles.Count) files enumerated)."
Write-Host 'This is a text-content check only; it does not certify geographic provenance or policy compliance.'
