$repoRoot = Split-Path $PSScriptRoot -Parent
$pythonScript = Join-Path $repoRoot 'Tools\ue_asset_doc_exporter_fixed_v3.py'

if (-not (Test-Path $pythonScript)) {
    throw "Script not found: $pythonScript"
}

$launcher = Get-Command py -ErrorAction SilentlyContinue
if ($launcher) {
    if ($args.Count -gt 0) {
        & $launcher.Source $pythonScript @args
    }
    else {
        & $launcher.Source $pythonScript --sync-pending-docs
    }
    exit $LASTEXITCODE
}

$pythonCmd = Get-Command python -ErrorAction SilentlyContinue
if ($pythonCmd) {
    if ($args.Count -gt 0) {
        & $pythonCmd.Source $pythonScript @args
    }
    else {
        & $pythonCmd.Source $pythonScript --sync-pending-docs
    }
    exit $LASTEXITCODE
}

throw "Python launcher not found. Install Python or add py/python to PATH."
