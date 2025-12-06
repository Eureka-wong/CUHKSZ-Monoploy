# Build Monopoly Online Server

$scriptDir = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
if (-not $scriptDir) { $scriptDir = Get-Location }

Push-Location $scriptDir

Write-Host "Building Monopoly Online Server..." -ForegroundColor Cyan

# Setup MSVC environment
$vcvars = "D:\Program Files\Microsoft\VisualStudio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
if (-not (Test-Path $vcvars)) {
    $vcvars = "D:\Program Files\Microsoft\VisualStudio\17\Community\VC\Auxiliary\Build\vcvarsall.bat"
}
if (-not (Test-Path $vcvars)) {
    Write-Host "Error: vcvarsall.bat not found. Please install Visual Studio." -ForegroundColor Red
    Pop-Location
    exit 1
}

if (Test-Path $vcvars) {
    cmd /c "`"$vcvars`" x64 && set" | ForEach-Object {
        if ($_ -match "^([^=]+)=(.*)$") {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2], "Process")
        }
    }
    Write-Host "MSVC environment configured" -ForegroundColor Green
} else {
    Write-Host "Error: vcvarsall.bat not found" -ForegroundColor Red
    Pop-Location
    exit 1
}

Write-Host "`nCleaning old build..." -ForegroundColor Cyan
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
Remove-Item CMakeCache.txt -ErrorAction SilentlyContinue

Write-Host "Configuring CMake with MSVC..." -ForegroundColor Cyan

# 尝试使用 Visual Studio 生成器
$generator = $null
$possibleGenerators = @(
    "Visual Studio 18 2026",
    "Visual Studio 17 2022",
    "Visual Studio 16 2019"
)

# 检查可用的生成器
$availableGenerators = cmake --help | Select-String "Visual Studio" | ForEach-Object { $_.Line.Trim() }
Write-Host "Available Visual Studio generators:" -ForegroundColor Yellow
$availableGenerators | ForEach-Object { Write-Host "  $_" -ForegroundColor Gray }

foreach ($gen in $possibleGenerators) {
    if ($availableGenerators -match $gen) {
        $generator = $gen
        Write-Host "Using generator: $generator" -ForegroundColor Green
        break
    }
}

if (-not $generator) {
    Write-Host "Warning: No Visual Studio generator found, trying default..." -ForegroundColor Yellow
    $generator = "Visual Studio 18 2026"
}

cmake -S . -B build -G "$generator" -A x64

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed" -ForegroundColor Red
    Pop-Location
    exit 1
}

Write-Host "`nBuilding project..." -ForegroundColor Cyan
cmake --build build --config Release

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nBuild successful!" -ForegroundColor Green
    $possiblePaths = @(
        ".\build\Release\monopoly_online_server.exe",
        ".\build\Debug\monopoly_online_server.exe",
        ".\build\monopoly_online_server.exe"
    )
    $exePath = $null
    foreach ($path in $possiblePaths) {
        if (Test-Path $path) {
            $exePath = $path
            break
        }
    }
    if ($exePath) {
        Write-Host "Server executable: $exePath" -ForegroundColor Yellow
    } else {
        Write-Host "Warning: Executable not found at expected paths" -ForegroundColor Yellow
        Write-Host "Searched in:" -ForegroundColor Yellow
        $possiblePaths | ForEach-Object { Write-Host "  $_" -ForegroundColor Gray }
    }
} else {
    Write-Host "`nBuild failed" -ForegroundColor Red
    Pop-Location
    exit 1
}

Pop-Location

