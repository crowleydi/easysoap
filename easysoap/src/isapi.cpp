//
// isapi.c : Defines the entry point for the DLL application.
//
//
// This file implements the standard DLL entry points for an
// ISAPI module.  This is free code.  It was taken and derived
// by several examples in the Microsoft documentation.
//
// This module creates a thread pool for processing the incoming
// requests.  Each thread executes the external method
// ThreadProc() which is where we put the SOAP server dispatch
// stuff.  Each thread will have it's own dispatch object and
// should make it easier to keep things thread safe.
//
//
// This file needs to be compiled and linked with code
// which implements the ThreadProc() method.
//
//
#include <httpext.h>

//
// Predeclare the thread method we're going to call.
DWORD WINAPI ThreadProc(void *);

//
//  It would be cool if this was configurable through
//  a registry key or something...
//
#define numWorkerThreads 10


//
// This can be changed to anything you want...
const char *extensionDesc = "EasySoap++ ISAPI DLL";


//
// Global variable to keep track of our threads
// and our IoPort.
//
HANDLE	gWorkerThreads[numWorkerThreads];
HANDLE	gIoPort;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}




//
// This method is called by IIS when the module
// is first loaded.  We use this to initialize
// our thread pool.
//
BOOL WINAPI
GetExtensionVersion( HSE_VERSION_INFO* pVer )
{
	long n;

	if (pVer)
	{
		pVer->dwExtensionVersion = HSE_VERSION;
		lstrcpyn(pVer->lpszExtensionDesc, extensionDesc, HSE_MAX_EXT_DLL_NAME_LEN);
	}

	gIoPort = CreateIoCompletionPort((HANDLE)INVALID_HANDLE_VALUE, NULL, 0, 0);

	for (n = 0; n < numWorkerThreads; ++n)
	{
		DWORD id;
		gWorkerThreads[n] = CreateThread(NULL, 0, ThreadProc, gIoPort, 0, &id);
	}
	return TRUE;
}

//
// This method is called by IIS for each request the
// module receives.  We pass the ECB to one of the
// the threads in our pool to complete the processing
// and then return.
//
DWORD WINAPI
HttpExtensionProc( EXTENSION_CONTROL_BLOCK *pECB )
{
	if (!pECB)
		return HSE_STATUS_ERROR;

	if (!PostQueuedCompletionStatus(gIoPort, (DWORD)pECB, 0, NULL))
	{
		DWORD	dwBufLen;
		char	szBuffer[128] = { 0 };

		wsprintf(szBuffer, "<H1>Error posting to completion port! Win32 Error = %i</H1>", GetLastError());
		dwBufLen = strlen(szBuffer);

		pECB->ServerSupportFunction(pECB->ConnID, HSE_REQ_SEND_RESPONSE_HEADER,
			NULL, 0, (DWORD *)"Content-Type: text/html\r\n\r\n");
		pECB->WriteClient(pECB->ConnID, szBuffer, &dwBufLen, 0);

		return HSE_STATUS_ERROR;
	}
	return HSE_STATUS_PENDING;
}

//
// This method is called by IIS when the module is going to
// be unloaded.  We tell all the threads to terminate, wait
// for them, clean up our IoPort and then exit.
//
BOOL WINAPI
TerminateExtension( DWORD dwFlags )
{
	long n;
	for (n = 0; n < numWorkerThreads; ++n)
		PostQueuedCompletionStatus(gIoPort, 0, 0, (OVERLAPPED*)0xFFFFFFFF);

	WaitForMultipleObjects(numWorkerThreads, gWorkerThreads, TRUE, 120000);
	CloseHandle(gIoPort);

	return TRUE;
}

 

