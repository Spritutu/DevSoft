; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!


;宏定义
#define AppName  "Edislab Pro"
#define AppVerName "Edislab Pro v1.0"
#define AppDefaultGroupName "Edislab"
#define AppPublisher "Edislab教育"#define DefaultOutputDir "C:\Users\Administrator\Desktop"
;#define DefaultAppCopyright "Copyright(C)2014-2015 Incxxx,Inc"
#define MutexName "05F14B9D-5AF2-4521-8B3A-37CC74AA2F22"
#define AppVerSion "1.0.0.0"
#define SetupVersion "1.0"
#define AppID "F4B17EA0-36EE-489F-BE5C-9DC8538C3F77"
#define Architecture "x64"
[Setup]
AppId={#AppID}
;程序名称
AppName={#AppName}
;程序版本名称
AppVerName={#AppVerName}
AppVersion={#AppVerSion}
;安装包的版本
VersionInfoVersion={#SetupVersion}
;默认的安装目录
DefaultDirName={pf}\Edislab
;默认的组名
DefaultGroupName={#AppDefaultGroupName}
;发布者名称AppPublisher={#AppPublisher}
;压缩相关
Compression=lzma
SolidCompression=yes
;指定背景为渐变色
WindowVisible=yes
;生成的安装文件目录
OutputDir={#DefaultOutputDir}
;源文件目录
;SourceDir={#SourceDir}
;安装文件名称
OutputBaseFilename="Setup_{#AppVerSion}_{#Architecture}"

SetupIconFile="Setup.ico"

AppMutex={#MutexName}
;AppCopyright={#DefaultAppCopyright}

ArchitecturesAllowed=x64

ArchitecturesInstallIn64BitMode=x64 ia64
[Tasks]
Name: desktopicon; Description: "创建桌面快捷方式(&D)"; GroupDescription: "添加快捷方式:"
Name: desktopicon\common; Description: "对于所有用户"; GroupDescription: "添加快捷方式:";Flags: exclusive 
Name: desktopicon\user; Description: "仅对当前用户"; GroupDescription: "添加快捷方式"; Flags: unchecked 

[Dirs]
;创建多余的目录
;Name:"{app}\Log"
;Name:"{app}\HisDots"
;Name:"{app}\RadarTempFiles"

[Files]
;Source:"vcredist_x64.exe";DestDir: "{tmp}";Check:NeedInstallVC9SP1
Source:"vcredist_x64.exe";DestDir: "{tmp}";Flags:ignoreversion
;Source:"xvid.exe";DestDir: "{tmp}";Flags:ignoreversion
Source:"Edislab Pro.exe";DestDir:{app};Flags:ignoreversion
;Source:"Setting.exe";DestDir:{app};Flags:ignoreversion
;Source:"交通显示处理子系统使用手册.chm";DestDir:{app};Flags:ignoreversion
Source:"bmp\*";DestDir:"{app}\bmp";Flags:ignoreversion recursesubdirs createallsubdirs
Source:"driver\*";DestDir:"{app}\driver";Flags:ignoreversion recursesubdirs createallsubdirs
;Source:"Cursor\*";DestDir:"{app}\Cursor";Flags:ignoreversion recursesubdirs createallsubdirs
;Source:"ICon\*";DestDir:"{app}\ICon";Flags:ignoreversion recursesubdirs createallsubdirs
;Source:"hik\*";DestDir:"{app}\hik";Flags:ignoreversion recursesubdirs createallsubdirs
;Source:"SeaMap\*";DestDir:"{app}\SeaMap";Flags:ignoreversion recursesubdirs createallsubdirs
;Source:"VTSConfig\*";DestDir:"{app}\VTSConfig";Flags:ignoreversion recursesubdirs createallsubdirs
;Source:"Wav\*";DestDir:"{app}\Wav";Flags:ignoreversion recursesubdirs createallsubdirs
Source:"*.dll";Excludes:"*d.dll,*d100.dll";DestDir:"{app}"
;Source:"*.bat";DestDir:"{app}"
;Source:"*.ocx";DestDir:"{app}"
Source:"UnInstall.ico";DestDir:"{app}"
Source:"SensorConfig.json";DestDir:"{app}"
Source:"Edislab Pro.ico";DestDir:"{app}";Source:"版本变更内容.txt";DestDir:{app};

[Icons]
Name:"{group}\{#AppName}";Filename:"{app}\Edislab Pro.exe";WorkingDir:"{app}";IconFilename:"{app}\Edislab Pro.ico"
Name:"{group}\卸载程序";Filename:"{uninstallexe}";WorkingDir:"{app}";IconFilename:"{app}\Uninstall.ico"
;Name:"{group}\程序配置";Filename:"{app}\Setting.exe";WorkingDir:"{app}";IconFilename:"{app}\Setting.exe"
Name:"{userdesktop}\Edislab Pro";Filename:"{app}\Edislab Pro.exe";WorkingDir:"{app}";IconFilename:"{app}\Edislab Pro.ico"

[UninstallDelete]
Type:filesandordirs;Name:"{app}"



[Run]
Filename: "{tmp}\vcredist_x64.exe"; Parameters: /q; WorkingDir: {tmp}; Flags: skipifdoesntexist; StatusMsg: "Installing Microsoft Visual C++ Runtime ..."; 
Filename: "{app}\driver\CP210xVCPInstaller_x64.exe";  WorkingDir: "{app}\driver"; Flags: skipifdoesntexist; StatusMsg: "正在安装驱动..."
;Filename: "regsvr32.exe"; Parameters: "/s YimaEnc.ocx"; WorkingDir: "{app}"; StatusMsg: "正在注册海图显示引擎...";Flags:
;Filename:"{app}\TDPS.exe";Description:"运行VTS";Flags:postinstall unchecked nowait
;Filename:"{app}\Setting.exe";Description:"程序配置";Flags:postinstall nowait
;Filename: "{app}\版本变更内容.txt"; Description: "查看版本变更内容"; Flags: postinstall shellexec skipifsilent

[UninstallRun]
;Filename: "regsvr32.exe"; Parameters: "/s /u YimaEnc.ocx"; WorkingDir: "{app}"; StatusMsg: "正在反注册海图显示引擎..."; Flags:  




;检测是否安装了VC Runtime Lib
[Code]
varvc9SP1Missing: Boolean;
function NeedInstallVC9SP1(): Boolean;
begin
 Result := vc9SP1Missing;
end;

function InitializeSetup(): Boolean;
var
 version: Cardinal;
begin
 if RegQueryDWordValue(HKEY_LOCAL_MACHINE,'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{1D8E6291-B0D5-35EC-8441-6616F567A0F7}','Version',version) = false
 then
   begin
     vc9SP1Missing := true;
   end;
 result := true;
end;



