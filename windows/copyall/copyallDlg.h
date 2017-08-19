#pragma once

#include "afxwin.h"
#include "afxcmn.h"

#include "taskbar.h"

class CcopyallDlg : public CDialog
{
public:
	CcopyallDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_COPYALL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnProgressUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressCompleted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressSetTotal(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCopyError(WPARAM wParam, LPARAM lParam);
	TaskBar m_taskBar;
	CProgressCtrl m_progressPart;
	CProgressCtrl m_progressWhole;
	CButton m_buttonOK;
	CEdit m_copyFromEdit;
	CEdit m_copyToEdit;
	CStatic m_status;
	CString m_lastFileAttempted;
	__int64 m_overallTotalSize;
	__int64 m_overallTransferredSize;
	__int64 m_previousTransferredSize;
	DWORD m_copyStartTime;

protected:
	void ResetTotals()
	{
		m_overallTotalSize = 0;
		m_overallTransferredSize = 0;
		m_previousTransferredSize = 0;
	}
};
