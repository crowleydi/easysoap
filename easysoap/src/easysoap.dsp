# Microsoft Developer Studio Project File - Name="easysoap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=easysoap - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "easysoap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "easysoap.mak" CFG="easysoap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "easysoap - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "easysoap - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "easysoap - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../win32/Release"
# PROP Intermediate_Dir "../win32/Release/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EASYSOAP_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EASYSOAP_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ../win32/Release/expat.lib ws2_32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "easysoap - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../win32/Debug"
# PROP Intermediate_Dir "../win32/Debug/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EASYSOAP_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EASYSOAP_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../win32/Debug/expat.lib ws2_32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "easysoap - Win32 Release"
# Name "easysoap - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SOAPArrayHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPBase64.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPBody.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPBodyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPClientSocketImp.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPDebugger.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPEnvelope.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPEnvelopeHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPFault.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPFaultHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPHeaderHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPMethod.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPMethodHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPonHTTP.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPPacketWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPParameterHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPParse.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPQName.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPResponse.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPSecureSocketImp.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPStructHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPUrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLParser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\SOAP.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPArray.h
# End Source File
# Begin Source File

SOURCE=.\SOAPArrayHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPBase64.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPBody.h
# End Source File
# Begin Source File

SOURCE=.\SOAPBodyHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPCGIHandler.h
# End Source File
# Begin Source File

SOURCE=.\SOAPClientSocketImp.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPDebugger.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPEnvelope.h
# End Source File
# Begin Source File

SOURCE=.\SOAPEnvelopeHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPException.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPFault.h
# End Source File
# Begin Source File

SOURCE=.\SOAPFaultHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPHashMap.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPHeader.h
# End Source File
# Begin Source File

SOURCE=.\SOAPHeaderHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPMethod.h
# End Source File
# Begin Source File

SOURCE=.\SOAPMethodHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPNamespaces.h
# End Source File
# Begin Source File

SOURCE=.\SOAPonHTTP.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPPacketWriter.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPParameter.h
# End Source File
# Begin Source File

SOURCE=.\SOAPParameterHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPParse.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPParseEventHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPProxy.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPQName.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPResponse.h
# End Source File
# Begin Source File

SOURCE=.\SOAPSecureSocketImp.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPSocket.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPString.h
# End Source File
# Begin Source File

SOURCE=.\SOAPStructHandler.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPTransport.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPUrl.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPUtil.h
# End Source File
# Begin Source File

SOURCE=..\include\XMLParser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
