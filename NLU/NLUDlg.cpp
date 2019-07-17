
// NLUDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "NLU.h"
#include "NLUDlg.h"
#include "zlib.h"
#include "afxdialogex.h"
#include "json/json.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//全局变量
//HANDLE hCom; //串口
extern HANDLE hCom; //串口
extern CWinThread* pThreadRX;
extern CWinThread* pThreadTX;
extern BOOL	m_bThreadRXrunning;
extern BOOL	m_bThreadTXrunning;

extern BYTE	m_byteRXFrame[BUFFERLENTH];

CString m_strgfilepath;

extern BYTE	m_byteWriteFrame1[BUFFERLENTH];
extern BYTE	m_byteWriteFrame2[BUFFERLENTH];
extern BYTE	m_byteWriteFrame3[BUFFERLENTH];
extern BYTE	m_byteWriteFrame4[BUFFERLENTH];

extern BOOL	m_bSendPackage;

UINT	m_uintBaseAddr;
BOOL	m_bReady;
extern  BYTE	m_byteRXbuffer[BUFFERLENTH];

UINT	m_unReSendAddr;
int		m_nBinFileLenth;
int		m_nBinFileCur;
BOOL	m_bReSend;
BOOL	m_bRecoder;

BOOL	m_bWorking;

BOOL	m_bHighSpeed;
int		m_ncountersecond;

BOOL m_bConnection;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNLUDlg 对话框



CNLUDlg::CNLUDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NLU_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNLUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM_NUM, m_combo_com_num);
	DDX_Control(pDX, IDC_BUTTON_COM, m_bn_com);
	DDX_Control(pDX, IDC_RICHEDIT2_RX, m_richeditctrl_rx);
	DDX_Control(pDX, IDC_EDIT_RX_NUM, m_edit_rx_num);
	DDX_Control(pDX, IDC_RICHEDIT2_TX, m_richeditctrl_tx);
	DDX_Control(pDX, IDC_EDIT_WAKEUP, m_edit_wakeup);
	DDX_Control(pDX, IDC_RICHEDIT2_INPUT, m_richeditctrl_input);
	DDX_Control(pDX, IDC_RICHEDIT2_RX_RESULT, m_richeditctrl_result);
	DDX_Control(pDX, IDC_RICHEDIT2_RX_INTENT, m_richedit_rx_intent);
	DDX_Control(pDX, IDC_RICHEDIT2_RX_VALUE, m_richeditctrl_rx_value);
}

BEGIN_MESSAGE_MAP(CNLUDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_FINDAVACOMPORT, OnReceiveAComPort)
	ON_MESSAGE(WM_COM_RX, OnThreadRXMessage)
	ON_MESSAGE(WM_COM_TX, OnThreadTXMessage)
	ON_BN_CLICKED(IDC_BUTTON_COM, &CNLUDlg::OnBnClickedButtonCom)
	ON_BN_CLICKED(IDC_BUTTON_LOOP_TEST, &CNLUDlg::OnBnClickedButtonLoopTest)
	ON_BN_CLICKED(IDC_BUTTON_ALL_CLEAR, &CNLUDlg::OnBnClickedButtonAllClear)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNLUDlg 消息处理程序

BOOL CNLUDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	VERIFY(font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		GB2312_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		L"微软雅黑"));                 // lpszFacename

	CWnd *cWnd = GetDlgItem(IDC_STATIC_FACE_TITLE);
	cWnd->SetFont(&font);
	cWnd = GetDlgItem(IDC_STATIC_VOICE_TEXT);
	cWnd->SetFont(&font);
	cWnd = GetDlgItem(IDC_STATIC_INTENT);
	cWnd->SetFont(&font);
	cWnd = GetDlgItem(IDC_STATIC_KEY);
	cWnd->SetFont(&font);


	Initialization();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNLUDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNLUDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNLUDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNLUDlg::Initialization()
{
	m_comport.InitPort();
	m_bConnection = FALSE;
	m_bReady = FALSE;
	m_bWorking = FALSE;	
	m_bHighSpeed = FALSE;
	m_ncountersecond = 0;
	m_edit_wakeup.SetFont(&font);
	m_edit_wakeup.SetWindowTextW(_T("未检测到人脸"));

	SetTimer(1, 1000*60, NULL);
}

