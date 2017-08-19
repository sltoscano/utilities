#include "StdAfx.h"

#include "copyall.h"
#include "copyallDlg.h"
#include "controller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CcopyallDlg::CcopyallDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcopyallDlg::IDD, pParent)
{
	ResetTotals();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcopyallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COPY_FROM, m_copyFromEdit);
	DDX_Control(pDX, IDC_COPY_TO, m_copyToEdit);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDC_PROGRESS_PART, m_progressPart);
	DDX_Control(pDX, IDC_PROGRESS_WHOLE, m_progressWhole);
}

BEGIN_MESSAGE_MAP(CcopyallDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CcopyallDlg::OnBnClickedOk)
	ON_MESSAGE(UM_PROGRESS_UPDATE, &CcopyallDlg::OnProgressUpdate)
	ON_MESSAGE(UM_PROGRESS_COMPLETED, &CcopyallDlg::OnProgressCompleted)
	ON_MESSAGE(UM_PROGRESS_SETTOTAL, &CcopyallDlg::OnProgressSetTotal)
	ON_MESSAGE(UM_COPY_ERROR, &CcopyallDlg::OnCopyError)
	ON_BN_CLICKED(IDCANCEL, &CcopyallDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CcopyallDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_progressPart.ShowWindow(SW_HIDE);
	m_progressWhole.ShowWindow(SW_HIDE);
	m_progressPart.SetRange(0, 100);
	m_progressWhole.SetRange(0, 100);

	m_taskBar.SetHWND(GetSafeHwnd());
	return TRUE;
}

void CcopyallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CcopyallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
