#include "StdAfx.h"

#include "copyall.h"
#include "copyallDlg.h"
#include "helpers.h"
#include "taskbar.h"
#include "controller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CcopyallDlg::OnBnClickedOk()
{
	m_progressPart.ShowWindow(SW_SHOW);
	m_progressWhole.ShowWindow(SW_SHOW);
	m_progressPart.SetPos(0);
	m_progressWhole.SetPos(0);
	m_taskBar.SetNoProgressState();
	m_buttonOK.EnableWindow(FALSE);
	m_copyFromEdit.EnableWindow(FALSE);
	m_copyToEdit.EnableWindow(FALSE);
	m_status.SetWindowText(_T("Calculating total copy size..."));

	CString copyFromStr;
	m_copyFromEdit.GetWindowText(copyFromStr);
	CString copyToStr;
	m_copyToEdit.GetWindowText(copyToStr);
	CString fileSpec(_T("*"));

	m_copyStartTime = GetTickCount();

	ResetTotals();
	Controller::PerformCopy(GetSafeHwnd(), copyFromStr, copyToStr, fileSpec);
}

LRESULT CcopyallDlg::OnProgressUpdate(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 1;
	if (lParam != sizeof(Controller::ProgressData))
		return 1;

	Controller::ProgressData* data = (Controller::ProgressData*) wParam;

	__int64 total = data->totalBytes;
	__int64 transferred = data->transferredBytes;
	int percent = Utils::GetPercent(transferred, total);
	m_progressPart.SetPos(percent);

	if (transferred == 0)
		m_previousTransferredSize = 0;
	__int64 currentTransferred = transferred - m_previousTransferredSize;
	m_previousTransferredSize = transferred;

	m_overallTransferredSize += currentTransferred;
	int percentOverall = Utils::GetPercent(m_overallTransferredSize, m_overallTotalSize);
	m_progressWhole.SetPos(percentOverall);

	m_taskBar.SetProgressValue(percentOverall, 100);

	CString truncPath;
	Utils::GetTruncatedMessage(data->filePath, 75, truncPath);

	DWORD timeElapsed = GetTickCount() - m_copyStartTime;
	double secondsElapsed = static_cast<double>(timeElapsed) / 1000.00;
	double bytesPerSecond = static_cast<double>(m_overallTransferredSize) / secondsElapsed;

	Utils::Magnitude ratePerSecond;
	ConvertToMagnitude(static_cast<__int64>(bytesPerSecond), ratePerSecond);

	CString progressText;

	Utils::Magnitude currProgress;
	ConvertToMagnitude(transferred, currProgress);
	Utils::Magnitude totalSize;
	ConvertToMagnitude(total, totalSize);
	Utils::Magnitude currOverallProgress;
	ConvertToMagnitude(m_overallTransferredSize, currOverallProgress);
	Utils::Magnitude totalOverallSize;
	ConvertToMagnitude(m_overallTotalSize, totalOverallSize);

	progressText.Format(_T("File: %s\nCopied: %lld %s / %lld %s (%d%%)\nOverall progress: %lld %s / %lld %s (%d%%)\nRate: %lld %s / sec"),
		truncPath,
		currProgress.value, currProgress.unit,
		totalSize.value, totalSize.unit,
		percent,
		currOverallProgress.value, currOverallProgress.unit,
		totalOverallSize.value, totalOverallSize.unit,
		percentOverall,
		ratePerSecond.value, ratePerSecond.unit);

	m_status.SetWindowText(progressText);

	HeapFree(GetProcessHeap(), 0, data);
	return 0;
}

LRESULT CcopyallDlg::OnProgressCompleted(WPARAM wParam, LPARAM lParam)
{
	m_progressPart.SetPos(0);
	m_progressPart.SetPos(100);
	m_taskBar.SetProgressValue(100, 100);

	Utils::Magnitude totalOverallSize;
	ConvertToMagnitude(m_overallTotalSize, totalOverallSize);

	CString statusText;

	if (lParam == ERROR_SUCCESS)
	{
		DWORD totalFiles = wParam;
		statusText.Format(_T("Copy completed\nTotal files: %d\nTotal amount copied: %lld %s"), 
			totalFiles, m_overallTransferredSize ? totalOverallSize.value : 0, 
			totalOverallSize.unit);
	}
	else
	{
		m_taskBar.SetErrorProgressState();
		_com_error error(lParam);
		statusText.Format(_T("Copy failed with the following error: 0x%08x (%s)\nFile: %s"), lParam, 
			error.ErrorMessage(), m_lastFileAttempted);
	}

	m_status.SetWindowText(statusText);

	ResetTotals();
	m_copyToEdit.EnableWindow(TRUE);
	m_copyFromEdit.EnableWindow(TRUE);
	m_buttonOK.EnableWindow(TRUE);
	m_progressPart.ShowWindow(SW_HIDE);
	m_progressWhole.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CcopyallDlg::OnProgressSetTotal(WPARAM wParam, LPARAM lParam)
{
	LARGE_INTEGER size;
	size.LowPart = lParam;
	size.HighPart = wParam;
	m_overallTotalSize = size.QuadPart;

	m_progressPart.ShowWindow(SW_SHOW);
	m_progressWhole.ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CcopyallDlg::OnCopyError(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 1;
	if (lParam != sizeof(Controller::ProgressData))
		return 1;

	Controller::ProgressData* data = (Controller::ProgressData*) wParam;
	Utils::GetTruncatedMessage(data->filePath, 75, m_lastFileAttempted);
	HeapFree(GetProcessHeap(), 0, data);
	return 0;
}

void CcopyallDlg::OnBnClickedCancel()
{
	m_progressPart.ShowWindow(SW_HIDE);
	m_progressWhole.ShowWindow(SW_HIDE);
	m_taskBar.SetNoProgressState();

	ResetTotals();
	Controller::CancelCopy();
	OnCancel();
}
