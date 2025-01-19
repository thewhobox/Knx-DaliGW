$toolsExist = Test-Path -PathType Leaf ~/bin/Kaenx.Creator.Console.exe

if (!$toolsExist) {
    Write-Host "
        Fuer das Setup fehlen die notwendigen OpenKNX-Tools oder sie sind veraltet..
        Bitte das neuste Paket herunterladen (mindestens Version $checkVersion)

            https://github.com/OpenKNX/Kaenx-Creator-Console/releases
        
        entpacken und das Readme befolgen.

        Danach bitte dieses Script erneut starten.

        ACHTUNG: Heutige Browser warnen vor dem Inhalt des OpenKNX-Tools Pakets, 
                 weil es ausfuehrbare Programme und ein PowerShell-Skript enthaellt.
                 Falls jemand dem Paketinhalt nicht traut, kann er sich OpenKNXproducer
                 und bossac selber kompilieren und entsprechend installieren.
    "
    timeout /T -1
}

if ($toolsExist) {
    $files = Get-ChildItem -Path data/*.ae-manu
    ~/bin/Kaenx.Creator.Console publish data/$($files[0].Name)
    timeout /T 20 
}