LRESULT CNLUDlg::OnReceiveAComPort(WPARAM wParam, LPARAM lParam)
{
	static int m_nindex = 0;
	CString m_strport;
	m_strport.Format(_T("COM%d"), lParam);
	m_combo_com_num.InsertString(m_nindex, m_strport);
	m_nindex++;
	m_combo_com_num.SetCurSel(0);
	return 0;
}

void CNLUDlg::OnBnClickedButtonCom()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{

		CString m_strsel;
		m_combo_com_num.GetWindowTextW(m_strsel);
		m_strsel.Delete(0, 3);

		if (m_comport.OpenPort(_ttoi(m_strsel)))
		{
			m_bConnection = TRUE;
			m_bn_com.SetWindowText(_T("关闭串口"));
		}
		else
		{
			m_bConnection = FALSE;
			m_bn_com.SetWindowText(_T("打开串口"));

		}
	}
	else
	{
		//OnSelchangeCombocomportnumber();	//�رյ�ǰ���ں��߳�
		m_bConnection = FALSE;
		m_bn_com.SetWindowText(_T("打开串口"));
		m_comport.CloseComPort();
	}
}

extern unsigned int m_unSendLenth;
void CNLUDlg::OnBnClickedButtonLoopTest()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		return ;
	}
	//获取输入字符串
	CString m_strtemp;
	m_unSendLenth =0;
	m_richeditctrl_input.GetWindowTextW(m_strtemp);

	while (!m_strtemp.IsEmpty())
	{
		if (m_strtemp.Left(1) == " ")
		{
			//直接删除
			m_strtemp.Delete(0, 1);
		}
		else
		{
			//取左边2个字符，转成16进制数后删除
			m_byteWriteFrame1[m_unSendLenth++]= wcstol(m_strtemp.Left(2), NULL, 16);
			m_strtemp.Delete(0, 2);
		}
	}

	m_bSendPackage = TRUE;
}
/*
//UTF8转ANSI
void UTF8toANSI(CString &strUTF8)
{
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;

	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strUTF8 = szBuffer;
	//清理内存
	delete[]szBuffer;
	delete[]wszBuffer;
}
//ANSI转UTF8
void ANSItoUTF8(CString &strAnsi)
{
	//获取转换为宽字节后需要的缓冲区大小，创建宽字节缓冲区，936为简体中文GB2312代码页
	UINT nLen = MultiByteToWideChar(936, NULL, (LPCCH)strAnsi, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(936, NULL, strAnsi, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	//获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strAnsi = szBuffer;
	//内存清理
	delete[]wszBuffer;
	delete[]szBuffer;
}
*/
/*
int UniToUTF8(CString strUnicode, char *szUtf8)
{
	//MessageBox(strUnicode);  
	int ilen = WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strUnicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8Temp = new char[ilen + 1];
	memset(szUtf8Temp, 0, ilen + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strUnicode, -1, szUtf8Temp, ilen, NULL, NULL);
	//size_t a = strlen(szUtf8Temp);  
	
	//printf(szUtf8, "%s", szUtf8Temp);//   
	delete[] szUtf8Temp;
	return ilen;
}
*/
int UniToUTF8(LPCTSTR strUnicode, char *szUtf8)
{
	//MessageBox(strUnicode);  
	int ilen = WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strUnicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8Temp = new char[ilen + 1];
	memset(szUtf8Temp, 0, ilen + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strUnicode, -1, szUtf8Temp, ilen, NULL, NULL);
	//size_t a = strlen(szUtf8Temp);  

	//printf(szUtf8, "%s", szUtf8Temp);//   
	delete[] szUtf8Temp;
	return ilen;
}

