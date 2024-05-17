#NSIS Modern User Interface
#Welcome/Finish Page Example Script
#Written by Joost Verburg
#Modified for Embroidermodder2

SetCompressor /SOLID lzma

!include "MUI2.nsh"

#--------------------------------
#Variables

    Var STARTMENUFOLDER

#Constants
!define _APP_VERSION_ "2.0"
!define _APP_NAME_    "Embroidermodder2"

!define MUI_ICON                       "embroidermodder2.ico"
!define MUI_UNICON                     "embroidermodder2.ico"
!define MUI_COMPONENTSPAGE_CHECKBITMAP "checkboxes.bmp"

!define MUI_WELCOMEFINISHPAGE_BITMAP   "install.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "uninstall.bmp"

#This displays the header image at the top strip rather than the icon on the right
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "header-install.bmp"

BrandingText "Embroidermodder Team"

!define MUI_FINISHPAGE_RUN "embroidermodder2.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Run Embroidermodder2"
!define MUI_FINISHPAGE_SHOWREADME "C:\Program Files\Embroidermodder2\help\index.html"
!define MUI_FINISHPAGE_SHOWREADME_TEXT "View Help"
!define MUI_FINISHPAGE_LINK "Embroidermodder Home Page"
!define MUI_FINISHPAGE_LINK_LOCATION "http://embroidermodder.sourceforge.net/"

#--------------------------------
#General

    #Name and file
    Name "Embroidermodder2"
    OutFile "embroidermodder2-installer.exe"

    #Default installation folder
    InstallDir "$PROGRAMFILES\Embroidermodder2"

    #Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\Embroidermodder2" ""

    #Request application privileges for Windows Vista
    RequestExecutionLevel user

#--------------------------------
#Interface Settings

    !define MUI_ABORTWARNING
    !define MUI_UNABORTWARNING

#--------------------------------
#Pages

    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_LICENSE "LICENSE"
    !insertmacro MUI_PAGE_COMPONENTS
    !insertmacro MUI_PAGE_DIRECTORY

    #Start Menu Folder Page Configuration
    !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
    !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Embroidermodder2" 
    !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
    !insertmacro MUI_PAGE_STARTMENU Application $STARTMENUFOLDER

    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH

    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH

#--------------------------------
#Languages

    !insertmacro MUI_LANGUAGE "English"

#--------------------------------
#Installer Sections

Section "Embroidermodder2" SecEmbMod

    SetOutPath "$INSTDIR"

    #ProgramFiles
    File "..\..\embroidermodder2\embroidermodder2.exe"
    File "..\..\embroidermodder2\*.dll"
    File /r /x *.svn "..\..\embroidermodder2\help"
    File /r /x *.svn "..\..\embroidermodder2\icons"
    File /r /x *.svn "..\..\embroidermodder2\images"

    #StartMenu
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$STARTMENUFOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENUFOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    !insertmacro MUI_STARTMENU_WRITE_END

    #Registry
    WriteRegStr HKCU "Software\Embroidermodder2" "" $INSTDIR

    #Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section "Sample Files" SecSample

    SetOutPath "$INSTDIR"

    #ProgramFiles
    #File /r /x *.svn "..\..\embroidermodder2\samples" #TODO: Add sample files created with Embroidermodder2

SectionEnd

#--------------------------------
#Descriptions

    #Language strings
    LangString DESC_SecEmbMod ${LANG_ENGLISH} "The Main Application"
    LangString DESC_SecSample ${LANG_ENGLISH} "Sample Embroidery Files"

    #Assign language strings to sections
    !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
        !insertmacro MUI_DESCRIPTION_TEXT ${SecEmbMod} $(DESC_SecEmbMod)
        !insertmacro MUI_DESCRIPTION_TEXT ${SecSample} $(DESC_SecSample)
    !insertmacro MUI_FUNCTION_DESCRIPTION_END

#--------------------------------
#Uninstaller Section

Section "Uninstall"

    #ADD YOUR OWN FILES HERE...

    #ProgramFiles
    Delete "$INSTDIR\Uninstall.exe"
    RMDir /r "$INSTDIR"

    #StartMenu
    !insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENUFOLDER
    Delete "$SMPROGRAMS\$STARTMENUFOLDER\Uninstall.lnk"
    RMDir "$SMPROGRAMS\$STARTMENUFOLDER"

    #Registry
    DeleteRegKey /ifempty HKCU "Software\Embroidermodder2"

SectionEnd
