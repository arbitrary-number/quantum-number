# setup-msvc-env.ps1

# Setup the PATH for javac
$env:PATH = "C:\Program Files\Amazon Corretto\jdk17.0.16_8\bin;" + $env:PATH

# Optional: clear existing environment
$ErrorActionPreference = "Stop"

# Path to vswhere
$vswherePath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

if (!(Test-Path $vswherePath)) {
    Write-Error "vswhere.exe not found at $vswherePath. Make sure Visual Studio is installed."
    exit 1
}

# Get the latest installed Visual Studio with C++ tools
$vsInfo = & $vswherePath -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath

if (-not $vsInfo) {
    Write-Error "Visual Studio with C++ tools not found."
    exit 1
}

Write-Output "Found Visual Studio at: $vsInfo"

# Path to vcvarsall.bat (used to set up environment)
$vcvarsPath = Join-Path $vsInfo "VC\Auxiliary\Build\vcvars64.bat"

if (!(Test-Path $vcvarsPath)) {
    Write-Error "vcvars64.bat not found at $vcvarsPath"
    exit 1
}

# Use a temporary batch file to capture environment variables
$tempFile = New-TemporaryFile
$envDumpScript = @"
@echo off
call `"$vcvarsPath`" >nul
set > `"$($tempFile.FullName)`"
"@

# Save to a temp batch file
$batFile = [System.IO.Path]::GetTempFileName() + ".bat"
[System.IO.File]::WriteAllText($batFile, $envDumpScript)

# Run the batch file and capture environment
cmd.exe /c "$batFile"

# Load environment variables from the dump into PowerShell session
Get-Content $tempFile.FullName | ForEach-Object {
    $pair = $_ -split '=', 2
    if ($pair.Length -eq 2) {
        [System.Environment]::SetEnvironmentVariable($pair[0], $pair[1], 'Process')
    }
}

# Cleanup
Remove-Item $batFile -Force
Remove-Item $tempFile.FullName -Force

Write-Output "MSVC environment variables are now available in this PowerShell session."
Write-Output "You can now run cl, cmake, or nmake as needed."
