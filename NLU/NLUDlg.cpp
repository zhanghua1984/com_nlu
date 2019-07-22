
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
extern BOOL m_bDealFinish;
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
unsigned short m_nsMsgID;
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
	DDX_Control(pDX, IDC_CHECK_HEX, m_button_check);
	DDX_Control(pDX, IDC_RICHEDIT2_INPUTJSON, m_richedit_inputjson);
	DDX_Control(pDX, IDC_EDIT_JSON_SEND_TIMER, m_edit_json_send_time);
	DDX_Control(pDX, IDC_CHECK_JSON_SEND_TIMER, m_button_set_timer);
	DDX_Control(pDX, IDC_CHECK_CLEAR_ALL, m_buttonTimeClearAll);
	DDX_Control(pDX, IDC_EDIT_CLEAR_ALL_TIMER, m_edit_time_clear);
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
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CNLUDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_CHECK_HEX, &CNLUDlg::OnBnClickedCheckHex)
	ON_BN_CLICKED(IDC_BUTTON_JSON, &CNLUDlg::OnBnClickedButtonJson)
	ON_BN_CLICKED(IDC_CHECK_JSON_SEND_TIMER, &CNLUDlg::OnBnClickedCheckJsonSendTimer)
	ON_BN_CLICKED(IDC_CHECK_CLEAR_ALL, &CNLUDlg::OnBnClickedCheckClearAll)
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
	/*
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
	*/
	//ShowWindow(SW_SHOWMAXIMIZED);
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
	//m_edit_wakeup.SetFont(&font);
	//m_edit_wakeup.SetWindowTextA(("未检测到人脸"));
	m_edit_json_send_time.SetWindowTextA(("10000"));
	m_edit_time_clear.SetWindowTextA("60000");
	m_buttonTimeClearAll.SetCheck(1);
	SetTimer(1, 1000 * 60, NULL);
	m_nsMsgID = 0;
}
/* Compress gzip data */
/* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
int  gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata)
{
	z_stream c_stream;
	int  err = 0;

	if (data && ndata > 0) {
		c_stream.zalloc = NULL;
		c_stream.zfree = NULL;
		c_stream.opaque = NULL;
		//只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer  
		if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
			MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)  return  -1;
		c_stream.next_in = data;
		c_stream.avail_in = ndata;
		c_stream.next_out = zdata;
		c_stream.avail_out = *nzdata;
		while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
			if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK)  return  -1;
		}
		if (c_stream.avail_in != 0)  return  c_stream.avail_in;
		for (;;) {
			if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END)  break;
			if (err != Z_OK)  return  -1;
		}
		if (deflateEnd(&c_stream) != Z_OK)  return  -1;
		*nzdata = c_stream.total_out;
		return  0;
	}
	return  -1;
}

/* Uncompress gzip data */
/* zdata 数据 nzdata 原数据长度 data 解压后数据 ndata 解压后长度 */
int  gzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
	int  err = 0;
	z_stream d_stream = { 0 };  /* decompression stream */
	static   char  dummy_head[2] = {
		0x8 + 0x7 * 0x10,
		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
	};
	d_stream.zalloc = NULL;
	d_stream.zfree = NULL;
	d_stream.opaque = NULL;
	d_stream.next_in = zdata;
	d_stream.avail_in = 0;
	d_stream.next_out = data;
	//只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本  
	if (inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK)  return  -1;
	//if(inflateInit2(&d_stream, 47) != Z_OK) return -1;  
	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
		d_stream.avail_in = d_stream.avail_out = 1;  /* force small buffers */
		if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END)  break;
		if (err != Z_OK) {
			if (err == Z_DATA_ERROR) {
				d_stream.next_in = (Bytef*)dummy_head;
				d_stream.avail_in = sizeof (dummy_head);
				if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) {
					return  -1;
				}
			}
			else   return  -1;
		}
	}
	if (inflateEnd(&d_stream) != Z_OK)  return  -1;
	*ndata = d_stream.total_out;
	return  0;
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
		m_combo_com_num.GetWindowTextA(m_strsel);
		m_strsel.Delete(0, 3);

		if (m_comport.OpenPort(_ttoi(m_strsel)))
		{
			m_bConnection = TRUE;
			m_bn_com.SetWindowText(("关闭串口"));
		}
		else
		{
			m_bConnection = FALSE;
			m_bn_com.SetWindowText(("打开串口"));

		}
	}
	else
	{
		//OnSelchangeCombocomportnumber();	//�رյ�ǰ���ں��߳�
		m_bConnection = FALSE;
		m_bn_com.SetWindowText(("打开串口"));
		m_comport.CloseComPort();
	}
}

