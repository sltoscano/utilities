#pragma once

#include "atlbase.h"
#include "atlcom.h"

#include "ShObjIdl.h"

class TaskBar
{
public:
	TaskBar();
	~TaskBar();

	void SetHWND(HWND handle);
	bool SetProgressValue(__int64 completed, __int64 total);
	bool SetNoProgressState();
	bool SetErrorProgressState();

protected:
	ShellObjects::ITaskbarList3 *m_taskBarList;
	HWND m_handle;
};
