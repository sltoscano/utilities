#include "StdAfx.h"

#include "StrSafe.h"

#include "copyfile.h"
#include "controller.h"


namespace Utils
{
	struct CopyFileData
	{
		HWND status;
		TCHAR* pFileName;
	};

	DWORD CALLBACK CopyFileCallback(
		LARGE_INTEGER totalFileSize,
		LARGE_INTEGER totalBytesTransferred,
		LARGE_INTEGER /*streamSize*/,
		LARGE_INTEGER /*streamBytesTransferred*/,
		DWORD /*dwStreamNumber*/,
		DWORD /*dwCallbackReason*/,
		HANDLE /*hSourceFile*/,
		HANDLE /*hDestinationFile*/,
		LPVOID lpData)
	{
		if (Controller::IsCopyCancelled())
			return PROGRESS_CANCEL;
		
		CopyFileData* pFileData = (CopyFileData *) lpData;

//		GetTickCount()

		Controller::UpdateProgress(pFileData->status, pFileData->pFileName, 
			totalFileSize.QuadPart, totalBytesTransferred.QuadPart);

		return PROGRESS_CONTINUE;
	}

	DWORD ValidateCopyDest(const CString& pathFrom, const CString& pathTo, const CString& /*fileSpec*/)
	{
		if (GetFileAttributes(pathFrom) && GetLastError() == ERROR_FILE_NOT_FOUND)
			return ERROR_FILE_NOT_FOUND;

		if (CreateDirectory(pathTo, NULL) == FALSE && GetLastError() != ERROR_ALREADY_EXISTS)
			return GetLastError();

		return ERROR_SUCCESS;
	}

	DWORD CopyFiles(const HWND status, const CString& pathFrom, const CString& pathTo, const CString& fileSpec, CString& lastFileAttempted)
	{
		WIN32_FIND_DATA findData;
		HANDLE hFile = FindFirstFile(pathFrom + _T("\\") + fileSpec, &findData);

		if (hFile == INVALID_HANDLE_VALUE)
			return ERROR_FILE_INVALID;

		do
		{
			CString newPathFrom = pathFrom + _T("\\") + findData.cFileName;
			CString newPathTo = pathTo + _T("\\") + findData.cFileName;

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(findData.cFileName, _T(".")) == 0 || _tcscmp(findData.cFileName, _T("..")) == 0)
					continue;

				if (CreateDirectory(newPathTo, NULL) == FALSE && GetLastError() != ERROR_ALREADY_EXISTS)
					return GetLastError();

				CopyFiles(status, newPathFrom, newPathTo, fileSpec, lastFileAttempted);
			}
			else
			{
				CopyFileData fileData = { status, newPathTo.GetBuffer() };
				if (CopyFileEx(newPathFrom, newPathTo, CopyFileCallback, &fileData, FALSE, 0) == FALSE)
				{
					lastFileAttempted = newPathTo;
					return GetLastError();
				}
			}
		} while (FindNextFile(hFile, &findData) != FALSE);

		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES)
			dwError = ERROR_SUCCESS;

		FindClose(hFile);
		return dwError;
	}


	DWORD CountFiles(const CString& pathFrom, const CString& fileSpec, DWORD& totalFiles, __int64& totalSize)
	{
		WIN32_FIND_DATA findData;
		HANDLE hFile = FindFirstFile(pathFrom + _T("\\") + fileSpec, &findData);

		if (hFile == INVALID_HANDLE_VALUE)
			return ERROR_FILE_INVALID;

		if (GetFileAttributes(pathFrom) && GetLastError() == ERROR_FILE_NOT_FOUND)
			return ERROR_FILE_NOT_FOUND;

		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(findData.cFileName, _T(".")) == 0 || _tcscmp(findData.cFileName, _T("..")) == 0)
					continue;

				CString newPathFrom = pathFrom + _T("\\") + findData.cFileName;
				CountFiles(newPathFrom, fileSpec, totalFiles, totalSize);
			}
			else
			{
				LARGE_INTEGER fileSize;
				fileSize.HighPart = findData.nFileSizeHigh;
				fileSize.LowPart = findData.nFileSizeLow;
				totalSize += fileSize.QuadPart;
				totalFiles++;
			}
		} while (FindNextFile(hFile, &findData) != FALSE);

		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES)
			dwError = ERROR_SUCCESS;

		FindClose(hFile);
		return dwError;
	}
};
