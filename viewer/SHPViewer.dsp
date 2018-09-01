# Microsoft Developer Studio Project File - Name="SHPViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SHPViewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SHPViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SHPViewer.mak" CFG="SHPViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SHPViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SHPViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SHPViewer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"SHPViewer.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SHPViewer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"SHPViewer.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SHPViewer - Win32 Release"
# Name "SHPViewer - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\FieldDLG.h
# End Source File
# Begin Source File

SOURCE=.\GPoint.h
# End Source File
# Begin Source File

SOURCE=.\InfoDLG.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\Projection.h
# End Source File
# Begin Source File

SOURCE=.\Projection_UTM.h
# End Source File
# Begin Source File

SOURCE=.\PSupport.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SHP.h
# End Source File
# Begin Source File

SOURCE=.\shpDBF.h
# End Source File
# Begin Source File

SOURCE=.\shpMultiPoint.h
# End Source File
# Begin Source File

SOURCE=.\shpObject.h
# End Source File
# Begin Source File

SOURCE=.\shpPoint.h
# End Source File
# Begin Source File

SOURCE=.\shpPoly.h
# End Source File
# Begin Source File

SOURCE=.\shpPolygon.h
# End Source File
# Begin Source File

SOURCE=.\SHPViewer.h
# End Source File
# Begin Source File

SOURCE=.\SHPViewerDoc.h
# End Source File
# Begin Source File

SOURCE=.\SHPViewerView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\SHPViewer.ico
# End Source File
# Begin Source File

SOURCE=.\res\SHPViewer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SHPViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoom_cen.cur
# End Source File
# Begin Source File

SOURCE=.\res\zoom_in.cur
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FieldDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\GPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Projection.cpp
# End Source File
# Begin Source File

SOURCE=.\Projection_UTM.cpp
# End Source File
# Begin Source File

SOURCE=.\PSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\SHP.cpp
# End Source File
# Begin Source File

SOURCE=.\shpDBF.cpp
# End Source File
# Begin Source File

SOURCE=.\shpMultiPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\shpObject.cpp
# End Source File
# Begin Source File

SOURCE=.\shpPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\shpPoly.cpp
# End Source File
# Begin Source File

SOURCE=.\shpPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\SHPViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\SHPViewer.rc
# End Source File
# Begin Source File

SOURCE=.\SHPViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SHPViewerView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# End Target
# End Project
# Section SHPViewer : {893B2D55-865F-4712-AE01-CD2885CD269C}
# 	2:10:GPoint.cpp:GPoint.cpp
# 	2:8:GPoint.h:GPoint.h
# 	2:13:CLASS: GPoint:GPoint
# 	2:19:Application Include:shpviewer.h
# End Section
# Section SHPViewer : {85BBF712-BC98-4DE5-8DFD-F874B5C39FC0}
# 	2:13:ProgressBar.h:ProgressBar.h
# 	2:19:CLASS: CProgressBar:CProgressBar
# 	2:19:Application Include:shpviewer.h
# 	2:15:ProgressBar.cpp:ProgressBar.cpp
# End Section
