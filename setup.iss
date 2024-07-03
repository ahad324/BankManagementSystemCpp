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
Source: "/Policy/PolicyChanger.exe"; DestDir: "{app}"
Source: "Server.js"; DestDir: "{app}"
Source: "output.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "package.json"; DestDir: "{app}"; Flags: ignoreversion
Source: ".env"; DestDir: "{app}"; Flags: ignoreversion
Source: "node_modules\*"; DestDir: "{app}\node_modules"; Flags: recursesubdirs createallsubdirs
Source: "Policy\*"; DestDir: "{app}\Policy";

[Icons]
Name: "{group}\AUTBank"; Filename: "{app}\Main.exe"; IconFilename: "{app}\Main.exe"
Name: "{commondesktop}\AUTBank"; Filename: "{app}\Main.exe"; Tasks: desktopicon; IconFilename: "{app}\Main.exe"

[Tasks]
Name: desktopicon; Description: "Create a desktop icon"

[Run]
Filename: "{app}/Policy/PolicyChanger.exe"; Description: "Policy Checker"; Flags: nowait postinstall skipifsilent runhidden
Filename: "{app}\Main.exe"; Description: "Launch AUTBank"; Flags: nowait postinstall skipifsilent