# Microsoft Developer Studio Project File - Name="easysoap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=easysoap - Win32 DebugSSL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "easysoap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "easysoap.mak" CFG="easysoap - Win32 DebugSSL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "easysoap - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "easysoap - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "easysoap - Win32 DebugSSL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "easysoap"
# PROP Scc_LocalPath "."
CPP=cl.exe
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
# PROP Intermediate_Dir "../win32/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "EASYSOAP_STATIC" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "easysoap - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../win32/Debug"
# PROP Intermediate_Dir "../win32/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "EASYSOAP_STATIC" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "easysoap - Win32 DebugSSL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "easysoap___Win32_DebugSSL"
# PROP BASE Intermediate_Dir "easysoap___Win32_DebugSSL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../win32/DebugSSL"
# PROP Intermediate_Dir "../win32/DebugSSL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "EASYSOAP_STATIC" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "../include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "EASYSOAP_STATIC" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../win32/Debug/easysoap.lib"
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "easysoap - Win32 Release"
# Name "easysoap - Win32 Debug"
# Name "easysoap - Win32 DebugSSL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SOAPArrayHandler.cpp
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

SOURCE=.\SOAPParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPParameterHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPParse.cpp
# ADD CPP /I "../../expat/lib"
# End Source File
# Begin Source File

SOURCE=.\SOAPProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPResponse.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPResponseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPSecureSocketImp.cpp

!IF  "$(CFG)" == "easysoap - Win32 Release"

!ELSEIF  "$(CFG)" == "easysoap - Win32 Debug"

!ELSEIF  "$(CFG)" == "easysoap - Win32 DebugSSL"

# ADD CPP /D "EASYSOAP_SSL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOAPSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPStructHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\SOAPUrl.cpp
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

SOURCE=..\include\SOAPBody.h
# End Source File
# Begin Source File

SOURCE=.\SOAPBodyHandler.h
# End Source File
# Begin Source File

SOURCE=.\SOAPClientSocketImp.h
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

SOURCE=..\include\SOAPResponse.h
# End Source File
# Begin Source File

SOURCE=.\SOAPResponseHandler.h
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

SOURCE=..\include\SOAPTypes.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPUrl.h
# End Source File
# Begin Source File

SOURCE=..\include\SOAPUtil.h
# End Source File
# End Group
# End Target
# End Project
