#pragma once

namespace Utils
{
	typedef DWORD (WINAPI *ThreadProc)(LPVOID threadParams);
	bool StartThread(ThreadProc threadProc, LPVOID threadParams, HANDLE& hThread);
	bool WaitForThread(HANDLE hThread);
};