LRESULT CNLUDlg::OnThreadRXMessage(WPARAM wParam, LPARAM lParam)
{
	//收到新的数据包重置定时器
	KillTimer(1);
	SetTimer(1, 1000 * 60, NULL);

	static int m_snc = 0;
	m_snc += wParam;
	CString m_temp,m_strshow;


	/*
	m_temp = "{\"event\":\"OnWakeup\"}";

	const char *istream = "{\"event\":\"OnWakeup\"}";
	uLong srcLen = strlen(istream) + 1;      // +1 for the trailing `\0`
	uLong destLen = compressBound(srcLen); // this is how you should estimate size 
										   // needed for the buffer
	unsigned char* ostream = (unsigned char*)malloc(destLen);
	int res = compress(ostream, &destLen, (const unsigned char *)istream, srcLen);
	for (int i = 0; i < destLen; i++)
	{
		m_temp.Format(_T("%02X "), ostream[i]);
		m_strshow += m_temp;
	}

	// destLen is now the size of actuall buffer needed for compression
	// you don't want to uncompress whole buffer later, just the used part
	if (res == Z_BUF_ERROR) {
		printf("Buffer was too small!\n");
		return 1;
	}
	if (res == Z_MEM_ERROR) {
		printf("Not enough memory for compression!\n");
		return 2;
	}

	unsigned char *src_stream = ostream;
	char* dest_stream = (char *)malloc(1024);
	uLong undestLen; //destLen is the actual size of the compressed buffer
	int des = uncompress((unsigned char *)dest_stream, &undestLen, src_stream, destLen);
	printf("%s\n", dest_stream);
	
	*/
	/*
	uLong srcLen = wParam -8;
	uLong destLen;
	unsigned char *src_stream = &m_byteRXFrame[7];
	unsigned char* dest_stream = (unsigned char *)malloc(1024);
	//destLen is the actual size of the compressed buffer
	int des = uncompress(dest_stream, &destLen, src_stream, srcLen);

	if (des == Z_OK)
	{

	}
	if (des == Z_BUF_ERROR) {
		printf("Buffer was too small!\n");
		return 1;
	}
	if (des == Z_MEM_ERROR) {
		printf("Not enough memory for compression!\n");
		return 2;
	}
	
	*/

	//数据解析

	char sBuf[BUFFERLENTH] = { 0 };

	//std::string m_stroutUTF8 = "";
	//strcpy(sBuf, "test");
	memcpy(sBuf, &m_byteRXFrame[7], wParam-8);
	int nBufSize = strlen(sBuf);

	// VC++默认使用ANSI，故取第一个参数为CP_ACP
	DWORD dBufSize = MultiByteToWideChar(CP_UTF8, 0, sBuf, nBufSize, NULL, 0);
	printf("需要wchar_t %u 个\n", dBufSize);
	wchar_t* dBuf = new wchar_t[dBufSize];
	wmemset(dBuf, 0, dBufSize);
	int nRet = MultiByteToWideChar(CP_UTF8, 0, sBuf, nBufSize, dBuf, dBufSize);
	dBuf[dBufSize] = '\0';
	//m_stroutUTF8 = (std::string)dBuf;
	if (nRet <= 0)
	{
		printf("转换失败\n");
		DWORD dwErr = GetLastError();
		switch (dwErr)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			printf("error insufficient buffer\n");
			break;
		case ERROR_INVALID_FLAGS:
			printf("error invalid flags\n");
			break;
		case ERROR_INVALID_PARAMETER:
			printf("error invalid parameter\n");
			break;
		case ERROR_NO_UNICODE_TRANSLATION:
			printf("error no unicode translation\n");
			break;
		}
	}
	else
	{
		//转换成功 开始解析json串
		//const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";

		m_temp.Format(_T("%s"), dBuf);
		//wchar_t* p = szAllData.GetBuffer(szAllData.GetLength());
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, dBuf, -1, NULL, 0, NULL, FALSE);
		char *psText;
		psText = new char[dwNum];
		//WideCharToMultiByte的再次运用，进行转换
		WideCharToMultiByte(CP_OEMCP, NULL, dBuf, -1, psText, dwNum, NULL, FALSE);
		//std::string赋值
		std::string m_str = psText; //大功告成
		//psText的清除
		delete[]psText;
		psText = NULL;
		CString m_strResultShow;
		Json::Reader reader;
		Json::Value root;
		int m_size;
		if (reader.parse(m_str, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
		{
			std::string m_strevent = root["event"].asString();  // 访问节点
			//int m_nstatus = root["status"].asInt();		//status = 0 ,表示识别正常，=1 没有识别到结果
			//std::string m_strparam = root["param"].asString();		// 访问节点

			CString m_strTime;
			SYSTEMTIME m_st;
			GetLocalTime(&m_st);
			m_strTime.Format(_T("时间戳:%2d:%2d:%2d.%3d ::"), m_st.wHour, m_st.wMinute, m_st.wSecond, m_st.wMilliseconds);

			//int code = root["code"].asInt();					// 访问节点
			//判断event类型
			if (m_strevent == "OnWakeup")
			{
				m_edit_wakeup.SetFont(&font);
				m_edit_wakeup.SetWindowTextW(_T("已检测到人脸"));
				/*
				m_strResultShow = _T("检测到人脸\r\n");
				m_richeditctrl_result.SetFont(&font);
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
				*/
			}
			if (m_strevent == "OnSleep")
			{
				m_edit_wakeup.SetFont(&font);
				m_edit_wakeup.SetWindowTextW(_T("未检测到人脸"));
				/*
				m_strResultShow = _T("未检测到人脸\r\n");
				m_richeditctrl_result.SetFont(&font);
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
				*/
			}
			if (m_strevent == "ASR")
			{
				//m_edit_wakeup.SetWindowTextW(_T("已睡眠"));
				
				std::string m_strtext = root["param"]["text"].asString();  // 访问节点
				int m_nfinish = root["param"]["finish"].asInt();
				//不管是否识别完成全部显示
				m_strResultShow = m_strtext.c_str();
				m_strResultShow += _T("\r\n");
				m_strResultShow.Insert(0, m_strTime);
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);

			}
			if (m_strevent == "NLU")
			{
				//状态错误
				int m_nstatus = root["status"].asInt();
				if (m_nstatus == 1)	return 0;
				//获取intent,根据intent获取内容
				std::string m_strIntent = root["param"]["intent"].asString();		// 访问节点
				std::string m_strKey;
				/*
				if (m_strIntent == "chat&chat")
				{
					m_strIntent = "聊天";
					m_strKey = root["param"]["answerText"].asString();		// 访问节点

				}
				if (m_strIntent == "guide&guide")
				{
					m_strIntent = "向导";
					std::string m_strslots = root["param"]["slots"].asString();		// 访问节点
					if (reader.parse(m_strslots, root))
					{
						Json::Value arrayObj = root["destination"];
						for (unsigned int i = 0; i < arrayObj.size(); i++)
						{
							if (!arrayObj[i].isMember("value"))
								continue;
							m_strKey = arrayObj[i]["value"].asString();
							break;
						}
					}
				}
				*/
				if (m_strIntent == "tell_me_why&common")
				{
					m_strIntent = "智能语音咨询";
					m_strKey = root["param"]["answerText"].asString();		// 访问节点
				}

				if (m_strIntent == "metro_search&transfer_route")
				{
					m_strIntent = "购票";
					std::string m_strslots = root["param"]["slots"].asString();		// 访问节点
					if (reader.parse(m_strslots, root))
					{
						Json::Value arrayObj = root["terminal_station"];
						for (unsigned int i = 0; i < arrayObj.size(); i++)
						{
							if (!arrayObj[i].isMember("value"))
								continue;
							m_strKey = arrayObj[i]["value"].asString();
							break;
						}
					}
				}

				//显示意图
				m_strResultShow = m_strIntent.c_str();
				m_strResultShow += _T("\r\n");
				//m_richedit_rx_intent.SetFont(&font);
				m_strResultShow.Insert(0, m_strTime);
				m_richedit_rx_intent.SetSel(-1, -1);
				m_richedit_rx_intent.ReplaceSel(m_strResultShow, 0);
				m_richedit_rx_intent.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);

				//显示关键值
				m_strResultShow = m_strKey.c_str();
				m_strResultShow += _T("\r\n");
				//m_richeditctrl_rx_value.SetFont(&font);
				m_strResultShow.Insert(0, m_strTime);
				m_richeditctrl_rx_value.SetSel(-1, -1);
				m_richeditctrl_rx_value.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_rx_value.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);

			}
			/*
			if (m_strevent == "Input")
			{
				//m_edit_wakeup.SetWindowTextW(_T("已睡眠"));
				m_strResultShow = _T("\r\n");
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
			}
			if (m_strevent == "OnInput")
			{
				//m_edit_wakeup.SetWindowTextW(_T("已睡眠"));
				m_strResultShow = _T("\r\n");
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
			}
			if (m_strevent == "Query")
			{
				//m_edit_wakeup.SetWindowTextW(_T("已睡眠"));
				m_strResultShow = _T("\r\n");
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
			}
			if (m_strevent == "OnQueryResult")
			{
				//m_edit_wakeup.SetWindowTextW(_T("已睡眠"));
				m_strResultShow = _T("\r\n");
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
			}
			if (m_strevent == "Report")
			{
				//m_edit_wakeup.SetWindowTextW(_T("已睡眠"));
				m_strResultShow = _T("\r\n");
				m_richeditctrl_result.SetSel(-1, -1);
				m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
				m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
			}
			*/
		}
		m_strshow += m_temp;
		m_richeditctrl_rx.SetSel(-1, -1);
		m_richeditctrl_rx.ReplaceSel(m_strshow, 0);
		m_richeditctrl_rx.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);

	}




	// 显示数据
	/*
	for (int i = 0; i < wParam; i++)
	{
		m_temp.Format(_T("%02X"), m_byteRXFrame[i]);
		m_strshow += m_temp;
	}
	*/


	return 0;









	//m_richeditctrl_rx.SetWindowTextW(m_strshow);
	//判断缓冲区内是否够一帧数据
		/*
	for (int m_nc = QueueLenth(q); m_nc >= 24; m_nc--)
	{
		//出队 检查是否收到0xfc
		if (DeQueue(q, m_byteFrame[0]) != 0)
		{
			AfxMessageBox(_T("读缓冲区失败"));
		}

		if (m_byteFrame[0] == 0xfc)
		{
			m_strshow = "FC";
			for (int j = 1; j < 24; j++)
			{
				DeQueue(q, m_byteFrame[j]);
				m_temp.Format("%02X", m_byteFrame[j]);
				m_strshow += m_temp;
			}
			//命令解析
			m_strshow += "\r\n";
			m_nc -= 23;
			//保存接收数据
			WriteRecoder(m_strshow);
			m_CRX.ReplaceSel(m_strshow);
			m_CRX.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
			CString m_str;
			//命令处理
			if ((m_byteFrame[0] == 0xfc) && (m_byteFrame[0x01] == 0x0c) && (m_byteFrame[0x05] == 0x70))
			{
				//准备
				if ((m_byteFrame[0x06] == 0xee) && (m_byteFrame[0x07] == 0xff))
				{
					//准备完成开始发送数据
					KillTimer(1);
					//开始计时
					SetTimer(2, 1000, NULL);
					m_ncountersecond = 0;
					m_strcountersecond.Format("%d 秒", m_ncountersecond);
					UpdateData(FALSE);
					Sleep(5);
					pSendThread = AfxBeginThread(ThreadSendBin, (LPVOID)1);
					m_str = "准备!";
					m_str.Insert(0, GetSystemTime());
					m_str += "\r\n";
					m_Cricheditshowmsg.ReplaceSel(m_str);
					m_Cricheditshowmsg.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
					//SendMSGtoSendThread();
				}
				//校验成功
				if ((m_byteFrame[0x06] == 0xdd) && (m_byteFrame[0x07] == 0xdd))
				{
					m_str = "数据校验成功!";
					m_str += "\r\n";
					m_str.Insert(0, GetSystemTime());
					m_Cricheditshowmsg.ReplaceSel(m_str);
					m_Cricheditshowmsg.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);


				}
				//校验失败
				if ((m_byteFrame[0x06] == 0x77) && (m_byteFrame[0x07] == 0x77))
				{
					m_str = "数据校验失败!";
					m_str += "\r\n";
					m_str.Insert(0, GetSystemTime());
					m_Cricheditshowmsg.ReplaceSel(m_str);
					m_Cricheditshowmsg.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
				}
				//数据地址超硬件出地址范围 溢出
				if ((m_byteFrame[0x06] == 0xbb) && (m_byteFrame[0x07] == 0xbb)&(m_byteFrame[0x08] == 0xbb) && (m_byteFrame[0x09] == 0xbb))
				{
					m_str = "数据溢出!";
					m_str += "\r\n";
					m_str.Insert(0, GetSystemTime());
					m_Cricheditshowmsg.ReplaceSel(m_str);
					m_Cricheditshowmsg.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
				}
				//重新发送
				if ((m_byteFrame[0x06] == 0xee) && (m_byteFrame[0x07] == 0xee)&(m_byteFrame[0x08] == 0xee) && (m_byteFrame[0x09] == 0xee))
				{
					//给发送线程发送消息，并告知重发地址
					SendMSGtoSendThread();
					m_str = "重发数据!";
					m_str += "\r\n";
					m_str.Insert(0, GetSystemTime());
					m_Cricheditshowmsg.ReplaceSel(m_str);
					m_Cricheditshowmsg.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
					CString m_strresend;
					m_str.Format("%02X", m_byteFrame[0x0a]);
					m_strresend = m_str;
					m_str.Format("%02X", m_byteFrame[0x0b]);
					m_strresend += m_str;
					m_str.Format("%02X", m_byteFrame[0x0c]);
					m_strresend += m_str;
					m_str.Format("%02X", m_byteFrame[0x0d]);
					m_strresend += m_str;
					m_strresend += "\r\n";
					m_strresend.Insert(0, "重发地址:");
					m_Cricheditshowmsg.ReplaceSel(m_strresend);
					m_Cricheditshowmsg.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
				}
			}

		}

	if (m_strshow.GetLength() > 1200)
	{
		m_strshow.Empty();
	}

	//m_CRX.SetWindowText(m_strshow);
	//数据总长度和有效开始位
	//m_temp.Format("%d", wParam);
	//m_CRXLENTH.SetWindowText(m_temp);
	//m_temp.Format("%d",lParam);
	//m_CRXVALUE.SetWindowText(m_temp);
	*/
	return 0;
}


LRESULT CNLUDlg::OnThreadTXMessage(WPARAM wParam, LPARAM lParam)
{
	//显示发送信息
	static int m_nTX = 0;

	CString m_str;
	CString m_strf1;
	for (int i = 0; i < wParam; i++)
	{
		m_str.Format(_T("%02X"), m_byteWriteFrame1[i]);
		m_strf1 += m_str;

	}
	m_strf1 += "\r\n";


	m_str = m_strf1;
	//文件写入记录
	m_richeditctrl_tx.SetSel(-1, -1);
	m_richeditctrl_tx.ReplaceSel(m_str, 0);

	return 0;
}

void CNLUDlg::OnBnClickedButtonAllClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_richeditctrl_rx.SetWindowTextW(_T(""));
	m_richeditctrl_result.SetWindowTextW(_T(""));
	m_richeditctrl_rx_value.SetWindowTextW(_T(""));
	m_richedit_rx_intent.SetWindowTextW(_T(""));

}


void CNLUDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
		case 1:
		{
				  OnBnClickedButtonAllClear();
				  break;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
