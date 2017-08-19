#pragma once

namespace Utils
{
	DWORD ValidateCopyDest(const CString& pathFrom, const CString& pathTo, const CString& fileSpec);
	DWORD CopyFiles(const HWND status, const CString& pathFrom, const CString& pathTo, const CString& fileSpec, CString& lastFileAttempted);
	DWORD CountFiles(const CString& pathFrom, const CString& fileSpec, DWORD& totalFiles, __int64& totalSize);
};
