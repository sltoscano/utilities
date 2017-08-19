#pragma once

#define UM_PROGRESS_UPDATE (WM_USER + 100)
#define UM_PROGRESS_COMPLETED (WM_USER + 101)
#define UM_PROGRESS_SETTOTAL (WM_USER + 102)
#define UM_COPY_ERROR (WM_USER + 103)

namespace Controller
{
	struct ProgressData
	{
		TCHAR filePath[MAX_PATH];
		__int64 totalBytes;
		__int64 transferredBytes;
	};

	bool PerformCopy(const HWND status, const CString& pathFrom, const CString& pathTo, const CString& fileSpec);
	void CancelCopy();
	bool IsCopyCancelled();

	bool UpdateProgress(const HWND status, LPCTSTR fileName, const __int64 totalBytes, const __int64 transferredBytes);
}
