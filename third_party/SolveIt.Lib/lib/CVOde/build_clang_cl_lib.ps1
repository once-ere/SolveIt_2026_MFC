param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64",
    [string]$ProjectDir
)

$ErrorActionPreference = "Stop"

if ($Platform -ne "x64") {
    Write-Host "Skipping CVOde build for platform $Platform"
    exit 0
}

$root = if ($ProjectDir) {
    (Resolve-Path ($ProjectDir.Trim('"'))).Path
} else {
    (Resolve-Path (Join-Path $PSScriptRoot "..\..\..\..")).Path
}

# Locate a Visual Studio installation that actually has the x64 C++ toolset
# (do NOT use clang-cl: build the legacy CVODE C sources with MSVC cl.exe).
$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    throw "vswhere.exe not found at $vswhere"
}

$installPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath | Select-Object -First 1
if (-not $installPath) {
    throw "A Visual Studio installation with the x64 C++ toolset was not found"
}

$msvcRoot = Join-Path $installPath "VC\Tools\MSVC"
$toolsetDir = Get-ChildItem $msvcRoot -Directory | Sort-Object Name -Descending | Select-Object -First 1
if (-not $toolsetDir) {
    throw "No MSVC toolset found under $msvcRoot"
}

$clExe = Join-Path $toolsetDir.FullName "bin\Hostx64\x64\cl.exe"
$libExe = Join-Path $toolsetDir.FullName "bin\Hostx64\x64\lib.exe"
foreach ($tool in @($clExe, $libExe)) {
    if (-not (Test-Path $tool)) {
        throw "Required tool was not found at $tool"
    }
}

# Initialize the MSVC x64 build environment (INCLUDE/LIB for CRT + Windows SDK)
# by importing the variables produced by vcvars64.bat.
$vcvars = Join-Path $installPath "VC\Auxiliary\Build\vcvars64.bat"
if (-not (Test-Path $vcvars)) {
    throw "vcvars64.bat not found at $vcvars"
}

if (-not $env:__CVODE_VCVARS_INITIALIZED) {
    cmd /c "`"$vcvars`" >nul 2>&1 && set" | ForEach-Object {
        if ($_ -match "^([^=]+)=(.*)$") {
            Set-Item -Path ("Env:" + $matches[1]) -Value $matches[2]
        }
    }
    $env:__CVODE_VCVARS_INITIALIZED = "1"
}

$sourceDir = Join-Path $PSScriptRoot "source"
$includeDir = Join-Path $PSScriptRoot "include"
$intermediateDir = Join-Path $root "x64\$Configuration\cvode_legacy"
$libPath = Join-Path $root "x64\$Configuration\cvode_legacy.lib"

New-Item -ItemType Directory -Force -Path $intermediateDir | Out-Null

$sources = @(
    "band.c",
    "cvband.c",
    "cvbandpre.c",
    "cvdense.c",
    "cvdiag.c",
    "cvode.c",
    "cvspgmr.c",
    "dense.c",
    "iterativ.c",
    "llnlmath.c",
    "nvector.c",
    "spgmr.c"
)

# Compile the legacy CVODE sources as C with MSVC. /W0 silences the many
# legacy K&R/C89 style warnings; _CRT_SECURE_NO_WARNINGS quiets CRT deprecations.
$compileFlags = @(
    "/nologo",
    "/c",
    "/TC",
    "/W0",
    "/D_CRT_SECURE_NO_WARNINGS",
    "/I$includeDir"
)

if ($Configuration -eq "Debug") {
    $compileFlags += @("/MTd", "/Od", "/Z7", "/D_DEBUG")
} else {
    $compileFlags += @("/MT", "/O2", "/DNDEBUG")
}

$objectFiles = @()
foreach ($sourceName in $sources) {
    $sourcePath = Join-Path $sourceDir $sourceName
    if (-not (Test-Path $sourcePath)) {
        throw "CVODE source was not found at $sourcePath"
    }
    $objectPath = Join-Path $intermediateDir (($sourceName -replace '\.c$', '.obj'))

    & $clExe @compileFlags "/Fo$objectPath" $sourcePath
    if ($LASTEXITCODE -ne 0) {
        throw "cl.exe failed while compiling $sourceName"
    }

    $objectFiles += $objectPath
}

if (Test-Path $libPath) {
    Remove-Item $libPath -Force
}

& $libExe /NOLOGO /MACHINE:X64 "/OUT:$libPath" @objectFiles
if ($LASTEXITCODE -ne 0) {
    throw "lib.exe failed while producing $libPath"
}

Write-Host "Built $libPath"

Write-Host "Built $libPath"