[Setup]
AppName=AUTBank
AppVersion=1.0
DefaultDirName={pf}\AUTBank
DefaultGroupName=AUTBank
OutputDir=.
OutputBaseFilename=AUTBank
Compression=lzma
SolidCompression=yes
SetupIconFile=.\Logo.ico
AppPublisher=AbdulAhad

[Files]
Source: "Main.exe"; DestDir: "{app}"
Source: "server.exe"; DestDir: "{app}"

[Icons]
Name: "{group}\AUTBank"; Filename: "{app}\Main.exe"; IconFilename: "{app}\Main.exe"
Name: "{commondesktop}\AUTBank"; Filename: "{app}\Main.exe"; Tasks: desktopicon; IconFilename: "{app}\Main.exe"

[Tasks]
Name: desktopicon; Description: "Create a desktop icon"

[Run]
Filename: "{app}\Main.exe"; Description: "Launch AUTBank"; Flags: nowait postinstall skipifsilent