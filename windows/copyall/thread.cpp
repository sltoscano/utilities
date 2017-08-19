#include "StdAfx.h"

#include "thread.h"


namespace Utils
{
	bool StartThread(ThreadProc threadProc, LPVOID threadParams, HANDLE& hThread)
	{
		DWORD dwThreadId;
		hThread = CreateThread(
            NULL,              // default security attributes
            0,                 // use default stack size
            threadProc,        // thread function
            threadParams,      // argument to thread function
            0,                 // use default creation flags
            &dwThreadId);      // returns the thread identifier
 
        // Check the return value for success.
        if (hThread == NULL)
            return false;

		return true;
	}

	bool WaitForThread(HANDLE hThread)
	{
		// Wait until all threads have terminated.
		if (WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0)
			return false;

		// Close all thread handles upon completion.
		if (CloseHandle(hThread) == FALSE)
			return false;

		return true;
	}
};
