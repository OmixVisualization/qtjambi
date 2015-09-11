; Installer script for qt jambi for windows
;

; -------------- Headers ------------
!include MUI2.nsh

; --------------- General --------------

CRCCheck force
XPStyle on
SetCompressor /FINAL /SOLID lzma
;SetCompressorDictSize 64
!define v 4.7.1
!define QtDir C:\Qt\4.7.1
!define comp mingw
InstallDir C:\qtjambi-${v} ; The default installation directory
Name "Qt Jambi Installer" ; The name of the installer
OutFile "setup-qt-jambi-${v}-win32-beta2.exe"; The file to write
BrandingText /TRIMLEFT "Qtjambi-${v}"
RequestExecutionLevel admin

; ----------- Icon and Bitmap ---------

;!define MUI_ICON install.ico : TODO: find suitable icon
;!define MUI_UNICON uninstall.ico ; TODO: find suitable icon

; -------------------------------------

!define MUI_ABORTWARNING

;------------- Language Selection Dialog Settings --------------

!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\Qtjambi" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;-------------- Pages -------------

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ..\LICENSE.GPL3
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

    ; These indented statements modify settings for MUI_PAGE_FINISH
    !define MUI_FINISHPAGE_NOAUTOCLOSE
    !define MUI_FINISHPAGE_RUN
	!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchDemo"
    !define MUI_FINISHPAGE_RUN_CHECKED
    !define MUI_FINISHPAGE_RUN_TEXT "Launch Qtjambi Demo"
    !define MUI_FINISHPAGE_SHOWREADME_CHECKED
    ;!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\changes-4.7.0-beta2" ; must have a .txt or .rtf suffix to work
	!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------- Languages ---------------

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Breton"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Esperanto"

; ------------ Reserve Files ------------

!insertmacro MUI_RESERVEFILE_LANGDLL

; ------------- Installer Functions ----------

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;-------------- Uninstaller Functions ------------

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

;------------ Installer -------------

Section "" ; No components page, name is not important
; Check for java
Var /GLOBAL abort
Var /GLOBAL abortMsg
Call checkJava
strCmp $abort "true" 0 +2
Abort "Installation aborted by user!"

SetOutPath $INSTDIR ; Set output path to the installation directory.
WriteUninstaller $INSTDIR\uninstall.exe ; Tell it where to put the uninstaller
; Qtjambi files
File ..\..\qtjambi-${v}.jar
File ..\..\qtjambi-designer-${v}.jar
File ..\..\qtjambi-examples-${v}.jar
File ..\..\build\qtjambi-win32-${comp}-${v}.jar
File examples.bat
File examples.vbs
File designer.bat
File designer.vbs
;File ..\changes-${v} ; TODO: Fails if no such file is there
File ..\changes-4.7.0-beta2 ; DEBUG
File ..\install.html
File ..\LICENSE.GPL3
File ..\LICENSE.LGPL
File ..\README.txt
;File ..\readme.html ; TODO: Fails if no such file is there

; Qt files + Juic
SetOutPath $INSTDIR\bin
File ..\..\bin\juic.exe
File ${QtDir}\bin\mingwm10.dll
File ${QtDir}\bin\libgcc_s_dw2-1.dll

File ${QtDir}\bin\designer.exe
File ${QtDir}\bin\linguist.exe
File ${QtDir}\bin\lrelease.exe
File ${QtDir}\bin\lupdate.exe
File ${QtDir}\bin\rcc.exe

File ${QtDir}\bin\QtCore4.dll
File ${QtDir}\bin\QtDesigner4.dll
File ${QtDir}\bin\QtDesignerComponents4.dll
File ${QtDir}\bin\QtGui4.dll
File ${QtDir}\bin\QtNetwork4.dll
File ${QtDir}\bin\QtXml4.dll
File ${QtDir}\bin\QtScript4.dll

SetOutPath $INSTDIR\plugins\designer
File /r ${QtDir}\plugins\designer\*.dll 

; Shortcuts 
CreateDirectory "$SMPROGRAMS\Qtjambi"
CreateShortCut "$SMPROGRAMS\Qtjambi\Qtjambi Demo.lnk" "$INSTDIR\examples.vbs"
CreateShortCut "$SMPROGRAMS\Qtjambi\Designer.lnk" "$INSTDIR\designer.vbs"
CreateShortCut "$SMPROGRAMS\Qtjambi\Uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

;------------ Uninstaller -------------

Section "uninstall"

; Bin
Delete $INSTDIR\bin\mingwm10.dll
Delete $INSTDIR\bin\libgcc_s_dw2-1.dll

Delete $INSTDIR\bin\designer.exe
Delete $INSTDIR\bin\linguist.exe
Delete $INSTDIR\bin\lrelease.exe
Delete $INSTDIR\bin\lupdate.exe
Delete $INSTDIR\bin\rcc.exe
Delete $INSTDIR\bin\juic.exe

Delete $INSTDIR\bin\QtCore4.dll
Delete $INSTDIR\bin\QtDesigner4.dll
Delete $INSTDIR\bin\QtDesignerComponents4.dll
Delete $INSTDIR\bin\QtGui4.dll
Delete $INSTDIR\bin\QtNetwork4.dll
Delete $INSTDIR\bin\QtXml4.dll
Delete $INSTDIR\bin\QtScript4.dll
RMDir  $INSTDIR\bin

; Plugins
RMDir /r $INSTDIR\plugins

; Qtjambi files
Delete $INSTDIR\uninstall.exe
Delete $INSTDIR\qtjambi-${v}.jar
Delete $INSTDIR\qtjambi-designer-${v}.jar
Delete $INSTDIR\qtjambi-examples-${v}.jar
Delete $INSTDIR\examples.bat
Delete $INSTDIR\examples.vbs
Delete $INSTDIR\designer.bat
Delete $INSTDIR\designer.vbs
Delete $INSTDIR\qtjambi-win32-${comp}-${v}.jar
;Delete $INSTDIR\changes-${v} ; TODO: Fails if no such file is there
Delete $INSTDIR\changes-4.7.0-beta2 ; DEBUG
Delete $INSTDIR\install.html
Delete $INSTDIR\LICENSE.GPL3
Delete $INSTDIR\LICENSE.LGPL
Delete $INSTDIR\README.txt
; Delete $INSTDIR\readme.html ; TODO: Fails if no such file is there
RMDir  $INSTDIR

; Removing shortcuts
Delete "$SMPROGRAMS\Qtjambi\Qtjambi Demo.lnk"
Delete "$SMPROGRAMS\Qtjambi\Designer.lnk"
Delete "$SMPROGRAMS\Qtjambi\Uninstall.lnk"
RMDir "$SMPROGRAMS\Qtjambi"
SectionEnd

; Function for testing installed java environment
Function checkJava
; Variables
VAR /GLOBAL version
readVersion:
readRegStr $version HKLM "SOFTWARE\JavaSoft\Java Runtime Environment" CurrentVersion
StrCmp $version "" 0 +2
MessageBox MB_ABORTRETRYIGNORE "Java was not found on your system. This way Qtjambi \
                                Will not be functional after installation. Make sure \
                                Java is installed before running Qtjambi." \
                                IDABORT abort IDRETRY retry
; ignore
StrCpy $abort "false"
Return
abort:
	StrCpy $abort "true"
	StrCpy $abortMsg "Java is not installed!"
	Return
retry:
	Goto readVersion
FunctionEnd

; Function for launching Qtjambi Demo at the end of the installation
Function LaunchDemo
  ExecShell "" "$SMPROGRAMS\Qtjambi\Qtjambi Demo.lnk"
FunctionEnd
