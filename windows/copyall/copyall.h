#pragma once

#include "resource.h"


class CcopyallApp : public CWinApp
{
public:
	CcopyallApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CcopyallApp theApp;
