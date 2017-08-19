#include "StdAfx.h"

#include "StrSafe.h"

#include "controller.h"
#include "copyfile.h"
#include "thread.h"


namespace Controller
{
	static bool s_copyCancel = false;
	static HANDLE s_copyThread = NULL;

	struct CopyParams
	{
		TCHAR pathFrom[MAX_PATH];
		TCHAR pathTo[MAX_PATH];
		TCHAR fileSpec[MAX_PATH];
		HWND status;
	};

	DWORD WINAPI CopyProc(LPVOID params)
	{
		CopyParams* copyParams = (CopyParams*) params;

		CString pathFrom(copyParams->pathFrom);
		CString pathTo(copyParams->pathTo);
		CString fileSpec(copyParams->fileSpec);

		DWORD totalFiles = 0;
		LARGE_INTEGER totalSize = {0};
		Utils::CountFiles(pathFrom, fileSpec, totalFiles, totalSize.QuadPart);

		PostMessage(copyParams->status, UM_PROGRESS_SETTOTAL, (WPARAM)totalSize.HighPart, (LPARAM)totalSize.LowPart);

		CString lastFileAttempted;
		DWORD result = Utils::ValidateCopyDest(pathFrom, pathTo, fileSpec);
		if (result == ERROR_SUCCESS)
			result = Utils::CopyFiles(copyParams->status, pathFrom, pathTo, fileSpec, lastFileAttempted);

		if (result != ERROR_SUCCESS)
		{
			ProgressData* data = (ProgressData*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ProgressData));
			StringCbCopy(data->filePath, MAX_PATH, lastFileAttempted);
			PostMessage(copyParams->status, UM_COPY_ERROR, (WPARAM)data, (LPARAM)sizeof(ProgressData));
		}

		PostMessage(copyParams->status, UM_PROGRESS_COMPLETED, totalFiles, result);

		HeapFree(GetProcessHeap(), 0, copyParams);
		copyParams = NULL;
		return result;
	}

	bool PerformCopy(const HWND status, const CString& pathFrom, const CString& pathTo, const CString& fileSpec)
	{
		s_copyCancel = false;

		CopyParams* params = (CopyParams*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(CopyParams));
		
		params->status = status;
		StringCchCopy(params->pathFrom, MAX_PATH, pathFrom);
		StringCchCopy(params->pathTo, MAX_PATH, pathTo);
		StringCchCopy(params->fileSpec, MAX_PATH, fileSpec);

		// The CopyProc thread proc will need to HeapFree the params
		return Utils::StartThread(CopyProc, params, s_copyThread);
	}
	
	void CancelCopy()
	{
		s_copyCancel = true;
		Utils::WaitForThread(s_copyThread);
	}

	bool IsCopyCancelled()
	{
		return s_copyCancel;
	}

	bool UpdateProgress(const HWND status, LPCTSTR fileName, const __int64 totalBytes, const __int64 transferredBytes)
	{
		ProgressData* data = (ProgressData*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ProgressData));

		StringCbCopy(data->filePath, MAX_PATH, fileName);
		data->totalBytes = totalBytes;
		data->transferredBytes = transferredBytes;

		// UI thread that receives this message needs to HeapFree the
		// above HeapAlloc once done with the data
		return PostMessage(status, UM_PROGRESS_UPDATE, (WPARAM)data, (LPARAM)sizeof(ProgressData)) == TRUE ? true : false;
	}
}
