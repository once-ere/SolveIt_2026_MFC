param(
    [string]$ProjectDir
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$sourcePath = Join-Path $scriptDir 'dformt_x64_stub.c'
$outputLib = Join-Path $scriptDir 'dformt_x64.lib'
$objectPath = Join-Path $scriptDir 'dformt_x64_stub.obj'

if (-not (Test-Path $sourcePath)) {
    throw "Stub source was not found at $sourcePath"
}

if ((Test-Path $outputLib) -and ((Get-Item $outputLib).LastWriteTimeUtc -ge (Get-Item $sourcePath).LastWriteTimeUtc)) {
    Write-Host "dformt_x64.lib is up to date"
    return
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path $vswhere)) {
    throw "vswhere.exe was not found at $vswhere"
}

$installPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if ([string]::IsNullOrWhiteSpace($installPath)) {
    throw 'A Visual Studio installation with C++ tools was not found.'
}

$msvcRoot = Join-Path $installPath 'VC\Tools\MSVC'
$toolsetDir = Get-ChildItem $msvcRoot -Directory | Sort-Object Name -Descending | Select-Object -First 1
if ($null -eq $toolsetDir) {
    throw "No MSVC toolset directory was found under $msvcRoot"
}

$clExe = Join-Path $toolsetDir.FullName 'bin\Hostx64\x64\cl.exe'
$libExe = Join-Path $toolsetDir.FullName 'bin\Hostx64\x64\lib.exe'

foreach ($tool in @($clExe, $libExe)) {
    if (-not (Test-Path $tool)) {
        throw "Required tool was not found at $tool"
    }
}

& $clExe /nologo /c /TC /Fo"$objectPath" "$sourcePath"
if ($LASTEXITCODE -ne 0) {
    throw 'Failed to compile dformt_x64_stub.c'
}

& $libExe /NOLOGO /MACHINE:X64 /OUT:"$outputLib" "$objectPath"
if ($LASTEXITCODE -ne 0) {
    throw 'Failed to create dformt_x64.lib'
}

Write-Host "Built $outputLib"