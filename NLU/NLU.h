
// NLU.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CNLUApp:
// 有关此类的实现，请参阅 NLU.cpp
//
#define BUFFERLENTH	0x8000			//缓冲区大小，数据包过长可适当增大，缓冲区过大时可采用动态内存分派

#define WM_FINDAVACOMPORT	(WM_USER+1)
#define WM_COM_RX			(WM_USER+2)
#define WM_COM_TX			(WM_USER+102) 
#define WM_UDISPLAY_MSG		(WM_USER+103)

class CNLUApp : public CWinApp
{
public:
	CNLUApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CNLUApp theApp;
