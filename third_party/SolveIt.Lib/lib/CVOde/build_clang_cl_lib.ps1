param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64",
    [string]$ProjectDir
)

$ErrorActionPreference = "Stop"

if ($Platform -ne "x64") {
    Write-Host "Skipping CVOde clang build for platform $Platform"
    exit 0
}

$root = if ($ProjectDir) {
    (Resolve-Path ($ProjectDir.Trim('"'))).Path
} else {
    (Resolve-Path (Join-Path $PSScriptRoot "..\..\..\..")).Path
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    throw "vswhere.exe not found at $vswhere"
}

$installPath = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath | Select-Object -First 1
if (-not $installPath) {
    throw "Visual Studio installation path not found"
}

$clangCl = Join-Path $installPath "VC\Tools\Llvm\x64\bin\clang-cl.exe"
if (-not (Test-Path $clangCl)) {
    throw "clang-cl.exe not found at $clangCl"
}

$msvcRoot = Join-Path $installPath "VC\Tools\MSVC"
$toolsetDir = Get-ChildItem $msvcRoot -Directory | Sort-Object Name -Descending | Select-Object -First 1
if (-not $toolsetDir) {
    throw "No MSVC toolset found under $msvcRoot"
}

$libExe = Join-Path $toolsetDir.FullName "bin\Hostx64\x64\lib.exe"
if (-not (Test-Path $libExe)) {
    throw "lib.exe not found at $libExe"
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

$compileFlags = @(
    "/nologo",
    "/c",
    "/TC",
    "/clang:-std=gnu89",
    "/clang:-Wno-format",
    "/clang:-Wno-return-type",
    "/clang:-Wno-dangling-else",
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
    $objectPath = Join-Path $intermediateDir (($sourceName -replace '\.c$', '.obj'))

    & $clangCl @compileFlags "/Fo$objectPath" $sourcePath
    if ($LASTEXITCODE -ne 0) {
        throw "clang-cl failed while compiling $sourceName"
    }

    $objectFiles += $objectPath
}

if (Test-Path $libPath) {
    Remove-Item $libPath -Force
}

& $libExe /nologo "/out:$libPath" @objectFiles
if ($LASTEXITCODE -ne 0) {
    throw "lib.exe failed while producing $libPath"
}

Write-Host "Built $libPath"