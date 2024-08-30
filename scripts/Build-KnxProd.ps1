# if(-Not (Test-Path -Path platformio.ini)) {
#     Set-Location ..
# }
# if(-Not (Test-Path -Path platformio.ini)) {
#     Write-Host "Skript aus falschem Pfad ausgeführt..."
#     timeout /T 20
#     exit 1
# }
# $projectDir = Get-Location

$files = Get-ChildItem -Path data/*.ae-manu
$files
~/bin/Kaenx.Creator.Console publish data/$($files[0].Name)