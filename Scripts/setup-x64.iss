; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Kiwi"
#define MyAppVersion "v1.0.3"
#define MyAppPublisher "CICM"
#define MyAppURL "http://kiwi.mshparisnord.fr/"
#define MyAppExeName "Kiwi.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{58EF36E0-CF1A-40B1-B918-D6E38E30E6D7}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableDirPage=yes
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputDir=..
OutputBaseFilename=Kiwi-x64-setup
SetupIconFile=..\Ressources\Project\VisualStudio15\Icon.ico
Compression=lzma
SolidCompression=yes
ChangesAssociations = yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: mypAssociation; Description: "Associate ""kiwi"" extension"; GroupDescription: File extensions:

[Files]
Source: "..\Build\KiwiBuild\Release\Kiwi.exe"; DestDir: "{app}"; Flags: ignoreversion; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "..\ThirdParty\faust\libraries\*"; DestDir: "{app}\Faust\Libs"; Flags: ignoreversion; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Registry]

Root: HKCR; Subkey: ".kiwi"; ValueData: "{#MyAppName}"; Flags: uninsdeletevalue; ValueType: string;  ValueName: ""; Tasks: mypAssociation
Root: HKCR; Subkey: "{#MyAppName}"; ValueData: "Program {#MyAppName}";  Flags: uninsdeletekey;   ValueType: string;  ValueName: ""; Tasks: mypAssociation
Root: HKCR; Subkey: "{#MyAppName}\DefaultIcon";             ValueData: "{app}\{#MyAppExeName},0";               ValueType: string;  ValueName: ""; Tasks: mypAssociation
Root: HKCR; Subkey: "{#MyAppName}\shell\open\command";  ValueData: """{app}\{#MyAppExeName}"" ""%1""";  ValueType: string;  ValueName: ""; Tasks: mypAssociation

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
