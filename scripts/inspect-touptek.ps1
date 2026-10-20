[CmdletBinding()]
param(
    [string]$MicroManagerPath = 'C:\Program Files\Micro-Manager-2.0'
)

$ErrorActionPreference = 'Stop'

Write-Host 'Primary researcher: Malavika S'
Write-Host ''
Write-Host 'Camera-like USB devices'

$devices = Get-PnpDevice -PresentOnly -ErrorAction SilentlyContinue |
    Where-Object {
        $_.InstanceId -like 'USB\VID_0547*' -or
        $_.FriendlyName -match 'Toup|ProCAM|USB3\.0 Camera'
    } |
    Select-Object Status, Class, FriendlyName, InstanceId

if ($devices) {
    $devices | Format-Table -AutoSize
} else {
    Write-Warning 'No matching ToupTek/ProCAM-style USB device was found.'
}

Write-Host ''
Write-Host 'Micro-Manager installation'

if (-not (Test-Path -LiteralPath $MicroManagerPath -PathType Container)) {
    Write-Warning "Micro-Manager directory not found: $MicroManagerPath"
    exit 0
}

Write-Host "Directory: $MicroManagerPath"

$adapterPaths = @(
    (Join-Path $MicroManagerPath 'mmgr_dal_Toupcam.dll'),
    (Join-Path $MicroManagerPath 'toupcam.dll')
)

foreach ($adapterPath in $adapterPaths) {
    if (Test-Path -LiteralPath $adapterPath -PathType Leaf) {
        $file = Get-Item -LiteralPath $adapterPath
        Write-Host ("Found: {0} ({1:N0} bytes)" -f $file.Name, $file.Length)
    } else {
        Write-Warning "Missing: $adapterPath"
    }
}

$logDirectory = Join-Path $MicroManagerPath 'CoreLogs'
if (Test-Path -LiteralPath $logDirectory -PathType Container) {
    $latestLog = Get-ChildItem -LiteralPath $logDirectory -File -Filter 'CoreLog*.txt' |
        Sort-Object LastWriteTime -Descending |
        Select-Object -First 1

    if ($latestLog) {
        Write-Host ''
        Write-Host "Latest CoreLog: $($latestLog.FullName)"
        Select-String -LiteralPath $latestLog.FullName -Pattern @(
            'MM Studio version',
            'MMCore version',
            'Device API version'
        ) |
            Select-Object -First 3 |
            ForEach-Object { Write-Host $_.Line }
    }
}

Write-Host ''
Write-Host 'This script is diagnostic only; it does not install or modify drivers.'
