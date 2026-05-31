# Script: check_and_copy.ps1
# Audits all SOURCE= files from SolveIt.dsp against the current workspace,
# restores missing files from the local missing/ archive when possible,
# mirrors recovered project-root files into .\src, and records any unresolved
# entries in .\this_file_is_missing.txt.

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$miss = Join-Path $root "missing"
$src = Join-Path $root "src"
$manifest = Join-Path $root "this_file_is_missing.txt"
$dspPath = Join-Path $root "SolveIt.dsp"

function Get-TargetPath {
    param(
        [string]$ProjectRoot,
        [string]$RelativePath
    )

    if ($RelativePath -like ".\*") {
        return [PSCustomObject]@{
            FullPath = Join-Path $ProjectRoot $RelativePath.Substring(2)
            MirrorToSrc = $true
            MissingRelative = $RelativePath.Substring(2)
        }
    }

    if ($RelativePath -like "..\*") {
        $trimmed = $RelativePath.Substring(3)
        $firstSegment = ($trimmed -split "\\")[0]
        $inRepoCandidate = Join-Path $ProjectRoot $trimmed
        $parentCandidate = Join-Path (Split-Path $ProjectRoot -Parent) $trimmed

        if (Test-Path (Join-Path $ProjectRoot $firstSegment)) {
            return [PSCustomObject]@{
                FullPath = $inRepoCandidate
                MirrorToSrc = $false
                MissingRelative = $trimmed
            }
        }

        return [PSCustomObject]@{
            FullPath = $parentCandidate
            MirrorToSrc = $false
            MissingRelative = $trimmed
        }
    }

    return [PSCustomObject]@{
        FullPath = Join-Path $ProjectRoot $RelativePath
        MirrorToSrc = $true
        MissingRelative = $RelativePath
    }
}

if (-not (Test-Path $src)) {
    New-Item -ItemType Directory -Path $src -Force | Out-Null
}

$dspLines = Get-Content $dspPath | Where-Object { $_ -match "^SOURCE=" }
$report = @()

foreach ($line in $dspLines) {
    $rel = ($line -replace "SOURCE=", "").Trim()
    $target = Get-TargetPath -ProjectRoot $root -RelativePath $rel
    $full = $target.FullPath

    if (Test-Path $full) {
        $report += [PSCustomObject]@{ File = $rel; Status = "EXISTS" }
        continue
    }

    $fname = Split-Path $full -Leaf
    $found = $false
    $expectedMissingPath = Join-Path $miss $target.MissingRelative

    if (Test-Path $expectedMissingPath) {
        $destDir = Split-Path $full -Parent
        if (-not (Test-Path $destDir)) {
            New-Item -ItemType Directory -Path $destDir -Force | Out-Null
        }

        Copy-Item $expectedMissingPath $full -Force

        if ($target.MirrorToSrc) {
            Copy-Item $expectedMissingPath (Join-Path $src $fname) -Force
        }

        $report += [PSCustomObject]@{ File = $rel; Status = "COPIED_FROM_missing" }
        $found = $true
    }

    if (-not $found) {
        $candidates = Get-ChildItem -Path $miss -Recurse -Filter $fname -File -ErrorAction SilentlyContinue
        if ($candidates) {
            $destDir = Split-Path $full -Parent
            if (-not (Test-Path $destDir)) {
                New-Item -ItemType Directory -Path $destDir -Force | Out-Null
            }

            Copy-Item $candidates[0].FullName $full -Force

            if ($target.MirrorToSrc) {
                Copy-Item $candidates[0].FullName (Join-Path $src $fname) -Force
            }

            $report += [PSCustomObject]@{ File = $rel; Status = "COPIED_FROM_missing_search" }
            $found = $true
        }
    }

    if (-not $found) {
        $report += [PSCustomObject]@{ File = $rel; Status = "STILL_MISSING" }
    }
}

$missingEntries = $report | Where-Object { $_.Status -eq "STILL_MISSING" } | Select-Object -ExpandProperty File

if ($missingEntries.Count -gt 0) {
    Set-Content -Path $manifest -Value $missingEntries
} else {
    Set-Content -Path $manifest -Value "No unresolved files from SolveIt.dsp."
}

Write-Host "=== RESULTS ==="
$report | Group-Object Status | ForEach-Object { Write-Host "$($_.Name): $($_.Count)" }
Write-Host ""
Write-Host "=== STILL MISSING ==="
$missingEntries | ForEach-Object { Write-Host $_ }
