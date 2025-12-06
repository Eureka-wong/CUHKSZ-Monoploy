# PowerShell script to package the client for distribution
# This creates a portable package that can run on any Windows computer without Qt/VS installed

param(
    [switch]$NoBuild
)

$ErrorActionPreference = "Stop"

$scriptDir = $PSScriptRoot
if (-not $scriptDir) {
    $scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
}
if (-not $scriptDir) {
    $scriptDir = Get-Location
}
Set-Location $scriptDir

Write-Host "=== Packaging CUHKSZ Monopoly Online Client ===" -ForegroundColor Cyan

if (-not $NoBuild) {
    Write-Host "Building project first..." -ForegroundColor Cyan
    & "$scriptDir\run.ps1"
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }
}

$qt6Path = $null
$possibleQtPaths = @(
    "C:\Qt\6.10.1\msvc2022_64"
)

foreach ($path in $possibleQtPaths) {
    if (Test-Path $path) {
        $qt6Path = $path
        Write-Host "Found Qt6 at: $qt6Path" -ForegroundColor Green
        break
    }
}


$buildDir = Join-Path $scriptDir "build\Windows-Release"
$releaseDir = Join-Path $buildDir "Release"
$exePath = Join-Path $releaseDir "CUHKSZ_Monopoly_Online.exe"

if (-not (Test-Path $exePath)) {
    throw "Executable not found: $exePath. Please build the project first."
}

$packageDir = Join-Path $scriptDir "Client_Portable"
if (Test-Path $packageDir) {
    Write-Host "Removing old package directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $packageDir
}
New-Item -ItemType Directory -Path $packageDir | Out-Null

Write-Host "Copying files to package directory..." -ForegroundColor Cyan

Copy-Item $exePath $packageDir -Force
Write-Host "  - Executable copied" -ForegroundColor Green

if (Test-Path $releaseDir) {
    Get-ChildItem -Path $releaseDir -File | ForEach-Object {
        Copy-Item $_.FullName $packageDir -Force
    }
    Write-Host "  - DLLs copied" -ForegroundColor Green
    
    Get-ChildItem -Path $releaseDir -Directory | ForEach-Object {
        $destDir = Join-Path $packageDir $_.Name
        Copy-Item $_.FullName $destDir -Recurse -Force
        Write-Host "  - Directory $($_.Name) copied" -ForegroundColor Green
    }
}

if ($qt6Path) {
    $windeployqt = Join-Path $qt6Path "bin\windeployqt.exe"
    if (Test-Path $windeployqt) {
        Write-Host "Running windeployqt to ensure all dependencies..." -ForegroundColor Cyan
        Push-Location $packageDir
        & $windeployqt --release --compiler-runtime CUHKSZ_Monopoly_Online.exe
        Pop-Location
        Write-Host "  - Qt dependencies verified" -ForegroundColor Green
    } else {
        Write-Warning "windeployqt not found, skipping dependency check"
    }
}

$imagesDir = Join-Path $scriptDir "images"
if (Test-Path $imagesDir) {
    $destImagesDir = Join-Path $packageDir "images"
    Copy-Item $imagesDir $destImagesDir -Recurse -Force
    Write-Host "  - Images copied" -ForegroundColor Green
}


$readmeContent = @"
CUHKSZ Monopoly Online - Portable Client
=========================================

This is a portable version of the Monopoly Online client.
You can run it on any Windows computer without installing Qt or Visual Studio.

HOW TO USE:
-----------
1. Double-click CUHKSZ_Monopoly_Online.exe to start the client
2. Enter the server URL (e.g., ws://10.30.198.46:9002)
3. Enter your player name
4. Connect to the server and join a room

REQUIREMENTS:
-------------
- Windows 10 or later
- All required DLLs are included in this package

NETWORK:
--------
- Make sure you're on the same network as the server
- The server IP address should be provided by the server administrator
- Default port is 9002

TROUBLESHOOTING:
---------------
- If the program doesn't start, make sure all files in this folder are present
- Check Windows Firewall settings if you can't connect to the server
- Make sure the server is running before connecting

For support, contact the game administrator.
"@

$readmePath = Join-Path $packageDir "README.txt"
$readmeContent | Out-File -FilePath $readmePath -Encoding UTF8
Write-Host "  - README.txt created" -ForegroundColor Green

# Create a simple batch file to run the client
$batchContent = @"
@echo off
echo Starting CUHKSZ Monopoly Online Client...
start "" "CUHKSZ_Monopoly_Online.exe"
"@

$batchPath = Join-Path $packageDir "Run_Client.bat"
$batchContent | Out-File -FilePath $batchPath -Encoding ASCII
Write-Host "  - Run_Client.bat created" -ForegroundColor Green

# Summary
Write-Host ""
Write-Host "=== Package Created Successfully ===" -ForegroundColor Green
Write-Host "Package location: $packageDir" -ForegroundColor Cyan
Write-Host ""
Write-Host "To distribute:" -ForegroundColor Yellow
Write-Host "  1. Copy the entire 'Client_Portable' folder to the target computer" -ForegroundColor White
Write-Host "  2. Double-click CUHKSZ_Monopoly_Online.exe to run" -ForegroundColor White
Write-Host ""
Write-Host "Package size:" -ForegroundColor Yellow
$packageSize = (Get-ChildItem -Path $packageDir -Recurse | Measure-Object -Property Length -Sum).Sum / 1MB
Write-Host "  $([math]::Round($packageSize, 2)) MB" -ForegroundColor White
Write-Host ""

