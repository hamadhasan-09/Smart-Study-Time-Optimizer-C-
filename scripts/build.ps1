$ErrorActionPreference = 'Stop'

$projectRoot = Split-Path -Parent $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'

if (-not (Test-Path $vswhere)) {
    Write-Error 'Visual Studio Installer was not found. Install Visual Studio C++ Build Tools.'
    exit 1
}

$vsPath = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath |
    Select-Object -First 1

if (-not $vsPath) {
    Write-Error 'MSVC C++ tools were not found. In Visual Studio Installer, install Desktop development with C++.'
    exit 1
}

$setupScript = Join-Path $vsPath 'Common7\Tools\VsDevCmd.bat'
$sourceFile = Join-Path $projectRoot 'main.cpp'
$outputFile = Join-Path $projectRoot 'study.exe'

if (-not (Test-Path $setupScript)) {
    Write-Error "Could not find MSVC environment setup: $setupScript"
    exit 1
}

if (-not (Test-Path $sourceFile)) {
    Write-Error "Missing source file: $sourceFile"
    exit 1
}

$commandLine = "call `"$setupScript`" -arch=x64 >nul && cl /nologo /EHsc /Zi /Fe:`"$outputFile`" `"$sourceFile`""
& cmd.exe /d /c $commandLine

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Build successful. Run .\study.exe in the VS Code terminal."
