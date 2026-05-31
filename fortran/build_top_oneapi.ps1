param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Debug',
    [switch]$Clean
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path $scriptDir -Parent
$vfprojPath = Join-Path $scriptDir 'top.vfproj'
$ifxPath = 'C:\Program Files (x86)\Intel\oneAPI\compiler\latest\bin\ifx.exe'
$visualStudioRoot = 'C:\Program Files\Microsoft Visual Studio\2022\Enterprise'
$msvcToolsRoot = Join-Path $visualStudioRoot 'VC\Tools\MSVC'

if (-not (Test-Path $ifxPath)) {
    throw "ifx.exe was not found at $ifxPath"
}

if (-not (Test-Path $msvcToolsRoot)) {
    throw "MSVC tool directory was not found at $msvcToolsRoot"
}

$toolsetDir = Get-ChildItem $msvcToolsRoot -Directory |
    Sort-Object Name -Descending |
    Select-Object -First 1

if ($null -eq $toolsetDir) {
    throw 'No MSVC toolset directory was found.'
}

$libExe = Join-Path $toolsetDir.FullName 'bin\Hostx64\x64\lib.exe'
if (-not (Test-Path $libExe)) {
    throw "lib.exe was not found at $libExe"
}

$outRoot = Join-Path $scriptDir (Join-Path 'build' (Join-Path $Configuration 'x64'))
$objDir = Join-Path $outRoot 'obj'
$modDir = Join-Path $outRoot 'mod'
$objDirArg = $objDir.TrimEnd('\') + '\'
$libDir = Join-Path $repoRoot 'SolveIt.Lib\lib'
$libName = if ($Configuration -eq 'Debug') { 'top_x64d.lib' } else { 'top_x64.lib' }
$libPath = Join-Path $libDir $libName

if ($Clean) {
    Remove-Item $outRoot -Recurse -Force -ErrorAction SilentlyContinue
    Remove-Item $libPath -Force -ErrorAction SilentlyContinue
}

New-Item -ItemType Directory -Path $objDir -Force | Out-Null
New-Item -ItemType Directory -Path $modDir -Force | Out-Null
New-Item -ItemType Directory -Path $libDir -Force | Out-Null

[xml]$vfproj = Get-Content $vfprojPath
$fileNodes = @($vfproj.SelectNodes('//File'))
if ($fileNodes.Count -eq 0) {
    throw "No <File> entries were found in $vfprojPath"
}

$sources = @(
    $fileNodes |
        ForEach-Object { $_.RelativePath } |
        Where-Object { $_ -match '\.(f|f90)$' } |
        ForEach-Object {
            $relativePath = $_ -replace '^\.\\', ''
            Join-Path $scriptDir $relativePath
        }
)

$topSource = Join-Path $scriptDir 'top.f90'
if (-not ($sources -contains $topSource)) {
    throw 'top.f90 was not found in top.vfproj source list.'
}

$prioritySources = @(
    (Join-Path $scriptDir 'top.f90'),
    (Join-Path $scriptDir 'TRLan\trl_info.f90'),
    (Join-Path $scriptDir 'TRLan\trl_comm_none.f90'),
    (Join-Path $scriptDir 'TRLan\trlaux.f90'),
    (Join-Path $scriptDir 'TRLan\trlcore.f90'),
    (Join-Path $scriptDir 'TRLan\trlan.f90'),
    (Join-Path $scriptDir 'TRLan\trlan77.f90'),
    (Join-Path $scriptDir 'TRLan\restart.f90')
)

$orderedSources = @($prioritySources | Where-Object { $sources -contains $_ }) +
    @($sources | Where-Object { $prioritySources -notcontains $_ })

$commonArgs = @(
    '/nologo',
    '/c',
    '/Qm64',
    '/iface:cvf',
    '/Qsave',
    '/libs:static',
    '/threads',
    "/module:$modDir",
    "/object:$objDirArg",
    "/I$modDir"
)

$configArgs = if ($Configuration -eq 'Debug') {
    @('/debug:full', '/Od', '/check:bounds')
} else {
    @('/debug:full', '/O2')
}

foreach ($source in $orderedSources) {
    Write-Host "Compiling $(Split-Path $source -Leaf)..."
    & $ifxPath @commonArgs @configArgs $source
    if ($LASTEXITCODE -ne 0) {
        throw "Compilation failed for $source"
    }
}

$objFiles = @(Get-ChildItem $objDir -Filter *.obj | Sort-Object Name | Select-Object -ExpandProperty FullName)
if ($objFiles.Count -eq 0) {
    throw "No object files were produced in $objDir"
}

$responsePath = Join-Path $outRoot 'lib.rsp'
Set-Content -Path $responsePath -Value ($objFiles | ForEach-Object { '"' + $_ + '"' })

& $libExe /NOLOGO "/OUT:$libPath" "@$responsePath"
if ($LASTEXITCODE -ne 0) {
    throw "lib.exe failed while producing $libPath"
}

Write-Host "Built $libPath"
