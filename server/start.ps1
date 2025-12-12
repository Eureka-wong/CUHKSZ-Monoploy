# Start Monopoly Online Server

$scriptDir = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
if (-not $scriptDir) { $scriptDir = Get-Location }

Push-Location $scriptDir

Write-Host "=== Starting Monopoly Online Server ===" -ForegroundColor Cyan
Write-Host "Server will listen on port 9002" -ForegroundColor Yellow
Write-Host "Press Ctrl+C to stop the server`n" -ForegroundColor Yellow

# Add vcpkg DLL path to PATH for this session
$vcpkgBin = "$scriptDir\vcpkg\installed\x64-windows\bin"
if (Test-Path $vcpkgBin) {
    $env:PATH = "$vcpkgBin;$env:PATH"
    Write-Host "Added vcpkg bin to PATH: $vcpkgBin" -ForegroundColor Green
}

# Run the server
$possiblePaths = @(
    ".\build\Release\monopoly_online_server.exe",
    ".\build\Debug\monopoly_online_server.exe",
    ".\build\monopoly_online_server.exe",
    ".\build\x64\Release\monopoly_online_server.exe",
    ".\build\x64\Debug\monopoly_online_server.exe"
)

$exePath = $null
foreach ($path in $possiblePaths) {
    if (Test-Path $path) {
        $exePath = $path
        Write-Host "Found server executable at: $path" -ForegroundColor Green
        break
    }
}

if (-not $exePath) {
    Write-Host "Error: Server executable not found!" -ForegroundColor Red
    Write-Host "`nPlease build the server first using:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1" -ForegroundColor Cyan
    Write-Host "`nSearched in the following paths:" -ForegroundColor Yellow
    foreach ($path in $possiblePaths) {
        Write-Host "  $path" -ForegroundColor Gray
    }
    Pop-Location
    exit 1
}

Write-Host "Starting server: $exePath" -ForegroundColor Green
& $exePath

Pop-Location

