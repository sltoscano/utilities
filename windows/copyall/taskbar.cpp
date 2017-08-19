#include "StdAfx.h"

#include "taskbar.h"


TaskBar::TaskBar() : 
	m_taskBarList(NULL),
	m_handle(NULL)
{
	HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_taskBarList));
	if (hr != S_OK)
		m_taskBarList = NULL;
}

TaskBar::~TaskBar()
{
	if (m_taskBarList != NULL)
		m_taskBarList->Release();
}

void TaskBar::SetHWND(HWND handle)
{
	m_handle = handle;
}

bool TaskBar::SetProgressValue(__int64 completed, __int64 total)
{
	if (m_taskBarList == NULL || m_handle == NULL)
		return false;

	wireHWND hwnd = reinterpret_cast<wireHWND>(m_handle);
	return m_taskBarList->SetProgressValue(hwnd, completed, total) == S_OK ? true : false;
}

bool TaskBar::SetNoProgressState()
{
	if (m_taskBarList == NULL || m_handle == NULL)
		return false;

	wireHWND hwnd = reinterpret_cast<wireHWND>(m_handle);
	return m_taskBarList->SetProgressState(hwnd, ShellObjects::TBPF_NOPROGRESS) == S_OK ? true : false;
}

bool TaskBar::SetErrorProgressState()
{
	if (m_taskBarList == NULL || m_handle == NULL)
		return false;

	wireHWND hwnd = reinterpret_cast<wireHWND>(m_handle);
	return m_taskBarList->SetProgressState(hwnd, ShellObjects::TBPF_ERROR) == S_OK ? true : false;
}