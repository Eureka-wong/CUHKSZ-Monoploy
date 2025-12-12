# PowerShell script to build and run CUHKSZ-Monoploy-online

param(
    [switch]$Rebuild
)

$ErrorActionPreference = "Stop"

# Get script directory
$scriptDir = $PSScriptRoot
if (-not $scriptDir) {
    $scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
}
if (-not $scriptDir) {
    $scriptDir = Get-Location
}
Set-Location $scriptDir

Write-Host "=== Building CUHKSZ-Monoploy-online ===" -ForegroundColor Cyan

# Find Qt6
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

if (-not $qt6Path) {
    Write-Warning "Qt6 not found in standard locations. Please set CMAKE_PREFIX_PATH manually."
}

# Set build directory
$buildDir = Join-Path $scriptDir "build\Windows-Release"
if ($Rebuild -and (Test-Path $buildDir)) {
    Write-Host "Removing old build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir
}

if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Configure CMake
Write-Host "Configuring CMake..." -ForegroundColor Cyan
$cmakeArgs = @(
    "-S", $scriptDir,
    "-B", $buildDir
)
if ($qt6Path) {
    $cmakeArgs += "-DCMAKE_PREFIX_PATH=$qt6Path"
}

# Find Visual Studio
$vsPath = $null
$possibleVsPaths = @(
    "D:\Program Files\Microsoft\VisualStudio\18\Community\VC\Auxiliary\Build\vcvars64.bat",
    "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat",
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
)

Write-Host "Checking for Visual Studio..." -ForegroundColor Cyan
foreach ($path in $possibleVsPaths) {
    if (Test-Path $path) {
        $vsPath = $path
        Write-Host "Found Visual Studio at: $path" -ForegroundColor Green
        break
    } else {
        Write-Host "Not found: $path" -ForegroundColor Gray
    }
}

# Check if VS 18 2026 generator is available
Write-Host "Checking for Visual Studio 18 2026 generator..." -ForegroundColor Cyan
$hasVS2026 = $false
try {
    $generators = cmake --help | Select-String "Visual Studio"
    if ($generators) {
        Write-Host "Available Visual Studio generators:" -ForegroundColor Yellow
        $generators | ForEach-Object { Write-Host "  $_" -ForegroundColor Gray }
        if ($generators -match "Visual Studio 18 2026") {
            $hasVS2026 = $true
            Write-Host "Visual Studio 18 2026 generator is AVAILABLE" -ForegroundColor Green
        } else {
            Write-Host "Visual Studio 18 2026 generator is NOT available" -ForegroundColor Yellow
        }
    } else {
        Write-Host "No Visual Studio generators found" -ForegroundColor Yellow
    }
} catch {
    Write-Host "Error checking generators: $_" -ForegroundColor Red
}

# Build with Visual Studio if found
if (Test-Path $vsPath) {
    if ($hasVS2026) {
        $generator = "Visual Studio 18 2026"
    } else {
        $generator = "Visual Studio 17 2022"
    }
    $platform = "x64"

    $cmakeArgsWithGenerator = @(
        "-S", $scriptDir,
        "-B", $buildDir,
        "-G", $generator,
        "-A", $platform
    )
    if ($qt6Path) {
        $cmakeArgsWithGenerator += "-DCMAKE_PREFIX_PATH=$qt6Path"
    }

    Write-Host "CMake command: cmake $($cmakeArgsWithGenerator -join ' ')" -ForegroundColor Gray

    $cacheFile = Join-Path $buildDir "CMakeCache.txt"
    if (Test-Path $cacheFile) {
        Write-Host "Removing CMake cache..." -ForegroundColor Yellow
        Remove-Item -Force $cacheFile -ErrorAction SilentlyContinue
    }

    & cmake $cmakeArgsWithGenerator
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }

    Write-Host "Building project..." -ForegroundColor Cyan
    & cmake --build $buildDir --config Release
    if ($LASTEXITCODE -ne 0) { throw "Build failed" }

    $exePath = Join-Path $buildDir "Release\CUHKSZ_Monopoly_Online.exe"
} else {
    Write-Host "Visual Studio not found, trying default generator..." -ForegroundColor Yellow
    & cmake $cmakeArgs
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }

    & cmake --build $buildDir --config Release
    if ($LASTEXITCODE -ne 0) { throw "Build failed" }

    $exePath = Join-Path $buildDir "CUHKSZ_Monopoly_Online.exe"
}

# Check executable
if (-not (Test-Path $exePath)) {
    Write-Warning "Executable not found at expected location: $exePath"
    $foundExe = Get-ChildItem -Path $buildDir -Filter "CUHKSZ_Monopoly_Online.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($foundExe) {
        $exePath = $foundExe.FullName
        Write-Host "Found executable at: $exePath" -ForegroundColor Green
    } else {
        throw "Cannot find CUHKSZ_Monopoly_Online.exe. Build may have failed."
    }
}

# Deploy Qt runtime
if ($qt6Path) {
    $windeployqt = Join-Path $qt6Path "bin\windeployqt.exe"
    if (Test-Path $windeployqt) {
        Write-Host "Deploying Qt6 runtime libraries..." -ForegroundColor Cyan
        $exeDir = Split-Path $exePath -Parent
        Push-Location $exeDir
        & $windeployqt --release CUHKSZ_Monopoly_Online.exe
        Pop-Location
        Write-Host "Qt6 runtime deployed." -ForegroundColor Green
    }
}

# Run executable
Write-Host "=== Launching Client ===" -ForegroundColor Cyan
Write-Host "Executable: $exePath" -ForegroundColor Green

$exeDir = Split-Path $exePath -Parent
Push-Location $exeDir
& $exePath
Pop-Location

Write-Host ""
Write-Host "Done!" -ForegroundColor Green