extern unsigned int m_unSendLenth;
void CNLUDlg::OnBnClickedButtonLoopTest()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(("串口未打开"));
		return;
	}
	//获取输入字符串
	CString m_strtemp;
	m_unSendLenth = 0;
	m_richeditctrl_input.GetWindowTextA(m_strtemp);

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
			m_byteWriteFrame1[m_unSendLenth++] = strtoul(m_strtemp.Left(2), NULL, 16);
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
/*
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
*/

LRESULT CNLUDlg::OnThreadRXMessage(WPARAM wParam, LPARAM lParam)
{
	//收到新的数据包重置定时器
	KillTimer(1);
	SetTimer(1, 1000 * 60, NULL);

	static int m_snc = 0;
	m_snc += wParam;
	
	CString m_temp, m_strshow;
	m_temp.Format("%d", m_snc);
	m_edit_rx_num.SetWindowTextA(m_temp);
	//数据解析

	unsigned char sBuf[BUFFERLENTH] = { 0 };

	//std::string m_stroutUTF8 = "";
	//strcpy(sBuf, "test");
	memcpy(sBuf, &m_byteRXFrame[7], wParam - 8);
	int nBufSize = wParam - 8;
	//数据解压缩

	BYTE* out_stream = (BYTE *)malloc(BUFFERLENTH);
	memset(out_stream, 0, BUFFERLENTH);
	uLong m_lLenth;
	int ret = gzdecompress(sBuf, nBufSize, out_stream, &m_lLenth);
	if (ret == 0)	//解压成功
	{
		// VC++默认使用ANSI，故取第一个参数为CP_ACP
		DWORD dBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)out_stream, m_lLenth, NULL, 0);
		wchar_t* dBuf = new wchar_t[dBufSize];
		wmemset(dBuf, 0, dBufSize);
		int nRet = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)out_stream, m_lLenth, dBuf, dBufSize);
		dBuf[dBufSize] = '\0';

		//m_stroutUTF8 = (std::string)dBuf;
		if (nRet < 0)
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
			std::string m_str = (char *)psText; //大功告成
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
					//m_edit_wakeup.SetFont(&font);
					//m_edit_wakeup.SetWindowTextA(("已检测到人脸"));
					m_strResultShow = ("[人脸唤醒]\r\n");
					//m_richeditctrl_result.SetFont(&font);
					m_strResultShow.Insert(0, m_strTime);
					m_richeditctrl_result.SetSel(-1, -1);
					m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
					m_richeditctrl_result.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
				}
				if (m_strevent == "OnSleep")
				{
					//m_edit_wakeup.SetFont(&font);
					//m_edit_wakeup.SetWindowTextA(("未检测到人脸"));
					m_strResultShow = ("[自动休眠]\r\n");
					//m_richeditctrl_result.SetFont(&font);
					m_strResultShow.Insert(0, m_strTime);
					m_richeditctrl_result.SetSel(-1, -1);
					m_richeditctrl_result.ReplaceSel(m_strResultShow, 0);
					m_richeditctrl_result.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
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
					//m_richeditctrl_result.PostMessageW(WM_VSCROLL, SB_BOTTOM, 0);
					m_richeditctrl_result.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
				}
				if (m_strevent == "NLU")
				{
					//状态错误
					int m_nstatus = root["status"].asInt();
					if (m_nstatus == 1)	return 0;
					//获取intent,根据intent获取内容
					std::string m_strIntent = root["intent"].asString();		// 访问节点
					std::string m_strKey = root["param"].toStyledString();

					//Json::Value Obj = root["param"].toStyledString();

					//CString m_strTemp;
					//int m_nCount = root["param"]["count"].asInt();
					//int m_nPrice = root["param"]["price"].asInt();

					//bool isObject = root["param"].isObject();	//true
					//bool isArray = root["param"].isArray();		//false
					
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
					if (m_strIntent == "metro_search")
					{
						m_strIntent = "购票";
						const Json::Value m_StationArray = root["param"]["station"];
						std::string m_strCode;
						int m_nDistance;
						std::string m_strTarget;
						for (unsigned int i = 0; i < m_StationArray.size(); i++)
						{
							m_strCode = m_StationArray[i]["code"].asString();		// 访问节点
							m_strTemp += "code:";
							m_strTemp += m_strCode.c_str();
							m_strTemp += "\n";
							m_nDistance = m_StationArray[i]["distance"].asInt();
							CString temp;
							temp.Format("distance:%d \n", m_nDistance);
							m_strTemp += temp;
							m_strTarget = m_StationArray[i]["poi"].asString();
							m_strTemp += "target:";
							m_strTemp += m_strTarget.c_str();
							m_strTemp += "\n";
							
						}
					}
					*/
					//显示意图
					m_strResultShow = m_strIntent.c_str();
					m_strResultShow += ("\r\n");
					//m_richedit_rx_intent.SetFont(&font);
					m_strResultShow.Insert(0, m_strTime);
					m_richedit_rx_intent.SetSel(-1, -1);
					m_richedit_rx_intent.ReplaceSel(m_strResultShow, 0);
					m_richedit_rx_intent.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
					
					//显示关键值
					m_strResultShow = m_strKey.c_str();
					m_strResultShow += ("\r\n");
					//m_richeditctrl_rx_value.SetFont(&font);
					//m_strResultShow += m_strTemp;
					m_strResultShow.Insert(0, m_strTime);
					m_richeditctrl_rx_value.SetSel(-1, -1);
					m_richeditctrl_rx_value.ReplaceSel(m_strResultShow, 0);
					m_richeditctrl_rx_value.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
					
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
			/*
			m_strshow += m_temp;
			m_richeditctrl_rx.SetSel(-1, -1);
			m_richeditctrl_rx.ReplaceSel(m_strshow, 0);
			m_richeditctrl_rx.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
			*/
		}
		}

		m_bDealFinish = true;
	free(out_stream);


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
	m_richeditctrl_rx.SetWindowTextA("");
	m_richeditctrl_result.SetWindowTextA("");
	m_richeditctrl_rx_value.SetWindowTextA("");
	m_richedit_rx_intent.SetWindowTextA("");

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
		case 2:
		{
				  OnBnClickedButtonJson();
				  break;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CNLUDlg::OnBnClickedButtonTest()
{
	// TODO:  在此添加控件通知处理程序代码
	CString m_strtemp;
	m_richeditctrl_input.GetWindowTextA(m_strtemp);

	const char *istream = m_strtemp;
	uLong srcLen = strlen(istream) + 1;      // +1 for the trailing `\0`
	uLong destLen = compressBound(srcLen); // this is how you should estimate size 
	// needed for the buffer
	unsigned char* ostream = (unsigned char*)malloc(destLen);
	int res = compress(ostream, &destLen, (const unsigned char *)istream, srcLen);
	// destLen is now the size of actuall buffer needed for compression
	// you don't want to uncompress whole buffer later, just the used part
	if (res == Z_BUF_ERROR){
		printf("Buffer was too small!\n");
		return ;
	}
	if (res == Z_MEM_ERROR){
		printf("Not enough memory for compression!\n");
		return ;
	}

	unsigned char *i2stream = ostream;
	char* o2stream = (char *)malloc(BUFFERLENTH);
	uLong destLen2 = destLen; //destLen is the actual size of the compressed buffer
	int des = uncompress((unsigned char *)o2stream, &srcLen, i2stream, destLen2);
	printf("%s\n", o2stream);



}


void CNLUDlg::OnBnClickedCheckHex()
{
	// TODO:  在此添加控件通知处理程序代码
	m_button_check.GetCheck();
}

/*

{"event":"Init", "param" : {"ip":"10.11.11.20", "mask" : "255.255.255.0", "gateway" : "10.11.11.1", "dns" : "8.8.8.8", "deviceID" : "1010XXXX", "aliServerIP" : "112.222.113.1"}}


*/
void CNLUDlg::OnBnClickedButtonJson()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		KillTimer(2);
		return;
	}
	USES_CONVERSION;
	//获取输入字符串
	CString m_strtemp;
	m_unSendLenth = 0;
	//m_richedit_inputjson.GetWindowTextW(m_strtemp);
	m_richedit_inputjson.GetWindowTextA(m_strtemp);
	if (!m_strtemp.IsEmpty())
	{
		//std::string m_str = LPCSTR(m_strtemp);

		// VC++默认使用ANSI，故取第一个参数为CP_ACP
		DWORD dBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)m_strtemp, m_strtemp.GetLength(), NULL, 0);
		wchar_t* dBuf = new wchar_t[dBufSize];
		wmemset(dBuf, 0, dBufSize);
		int nRet = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)m_strtemp, m_strtemp.GetLength(), dBuf, dBufSize);
		dBuf[dBufSize] = '\0';

		//m_stroutUTF8 = (std::string)dBuf;
		if (nRet < 0)
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
			//压缩
			/* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
			Bytef m_buffer[BUFFERLENTH];
			memset(m_buffer, 0, BUFFERLENTH);
			uLong m_nLenth;
			int m_ret = gzcompress((Bytef *)dBuf, dBufSize + 1, m_buffer, &m_nLenth);
			if (m_ret < 0)	
			{
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
				int i = 0;

				m_byteWriteFrame1[i++] = 0xa5;
				m_byteWriteFrame1[i++] = 0x01;
				m_byteWriteFrame1[i++] = 0x04;
				m_byteWriteFrame1[i++] = m_nLenth & 0x000000ff;
				m_byteWriteFrame1[i++] = (m_nLenth >>8) & 0x000000ff;
				m_byteWriteFrame1[i++] = m_nsMsgID & 0x00ff;
				m_byteWriteFrame1[i++] = (m_nsMsgID >>8) & 0x00ff;
				memcpy(&m_byteWriteFrame1[i], m_buffer, m_nLenth);
				i += m_nLenth;
				BYTE checkcode = 0;
				for (int j = 0; j < m_nLenth + 7; j++)
				{
					checkcode += m_byteWriteFrame1[j];
				}

				checkcode = ~checkcode + 1;
				checkcode = checkcode ^ 0x01;
				m_byteWriteFrame1[i++] = checkcode;
				m_nsMsgID++;
				m_unSendLenth = m_nLenth + 8;
				m_bSendPackage = TRUE;
			}

		}

	}
}



void CNLUDlg::OnBnClickedCheckJsonSendTimer()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		KillTimer(2);
		return;
	}
	if (m_button_set_timer.GetCheck())
	{
		CString m_strtemp;
		m_edit_json_send_time.GetWindowTextA(m_strtemp);
		int m_nTime = strtoul(m_strtemp, NULL, 10);
		SetTimer(2, m_nTime, NULL);
	}
	else
	{
		KillTimer(2);
	}
}


void CNLUDlg::OnBnClickedCheckClearAll()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_buttonTimeClearAll.GetCheck())
	{
		CString m_strtemp;
		m_edit_time_clear.GetWindowTextA(m_strtemp);
		int m_nTime = strtoul(m_strtemp, NULL, 10);
		SetTimer(1, m_nTime, NULL);
	}
	else
	{
		KillTimer(1);
	}
}
