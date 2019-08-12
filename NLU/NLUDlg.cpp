
// NLUDlg.cpp: 实现文件
//
#pragma once 

#include "stdafx.h"
#include "NLU.h"
#include "NLUDlg.h"
#include "zlib.h"
#include "afxdialogex.h"
#include "json/json.h"

//#include <SDL.h>

#pragma comment(lib,"..\\lib\\avutil.lib")
#pragma comment(lib,"..\\lib\\avformat.lib")
#pragma comment(lib,"..\\lib\\avcodec.lib")
#pragma comment(lib,"..\\lib\\swscale.lib")
//#pragma comment(lib,"..\\lib\\x86\\SDL2.lib")
//#pragma comment(lib,"..\\lib\\x86\\SDL2main.lib")
//#pragma comment(lib,"..\\lib\\x86\\SDL2test.lib")



extern "C"
{
#include "SDL.h"
//#include "SDL_thread.h"
//#include "SDL_main.h"
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/fifo.h"
}

/*
#ifdef __cplusplus
extern "C"
{
#pragma comment (lib, "Ws2_32.lib")  
#pragma comment (lib, "avcodec.lib")
#pragma comment (lib, "avdevice.lib")
#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
#pragma comment (lib, "avutil.lib")
#pragma comment (lib, "swresample.lib")
#pragma comment (lib, "swscale.lib")
};
#endif

#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/fifo.h"
*/
/*
#ifdef _cplusplus
extern "C"
{
#endif

	
#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/fifo.h"

//#include "libavcodec/avcodec.h"


#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"

#ifdef _cplusplus
}
#endif
*/


#define HAVE_LIBC 1

#ifdef HAVE_LIBC
/* Useful headers */
/* #undef HAVE_ALLOCA_H */
#define HAVE_SYS_TYPES_H 1
#define HAVE_STDIO_H 1
#define STDC_HEADERS 1
#define HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1
#define HAVE_MALLOC_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_CTYPE_H 1
#define HAVE_MATH_H 1
/* #undef HAVE_ICONV_H */
#define HAVE_SIGNAL_H 1
/* #undef HAVE_ALTIVEC_H */
/* C library functions */
#define HAVE_MALLOC 1
#define HAVE_CALLOC 1
#define HAVE_REALLOC 1
#define HAVE_FREE 1
/* #undef HAVE_ALLOCA */
#ifndef _WIN32 /* Don't use C runtime versions of these on Windows */
#define HAVE_GETENV 1
#define HAVE_PUTENV 1
/* #undef HAVE_UNSETENV */
#endif
#define HAVE_QSORT 1
#define HAVE_ABS 1
/* #undef HAVE_BCOPY */
#define HAVE_MEMSET 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMCMP 1
#define HAVE_STRLEN 1
/* #undef HAVE_STRLCPY */
/* #undef HAVE_STRLCAT */
#define HAVE_STRDUP 1
#define HAVE__STRREV 1
#define HAVE__STRUPR 1
#define HAVE__STRLWR 1
/* #undef HAVE_INDEX */
/* #undef HAVE_RINDEX */
#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1
#define HAVE_ITOA 1
#define HAVE__LTOA 1
/* #undef HAVE__UITOA */
#define HAVE__ULTOA 1
#define HAVE_STRTOL 1
#define HAVE_STRTOUL 1
#define HAVE__I64TOA 1
#define HAVE__UI64TOA 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOULL 1
#define HAVE_STRTOD 1
#define HAVE_ATOI 1
#define HAVE_ATOF 1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP 1
#define HAVE__STRICMP 1
#define HAVE_STRCASECMP 1
#define HAVE__STRNICMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_SSCANF 1
#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1
/* #undef HAVE_ICONV */
/* #undef HAVE_SIGACTION */
/* #undef HAVE_SETJMP */
/* #undef HAVE_NANOSLEEP */
/* #undef HAVE_CLOCK_GETTIME */
/* #undef HAVE_GETPAGESIZE */
/* #undef HAVE_MPROTECT */
#else
/* We may need some replacement for stdarg.h here */
#include <stdarg.h>
#endif /* HAVE_LIBC */

/* Allow disabling of core subsystems */
/* #undef SDL_AUDIO_DISABLED */
/* #undef SDL_CDROM_DISABLED */
/* #undef SDL_CPUINFO_DISABLED */
/* #undef SDL_EVENTS_DISABLED */
/* #undef SDL_FILE_DISABLED */
/* #undef SDL_JOYSTICK_DISABLED */
/* #undef SDL_LOADSO_DISABLED */
/* #undef SDL_THREADS_DISABLED */
/* #undef SDL_TIMERS_DISABLED */
/* #undef SDL_VIDEO_DISABLED */
/* Enable various audio drivers */
/* #undef SDL_AUDIO_DRIVER_ALSA */
/* #undef SDL_AUDIO_DRIVER_ALSA_DYNAMIC */
/* #undef SDL_AUDIO_DRIVER_ARTS */
/* #undef SDL_AUDIO_DRIVER_ARTS_DYNAMIC */
/* #undef SDL_AUDIO_DRIVER_BAUDIO */
/* #undef SDL_AUDIO_DRIVER_BSD */
/* #undef SDL_AUDIO_DRIVER_COREAUDIO */
/* #undef SDL_AUDIO_DRIVER_DART */
/* #undef SDL_AUDIO_DRIVER_DC */
#define SDL_AUDIO_DRIVER_DISK 1
#define SDL_AUDIO_DRIVER_DUMMY 1
/* #undef SDL_AUDIO_DRIVER_DMEDIA */
#define SDL_AUDIO_DRIVER_DSOUND 1
/* #undef SDL_AUDIO_DRIVER_PULSE */
/* #undef SDL_AUDIO_DRIVER_PULSE_DYNAMIC */
/* #undef SDL_AUDIO_DRIVER_ESD */
/* #undef SDL_AUDIO_DRIVER_ESD_DYNAMIC */
/* #undef SDL_AUDIO_DRIVER_MINT */
/* #undef SDL_AUDIO_DRIVER_MMEAUDIO */
/* #undef SDL_AUDIO_DRIVER_NAS */
/* #undef SDL_AUDIO_DRIVER_NAS_DYNAMIC */
/* #undef SDL_AUDIO_DRIVER_OSS */
/* #undef SDL_AUDIO_DRIVER_OSS_SOUNDCARD_H */
/* #undef SDL_AUDIO_DRIVER_PAUD */
/* #undef SDL_AUDIO_DRIVER_QNXNTO */
/* #undef SDL_AUDIO_DRIVER_SNDMGR */
/* #undef SDL_AUDIO_DRIVER_SUNAUDIO */
#define SDL_AUDIO_DRIVER_WAVEOUT 1
/* Enable various cdrom drivers */
/* #undef SDL_CDROM_AIX */
/* #undef SDL_CDROM_BEOS */
/* #undef SDL_CDROM_BSDI */
/* #undef SDL_CDROM_DC */
/* #undef SDL_CDROM_DUMMY */
/* #undef SDL_CDROM_FREEBSD */
/* #undef SDL_CDROM_LINUX */
/* #undef SDL_CDROM_MACOS */
/* #undef SDL_CDROM_MACOSX */
/* #undef SDL_CDROM_MINT */
/* #undef SDL_CDROM_OPENBSD */
/* #undef SDL_CDROM_OS2 */
/* #undef SDL_CDROM_OSF */
/* #undef SDL_CDROM_QNX */
#define SDL_CDROM_WIN32 1
/* Enable various input drivers */
/* #undef SDL_INPUT_LINUXEV */
/* #undef SDL_INPUT_TSLIB */
/* #undef SDL_JOYSTICK_BEOS */
/* #undef SDL_JOYSTICK_DC */
/* #undef SDL_JOYSTICK_DUMMY */
/* #undef SDL_JOYSTICK_IOKIT */
/* #undef SDL_JOYSTICK_LINUX */
/* #undef SDL_JOYSTICK_MACOS */
/* #undef SDL_JOYSTICK_MINT */
/* #undef SDL_JOYSTICK_OS2 */
/* #undef SDL_JOYSTICK_RISCOS */
#define SDL_JOYSTICK_WINMM 1
/* #undef SDL_JOYSTICK_USBHID */
/* #undef SDL_JOYSTICK_USBHID_MACHINE_JOYSTICK_H */
/* Enable various shared object loading systems */
/* #undef SDL_LOADSO_BEOS */
/* #undef SDL_LOADSO_DLCOMPAT */
/* #undef SDL_LOADSO_DLOPEN */
/* #undef SDL_LOADSO_DUMMY */
/* #undef SDL_LOADSO_LDG */
/* #undef SDL_LOADSO_MACOS */
/* #undef SDL_LOADSO_OS2 */
#define SDL_LOADSO_WIN32 1
/* Enable various threading systems */
/* #undef SDL_THREAD_BEOS */
/* #undef SDL_THREAD_DC */
/* #undef SDL_THREAD_OS2 */
/* #undef SDL_THREAD_PTH */
/* #undef SDL_THREAD_PTHREAD */
/* #undef SDL_THREAD_PTHREAD_RECURSIVE_MUTEX */
/* #undef SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP */
/* #undef SDL_THREAD_SPROC */
#define SDL_THREAD_WIN32 1
/* Enable various timer systems */
/* #undef SDL_TIMER_BEOS */
/* #undef SDL_TIMER_DC */
/* #undef SDL_TIMER_DUMMY */
/* #undef SDL_TIMER_MACOS */
/* #undef SDL_TIMER_MINT */
/* #undef SDL_TIMER_OS2 */
/* #undef SDL_TIMER_RISCOS */
/* #undef SDL_TIMER_UNIX */
#define SDL_TIMER_WIN32 1
/* #undef SDL_TIMER_WINCE */
/* Enable various video drivers */
/* #undef SDL_VIDEO_DRIVER_AALIB */
/* #undef SDL_VIDEO_DRIVER_BWINDOW */
/* #undef SDL_VIDEO_DRIVER_CACA */
/* #undef SDL_VIDEO_DRIVER_DC */
#define SDL_VIDEO_DRIVER_DDRAW 1
/* #undef SDL_VIDEO_DRIVER_DGA */
/* #undef SDL_VIDEO_DRIVER_DIRECTFB */
/* #undef SDL_VIDEO_DRIVER_DRAWSPROCKET */
#define SDL_VIDEO_DRIVER_DUMMY 1
/* #undef SDL_VIDEO_DRIVER_FBCON */
/* #undef SDL_VIDEO_DRIVER_GAPI */
/* #undef SDL_VIDEO_DRIVER_GEM */
/* #undef SDL_VIDEO_DRIVER_GGI */
/* #undef SDL_VIDEO_DRIVER_IPOD */
/* #undef SDL_VIDEO_DRIVER_NANOX */
/* #undef SDL_VIDEO_DRIVER_OS2FS */
/* #undef SDL_VIDEO_DRIVER_PHOTON */
/* #undef SDL_VIDEO_DRIVER_PICOGUI */
/* #undef SDL_VIDEO_DRIVER_PS2GS */
/* #undef SDL_VIDEO_DRIVER_PS3 */
/* #undef SDL_VIDEO_DRIVER_QTOPIA */
/* #undef SDL_VIDEO_DRIVER_QUARTZ */
/* #undef SDL_VIDEO_DRIVER_RISCOS */
/* #undef SDL_VIDEO_DRIVER_SVGALIB */
/* #undef SDL_VIDEO_DRIVER_TOOLBOX */
/* #undef SDL_VIDEO_DRIVER_VGL */
#define SDL_VIDEO_DRIVER_WINDIB 1
/* #undef SDL_VIDEO_DRIVER_WSCONS */
/* #undef SDL_VIDEO_DRIVER_X11 */
/* #undef SDL_VIDEO_DRIVER_X11_DGAMOUSE */
/* #undef SDL_VIDEO_DRIVER_X11_DYNAMIC */
/* #undef SDL_VIDEO_DRIVER_X11_DYNAMIC_XEXT */
/* #undef SDL_VIDEO_DRIVER_X11_DYNAMIC_XRANDR */
/* #undef SDL_VIDEO_DRIVER_X11_DYNAMIC_XRENDER */
/* #undef SDL_VIDEO_DRIVER_X11_VIDMODE */
/* #undef SDL_VIDEO_DRIVER_X11_XINERAMA */
/* #undef SDL_VIDEO_DRIVER_X11_XME */
/* #undef SDL_VIDEO_DRIVER_X11_XRANDR */
/* #undef SDL_VIDEO_DRIVER_X11_XV */
/* #undef SDL_VIDEO_DRIVER_XBIOS */
/* Enable OpenGL support */
#define SDL_VIDEO_OPENGL 1
/* #undef SDL_VIDEO_OPENGL_GLX */
#define SDL_VIDEO_OPENGL_WGL 1
/* #undef SDL_VIDEO_OPENGL_OSMESA */
/* #undef SDL_VIDEO_OPENGL_OSMESA_DYNAMIC */
/* Disable screensaver */
#define SDL_VIDEO_DISABLE_SCREENSAVER 1
/* Enable assembly routines */
#define SDL_ASSEMBLY_ROUTINES 1
#define SDL_HERMES_BLITTERS 1
/* #undef SDL_ALTIVEC_BLITTERS */
//#endif /* _SDL_config_h */

#define IMAGE_WIDTH		480
#define IMAGE_HEIGHT	640

//#include <opencv2\opencv.hpp>
#define PKG_BODY_LEN	1500

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

#define HEAD_FLAG "JVS"

// SDL init
SDL_Window *window;
SDL_Renderer *renderer;
SDL_RendererInfo info;
SDL_Surface *image;
SDL_Rect    rect;
SDL_Rect    m_showrect;
Uint8 *imageYUV;
SDL_Texture *texture;
SDL_Event event;
Uint32 then, now, frames;
SDL_bool done = SDL_FALSE;

int m_nFrameShowOK = 1;

char* yuvdata[3];

//2304000					1152000


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
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picture_display);
	DDX_Control(pDX, IDC_RICHEDIT_RX_NETWORKINFO, m_RichEditCtrl_loginfo);
	DDX_Control(pDX, IDC_CHECK_NOACK, m_button_no_ack);
}

BEGIN_MESSAGE_MAP(CNLUDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_FINDAVACOMPORT, OnReceiveAComPort)
	ON_MESSAGE(WM_COM_RX, OnThreadRXMessage)
	ON_MESSAGE(WM_COM_TX, OnThreadTXMessage)
	ON_MESSAGE(WM_NETWORK_RX, OnThreadNetWorkRXMessage)
	ON_MESSAGE(WM_SHOW_VIDEO, OnThreadShowVideoMessage)
	ON_BN_CLICKED(IDC_BUTTON_COM, &CNLUDlg::OnBnClickedButtonCom)
	ON_BN_CLICKED(IDC_BUTTON_LOOP_TEST, &CNLUDlg::OnBnClickedButtonLoopTest)
	ON_BN_CLICKED(IDC_BUTTON_ALL_CLEAR, &CNLUDlg::OnBnClickedButtonAllClear)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CNLUDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_CHECK_HEX, &CNLUDlg::OnBnClickedCheckHex)
	ON_BN_CLICKED(IDC_BUTTON_JSON, &CNLUDlg::OnBnClickedButtonJson)
	ON_BN_CLICKED(IDC_CHECK_JSON_SEND_TIMER, &CNLUDlg::OnBnClickedCheckJsonSendTimer)
	ON_BN_CLICKED(IDC_CHECK_CLEAR_ALL, &CNLUDlg::OnBnClickedCheckClearAll)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO_START, &CNLUDlg::OnBnClickedButtonVideoStart)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO_STOP, &CNLUDlg::OnBnClickedButtonVideoStop)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOMER_SERVICE_START, &CNLUDlg::OnBnClickedButtonCustomerServiceStart)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_SERVICE_STOP, &CNLUDlg::OnBnClickedButtonCustomServiceStop)
	ON_BN_CLICKED(IDC_BUTTON_ENV_INIT, &CNLUDlg::OnBnClickedButtonEnvInit)
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
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
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
typedef enum msg_type
{
	PKG_TYPE_START_REQ = 0x0001, //客户端开始发送视频流请求
	PKG_TYPE_START_RESP = 0x0101, //服务端回复的请求响应
	PKG_TYPE_STOP_REQ = 0x0002, //客户端结束发送视频流的请求
	PKG_TYPE_STOP_RESP = 0x0102,
	PKG_TYPE_DATA_REQ = 0x0003, //客户端发送视频流数据的请求
	PKG_TYPE_DATA_RESP = 0x0103
} msg_type;



typedef struct net_package
{
	unsigned char headFlag[3];  //固定字节,JVS
	unsigned char pversion;     //版本号,0x01
	unsigned short msgType;     //消息类型，取值参考pkg_type
	unsigned short retCode;     //错误码
	unsigned int seqNumb;       //序号，从1开始网上加
	unsigned int timeSpan;      //时间戳
	unsigned int cframeLen;    //1帧数据的总长度
	unsigned int cframeCnt;     //1帧数据的总包数
	unsigned int cframeIndex;   //1帧数据当前第几个包
	unsigned int bodyLen;       //消息体长度
	char value[PKG_BODY_LEN];
}net_package;

typedef struct video_param
{
	unsigned int width;
	unsigned int height;
}video_param;
typedef struct net_video_param
{
	unsigned int width;     //视频宽度
	unsigned int height;    //视频高度
	unsigned int fps;       //视频帧率
}net_video_param;

typedef struct m_cFrame
{
	unsigned int m_cFrameLenth;
	unsigned int m_cFrameRXCounter;
	unsigned int m_cFramePacketNumer;
	unsigned int m_cFrameTimeSpan;
	uint8_t m_cFrameData[BUFFERLENTH];
};

//char m_cVideoData[BUFFERLENTH];

inline void net_pkg_hton(net_package *pPkg)
{
	pPkg->msgType = htons(pPkg->msgType);
	pPkg->retCode = htons(pPkg->retCode);

	pPkg->seqNumb = htonl(pPkg->seqNumb);
	pPkg->timeSpan = htonl(pPkg->timeSpan);
	pPkg->cframeLen = htonl(pPkg->cframeLen);
	pPkg->cframeCnt = htonl(pPkg->cframeCnt);
	pPkg->cframeIndex = htonl(pPkg->cframeIndex);
	pPkg->bodyLen = htonl(pPkg->bodyLen);
}

inline void net_pkg_ntoh(net_package *pPkg)
{
	pPkg->msgType = ntohs(pPkg->msgType);
	pPkg->retCode = ntohs(pPkg->retCode);

	pPkg->seqNumb = ntohl(pPkg->seqNumb);
	pPkg->timeSpan = ntohl(pPkg->timeSpan);
	pPkg->cframeLen = ntohl(pPkg->cframeLen);
	pPkg->cframeCnt = ntohl(pPkg->cframeCnt);
	pPkg->cframeIndex = ntohl(pPkg->cframeIndex);
	pPkg->bodyLen = ntohl(pPkg->bodyLen);
}

inline void net_video_param_hton(net_video_param *pParam)
{
	pParam->width = htonl(pParam->width);
	pParam->height = htonl(pParam->height);
	pParam->fps = htonl(pParam->fps);
}

inline void net_video_param_ntoh(net_video_param *pParam)
{
	pParam->width = ntohl(pParam->width);
	pParam->height = ntohl(pParam->height);
	pParam->fps = ntohl(pParam->fps);
}
int mSeqIndex;
int                 m_nPkgCount;
int                 m_nCurLen;
unsigned char*     m_pFrameData;

#define HEAD_LEN (sizeof(net_package)-PKG_BODY_LEN)

m_cFrame m_cFrameData;
const char *filename, *outfilename;
const AVCodec *codec;
AVCodecParserContext *parser;
AVCodecContext *c = NULL;
FILE *f;
AVFrame *m_gpAVFrame; //存放解码后的数据
AVFrame *frame; //存放解码后的数据
uint8_t inbuf[BUFFERLENTH + AV_INPUT_BUFFER_PADDING_SIZE];
uint8_t *data;
size_t   data_size;
int ret;
AVPacket *pkt;

net_package pkg;

static void frame_save(AVFrame * m_AVFrame, char *filename)
{
	FILE *f;
	int i;

	f = fopen(filename, "ab+");
	//fprintf(f, "P5\n%d %d\n%d\n", frame->width, frame->height, 255);

	for (i = 0; i < frame->height; i++)
	{
		fwrite(frame->data[0] + i * frame->linesize[0], 1, frame->width, f);
	}
	for (i = 0; i < frame->height / 2; i++)
	{
		fwrite(frame->data[1] + i * frame->linesize[1], 1, frame->width / 2, f);
	}
	for (i = 0; i < frame->height / 2; i++)
	{
		fwrite(frame->data[2] + i * frame->linesize[2], 1, frame->width / 2, f);
	}

	fclose(f);
}

void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt,const char *filename)
{
	char buf[1024];
	int ret;

	ret = avcodec_send_packet(dec_ctx, pkt);//把avpkt放入解码队列的缓存中 avcodec_send_packet 调用后 会创建一个 avpkt对象，会把实际的数据 引用数据+1 或者copy一份
	if (ret < 0) {
		fprintf(stderr, "Error sending a packet for decoding\n");
		exit(1);
	}

	while (ret >= 0) 
	{
		ret = avcodec_receive_frame(dec_ctx, frame); //从解码成功的数据中取出一个 frame 解码的时候前几帧的可能获取失败 导致播放的时候视频最后的几帧没有播放（解决方案 结尾处的avcodec_send_packet avpkt传NULL 一直avcodec_receive_frame，直到取不出帧为止）
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			break;
		else if (ret < 0) {
			fprintf(stderr, "Error during decoding\n");
			break;
		}
		while (m_nFrameShowOK != 1)
		{

		}
		memcpy(m_gpAVFrame, frame,sizeof(AVFrame));
		//printf("saving frame %3d\n", dec_ctx->frame_number);
		//fflush(stdout);

		/* the picture is allocated by the decoder. no need to free it */
		//_snprintf(buf, sizeof(buf), "%s-%d", filename, dec_ctx->frame_number);
		//_snprintf(buf, sizeof(buf),"%s", filename);
		//_snprintf(buf, sizeof(buf), "%04d", dec_ctx->frame_number);
		//pgm_save(frame->data[0], frame->linesize[0],frame->width, frame->height, buf);
		//frame_save(m_gpAVFrame, buf);
		::PostMessageA(AfxGetApp()->m_pMainWnd->m_hWnd, WM_SHOW_VIDEO, 0, 0);
	}
}
int socket_nonblock_send(int fd, void* buffer, int length, long timeout)
{
	long written_bytes;
	char* ptr = (char*)buffer;
	int bytes_left = length;
	fd_set writefds;
	struct timeval tv;
	int ret = 0;
	while (bytes_left > 0)
	{
		written_bytes = send(fd, ptr, bytes_left, 0);
		if (written_bytes < 0)
		{
			if (errno == EINTR)     //由于信号中断，没写成功任何数据
			{
				written_bytes = 0;
			}
			else if (errno == EWOULDBLOCK)
			{    //即EAGAIN，socket内核缓存区满,或者网线断开
				FD_ZERO(&writefds);
				FD_SET(fd, &writefds);
				tv.tv_sec = timeout / 1000000;
				tv.tv_usec = timeout % 1000000;
				ret = select(fd + 1, NULL, &writefds, NULL, &tv); //阻塞,err:0 timeout err:-1 错误见errno
				if (ret == 0 || (ret < 0 && errno != EINTR)){//超时，判定为网线断开
					//LOGI("select error ret[%d][%s]", ret, strerror(errno));
					return -2;
				}
				written_bytes = 0;  //未超时，判定为socket缓冲区满导致的网络阻塞
			}
			else if (errno == EPIPE)
			{//连接套接字的本地端已关闭.(如对端被kill掉)
				//LOGI("write socket error %d:%s", errno, strerror(errno));
				return -1;
			}
			else
			{       //其他错误
				//LOGI("write socket error %d:%s", errno, strerror(errno));
				return -3;
			}
		}

		bytes_left -= written_bytes;
		ptr += written_bytes;
	}

	return 0;
}

#include <time.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif

int getTimeSpan()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);    //该函数在sys/time.h头文件中
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
int socket_send_other_data(int fd, int type, int seq, int retcode, void *pBody, int len)
{
	//LOGI("send data fd[%d],len[%d],seq[%d]", fd, len, seq);

	net_package pkg;
	memset(&pkg, 0, sizeof(pkg));
	strncpy((char*)pkg.headFlag, HEAD_FLAG, sizeof(pkg.headFlag));
	pkg.pversion = 0x01;
	pkg.msgType = type;
	pkg.seqNumb = seq;
	pkg.retCode = retcode;
	pkg.timeSpan = getTimeSpan();

	if (len > 0 && pBody)
	{
		memcpy(pkg.value, pBody, len);
		pkg.bodyLen = len;
	}

	net_pkg_hton(&pkg);
	int iRet = socket_nonblock_send(fd, &pkg, HEAD_LEN + len, 1000);
	if (iRet < 0)
	{
		//LOGE("send other failed, fd:%d, %s", fd, strerror(errno));
		return iRet;
	}
	return 0;
}

int proc_pkg_data(int socket, net_package *pPkg)
{
	if (pPkg->cframeCnt == 1)
	{//1帧只有1个包
	}
	else
	{
		/*
		if (m_pFrameData == NULL)		//第一次来分配相关内存
		{
			m_pFrameData = (unsigned char*)malloc(pPkg->cframeLen);
			if (!m_pFrameData)
			{
				//LOGE("malloc video buf failed");
				return -1;
			}
		}
		*/
		if (pPkg->bodyLen > sizeof(pPkg->value) || pPkg->bodyLen < 1)
		{
			//LOGE("the body len error, len:%d", pPkg->bodyLen);
			return -1;
		}
		else if ((m_nCurLen + pPkg->bodyLen) > pPkg->cframeLen)
		{
			//LOGE("this fram data error, len:%d,%d,%d", m_nCurLen, pPkg->bodyLen, pPkg->cframeLen);
			return -1;
		}
		memcpy(m_pFrameData + m_nCurLen, pPkg->value, pPkg->bodyLen);
		m_nCurLen += pPkg->bodyLen;
		m_nPkgCount++;

		if (pPkg->cframeCnt == m_nPkgCount)
		{
			//LOGI("this frame over:%d, len:%d", pPkg->timeSpan, m_nCurLen);
			//一帧接收完成,将帧数据发送到解码器
			data = (uint8_t *)m_pFrameData;
			int data_size = m_nCurLen;
			while (data_size > 0) {
				ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size, data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
				if (ret < 0) {
					fprintf(stderr, "Error while parsing\n");
					break;
				}
				data += ret;
				data_size -= ret;

				if (pkt->size)
					decode(c, frame, pkt, outfilename);
			}
			//free(m_pFrameData);
			//m_pFrameData = NULL;
			m_nPkgCount = 0;
			m_nCurLen = 0;
		}
	}

	return 0;
}

int proc_pkg_msg(int socket, net_package *pPkg)
{
	//LOGE("proc recv msg, type[%d]", pPkg->msgType);
	switch (pPkg->msgType)
	{
	case PKG_TYPE_START_REQ:
		//socket_send_other_data(socket, PKG_TYPE_START_RESP, pPkg->seqNumb, 0, NULL, 0);
		if (pPkg->bodyLen > 0)
		{
			net_video_param *pVParam = (net_video_param*)pPkg->value;
			net_video_param_ntoh(pVParam);
		}
		break;
	case PKG_TYPE_STOP_REQ:
		//socket_send_other_data(socket, PKG_TYPE_STOP_RESP, pPkg->seqNumb, 0, NULL, 0);
		break;
	case PKG_TYPE_DATA_REQ:
		proc_pkg_data(socket, pPkg);
		mSeqIndex = pPkg->seqNumb;
		//socket_send_other_data(socket, PKG_TYPE_DATA_RESP, mSeqIndex, 0, NULL, 0);
		mSeqIndex++;
		break;
	default:
		break;
	}
	return 0;
}

int proc_recv_buffer(int iSocket, unsigned char *szRecvBuf, const int dataLen)
{

	unsigned char *pBuf = szRecvBuf;
	int bufLen = dataLen;
	while (bufLen > HEAD_LEN)
	{
		if (strncmp((const char*)pBuf, HEAD_FLAG, strlen(HEAD_FLAG)) != 0)
		{
			//LOGE("skip data:%x,%x,%x", pBuf[0], pBuf[1], pBuf[2]);
			pBuf++;
			bufLen--;
			continue;
		}

		//LOGE("this will proc pkg");
		memset(&pkg, 0, sizeof(pkg));
		memcpy(&pkg, pBuf, HEAD_LEN);
		net_pkg_ntoh(&pkg);
		if (pkg.bodyLen > sizeof(pkg.value)){ //消息体长度错误
			bufLen -= HEAD_LEN;
			pBuf += HEAD_LEN;
			//LOGE("this data error, body len:%d, skip it", pkg.bodyLen);
			continue;
		}
		else if (bufLen < HEAD_LEN + pkg.bodyLen){ //消息体没收完
			//LOGE("not enough data parse:%d,%d,%d, wait next", pkg.bodyLen, HEAD_LEN, bufLen);
			break;
		}
		else{
			bufLen -= HEAD_LEN;
			pBuf += HEAD_LEN;
			if (pkg.bodyLen > 0)
			{
				memcpy(pkg.value, pBuf, pkg.bodyLen);
				bufLen -= pkg.bodyLen;
				pBuf += pkg.bodyLen;
			}
			proc_pkg_msg(iSocket, &pkg);
			if (pkg.msgType == PKG_TYPE_STOP_RESP)
			{
				//usleep(50 * 1000);
				break;
			}
		}
	}

	return bufLen;
}


// 数据接收线程
DWORD WINAPI ClientSessionThread(LPVOID lparam)
{
	SOCKET m_socketClient = (SOCKET)(LPVOID)lparam;
	char * recvBuf = (char *)malloc(BUFFERLENTH);
	unsigned char * m_FrameBuf = (unsigned char *)malloc(BUFFERLENTH*2);
	int m_ret;
	int m_nLenthRX=0;
	/*
	m_cFrameData.m_cFrameRXCounter = 0;
	m_cFrameData.m_cFramePacketNumer = 0;
	m_cFrameData.m_cFrameLenth = 0;
	m_cFrameData.m_cFrameTimeSpan = 0;
	*/
	while (1)	//保持连接
	{
		m_ret = recv(m_socketClient, recvBuf, BUFFERLENTH, 0);// 接受客户端消息
		if (m_ret < 0)
		{
			if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN){
				break;
			}
			closesocket(m_socketClient);
			//::PostMessage(AfxGetMainWnd()->m_hWnd, WM_NETWORK_RX, 0, 1);
			break;
		}
		else if (m_ret == 0) //断开连接
		{
			//printf("socket recv err [%d][%s]", socket, strerror(errno));
			closesocket(m_socketClient);
			//::PostMessage(AfxGetMainWnd()->m_hWnd, WM_NETWORK_RX, 0, 2);
			break;
		}
		else
		{
			int nNot = 0;
			if (m_ret > (BUFFERLENTH - m_nLenthRX))
			{
				//这一次收到的数据长度大于剩余的缓冲区
				nNot = 1;
			}
			else
			{
				//printf("this recv len:%d[%x,%x,%x]", iRecved, mszTmpBuf[0], mszTmpBuf[1], mszTmpBuf[2]);
				memcpy(m_FrameBuf + m_nLenthRX, recvBuf, m_ret);
				m_nLenthRX += m_ret;
			}

			int iRemainLen = proc_recv_buffer(m_socketClient, m_FrameBuf, m_nLenthRX); //函数返回剩余的内容长度
			if (iRemainLen > 0)
			{
				int proced = m_nLenthRX - iRemainLen;
				unsigned char *pSrc = m_FrameBuf + proced;
				memmove(m_FrameBuf, pSrc, iRemainLen);
				m_nLenthRX = iRemainLen;
			}
			else
			{
				memset(m_FrameBuf, 0, BUFFERLENTH*2);
				m_nLenthRX = 0;
			}

			if (nNot == 1 && m_ret < (BUFFERLENTH - m_nLenthRX))
			{
				//这一次收到的数据长度大于剩余的缓冲区
				//LOGE("copy too large buffer to buffer");
				memcpy(m_FrameBuf + m_nLenthRX, recvBuf, m_ret);
				m_nLenthRX += m_ret;
			}


			/*
			//协议解析
			net_package * m_net_package;
			m_net_package = (net_package *)recvBuf;
			//查找帧头
			//net_package pkg;
			if ((m_net_package->headFlag[0] == 0x4a) && (m_net_package->headFlag[1] == 0x56) && (m_net_package->headFlag[2] == 0x53))
			{
				//memset(&m_net_package, 0, sizeof(m_net_package));
				//memcpy(&m_net_package, recvBuf, HEAD_LEN);
				net_pkg_ntoh(m_net_package);
				switch (m_net_package->msgType)
				{
					case PKG_TYPE_START_REQ:
					{
						break;
					}
					case PKG_TYPE_START_RESP:
					{
						break;
					}
					case PKG_TYPE_STOP_REQ:
					{
						break;
					}
					case PKG_TYPE_STOP_RESP:
					{

						break;
					}
					case PKG_TYPE_DATA_REQ:
					{

						m_cFrameData.m_cFrameRXCounter++;
						//if ((m_cFrameData.m_cFrameRXCounter > m_cFrameData.m_cFramePacketNumer) || (m_cFrameData.m_cFramePacketNumer != pkg.cframeLen) || (m_cFrameData.m_cFramePacketNumer != pkg.cframeCnt))	//新帧
						if (m_cFrameData.m_cFrameTimeSpan != m_net_package->timeSpan)	//同一帧数据，时间戳相等
						{
							m_cFrameData.m_cFrameRXCounter = 1;
							m_cFrameData.m_cFrameLenth = m_net_package->cframeLen;
							m_cFrameData.m_cFramePacketNumer = m_net_package->cframeCnt;
							m_cFrameData.m_cFrameTimeSpan = m_net_package->timeSpan;
							//::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_SHOW_VIDEO, 0, 0);
						}
						memcpy(&m_cFrameData.m_cFrameData[(m_net_package->cframeIndex - 1) * 1500], m_net_package->value, m_net_package->bodyLen);

						if (m_cFrameData.m_cFramePacketNumer == m_cFrameData.m_cFrameRXCounter)
						{
							::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_SHOW_VIDEO, 0, 0);
						}
						break;
					}
					case PKG_TYPE_DATA_RESP:
					{

						 break;
					}
				}

			}
			else
			{
				//数据包帧头错误,数据直接扔掉
			}

			*/
		}
	}
	free(recvBuf);
	return 0;
}



LRESULT CNLUDlg::OnThreadShowVideoMessage(WPARAM wParam, LPARAM lParam)
{
	/*
	while (!feof(f)) {
		// read raw data from the input file 
		data_size = fread(inbuf, 1, BUFFERLENTH, f);
		if (!data_size)
			break;

		// use the parser to split the data into frames 
		data = inbuf;
		while (data_size > 0) {
			ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size, data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
			if (ret < 0) {
				fprintf(stderr, "Error while parsing\n");
				exit(1);
			}
			data += ret;
			data_size -= ret;

			if (pkt->size)
				decode(c, frame, pkt, outfilename);
		}
	}

	// flush the decoder 
	decode(c, frame, NULL, outfilename);
	*/
	/*
	data = (uint8_t *)pkg.value;
	int data_size = pkg.bodyLen;
	while (data_size > 0) {
		ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size, data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
		if (ret < 0) {
			fprintf(stderr, "Error while parsing\n");
			break;
		}
		data += ret;
		data_size -= ret;

		if (pkt->size)
			decode(c, frame, pkt, outfilename);
	}
	*/

	/*
	AVFrame * m_frame = (AVFrame *)wParam;
	char* yuvdata[3];
	int linesize[3] = { IMAGE_HEIGHT, IMAGE_HEIGHT / 2, IMAGE_HEIGHT / 2 };	//Y U V 行数
	//2304000					1152000
	yuvdata[0] = (char*)malloc((linesize[0] * IMAGE_WIDTH + linesize[1] * IMAGE_WIDTH / 2 + linesize[2] * IMAGE_WIDTH / 2));
	yuvdata[1] = yuvdata[0] + linesize[0] * IMAGE_WIDTH;
	yuvdata[2] = yuvdata[1] + linesize[1] * IMAGE_WIDTH;
	*/

	/*
	// SDL init
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_RendererInfo info;
	SDL_Surface *image;
	SDL_Rect    rect;
	Uint8 *imageYUV;
	SDL_Texture *texture;
	SDL_Event event;
	Uint32 then, now, frames;
	SDL_bool done = SDL_FALSE;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return -1;
	}

	// Create the window and renderer 
	//创建窗口  
	window = SDL_CreateWindowFrom((void *)(GetDlgItem(IDC_STATIC_PICTURE)->GetSafeHwnd()));
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -2;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Couldn't set create renderer: %s\n", SDL_GetError());
		return -3;
	}
	SDL_GetRendererInfo(renderer, &info);
	//printf("Using %s rendering\n", info.name);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, m_gpAVFrame->width, m_gpAVFrame->height);
	if (!texture) {
		fprintf(stderr, "Couldn't set create texture: %s\n", SDL_GetError());
		return -4;
	}

	rect.x = 0;
	rect.y = 0;
	rect.w = m_gpAVFrame->width;
	rect.h = m_gpAVFrame->height;
	// end
	*/

	// get YUV420P frame data

	int i;
	for (i = 0; i < m_gpAVFrame->height; i++)
	{
		memcpy(yuvdata[0] + i*IMAGE_WIDTH, m_gpAVFrame->data[0] + i*m_gpAVFrame->linesize[0], IMAGE_WIDTH);
	}
	for (i = 0; i < m_gpAVFrame->height/2; i++)
	{
		memcpy(yuvdata[1] + i*IMAGE_WIDTH/2, m_gpAVFrame->data[1] + i*m_gpAVFrame->linesize[1], IMAGE_WIDTH/2);
	}
	for (i = 0; i < m_gpAVFrame->height / 2; i++)
	{
		memcpy(yuvdata[2] + i*IMAGE_WIDTH/2, m_gpAVFrame->data[2] + i*m_gpAVFrame->linesize[2], IMAGE_WIDTH/2);
	}
	

	SDL_UpdateTexture(texture, &rect, yuvdata[0], IMAGE_WIDTH);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, texture, &rect, &m_showrect);
	SDL_RenderPresent(renderer);
	//SDL_Delay(1000);		//15 frame/s


	return 0;
}
UINT ThreadNetWork(LPVOID lpParam)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return err;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(18980);

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));// 绑定端口

	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;// 连接上的客户端ip地址
	int len = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);// 接受客户端连接,获取客户端的ip地址
		DWORD dwThreadId;
		HANDLE hThread;

		hThread = CreateThread(NULL, NULL, ClientSessionThread, (LPVOID)sockConn, 0, &dwThreadId);
		::PostMessage(AfxGetMainWnd()->m_hWnd, WM_NETWORK_RX, (WPARAM)&addrClient, 0);
		/*
		char sendBuf[50];
		sprintf(sendBuf, "Welcome %s to here!", inet_ntoa(addrClient.sin_addr));// 组合消息发送出去
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);// 发送消息到客户端
		char recvBuf[50];
		recv(sockConn, recvBuf, 50, 0);// 接受客户端消息
		printf("%s\n", recvBuf);
		*/
		//closesocket(sockConn);//断开连接
	}

	return 0;
}



void video_init()
{
	//filename = "test.h264";
	//outfilename = "testout.mp4";

	pkt = av_packet_alloc();	//分配结构体指针
	if (!pkt)
		exit(1);

	/* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
	//memset(inbuf + BUFFERLENTH, 0, AV_INPUT_BUFFER_PADDING_SIZE);

	/* find the MPEG-1 video decoder */
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);	//获取解码器
	if (!codec) {
		fprintf(stderr, "Codec not found\n");
		exit(1);
	}

	parser = av_parser_init(codec->id);	//初始化AVCodecParserContext
	if (!parser) {
		fprintf(stderr, "parser not found\n");
		exit(1);
	}

	c = avcodec_alloc_context3(codec);	//空间申请
	if (!c) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}

	/* For some codecs, such as msmpeg4 and mpeg4, width and height
	MUST be initialized there because this information is not
	available in the bitstream. */

	/* open it */
	if (avcodec_open2(c, codec, NULL) < 0) {
		fprintf(stderr, "Could not open codec\n");
		exit(1);
	}
	/*
	f = fopen(filename, "rb");
	if (!f) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}
	*/
	frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}
	m_gpAVFrame = av_frame_alloc();
	if (!m_gpAVFrame) {
		fprintf(stderr, "Could not allocate video m_gpAVFrame\n");
		exit(1);
	}
	/*
	while (!feof(f)) {
		// read raw data from the input file 
		data_size = fread(inbuf, 1, BUFFERLENTH, f);
		if (!data_size)
			break;

		// use the parser to split the data into frames 
		data = inbuf;
		while (data_size > 0) {
			ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
			if (ret < 0) {
				fprintf(stderr, "Error while parsing\n");
				exit(1);
			}
			data += ret;
			data_size -= ret;

			if (pkt->size)
				decode(c, frame, pkt, outfilename);
		}
	}

	// flush the decoder 
	decode(c, frame, NULL, outfilename);

	fclose(f);

	av_parser_close(parser);
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&pkt);
	*/

	m_pFrameData = (unsigned char*)malloc(BUFFERLENTH);
	if (!m_pFrameData)
	{
		//LOGE("malloc video buf failed");
		exit(1);
	}

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
	//m_buttonTimeClearAll.SetCheck(1);
	//SetTimer(1, 1000 * 60, NULL);
	m_nsMsgID = 0;
	video_init();

	pThreadRX = AfxBeginThread(ThreadNetWork, 0);
	pThreadRX->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);

	yuvdata[0] = (char*)malloc((IMAGE_HEIGHT * IMAGE_WIDTH + IMAGE_HEIGHT * IMAGE_WIDTH / 2 ));
	yuvdata[1] = yuvdata[0] + IMAGE_HEIGHT * IMAGE_WIDTH;
	yuvdata[2] = yuvdata[1] + (IMAGE_HEIGHT * IMAGE_WIDTH >> 2);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return ;
	}

	// Create the window and renderer 
	//创建窗口  
	window = SDL_CreateWindowFrom((void *)(GetDlgItem(IDC_STATIC_PICTURE)->GetSafeHwnd()));
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return ;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Couldn't set create renderer: %s\n", SDL_GetError());
		return ;
	}
	SDL_GetRendererInfo(renderer, &info);
	//printf("Using %s rendering\n", info.name);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, IMAGE_WIDTH, IMAGE_HEIGHT);
	if (!texture) {
		fprintf(stderr, "Couldn't set create texture: %s\n", SDL_GetError());
		return ;
	}

	CRect m_rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_PICTURE);//IDC_picture为picture控件ID

	pWnd->GetClientRect(&m_rect);//rc为控件的大小。


	rect.x = 0;
	rect.y = 0;
	rect.w = IMAGE_WIDTH;
	rect.h = IMAGE_HEIGHT;
	//rect.w = m_rect.Width();
	//rect.h = m_rect.Height();

	m_showrect.x = 0;
	m_showrect.y = 0;
	m_showrect.w = m_rect.Width();
	m_showrect.h = m_rect.Height();
	// end
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
	/*
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
	*/
	int i = 0;

	m_byteWriteFrame1[i++] = 0xa5;
	m_byteWriteFrame1[i++] = 0x01;
	m_byteWriteFrame1[i++] = 0x01;
	m_byteWriteFrame1[i++] = 0x04;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = m_nsMsgID & 0x00ff;
	m_byteWriteFrame1[i++] = (m_nsMsgID >> 8) & 0x00ff;

	//
	srand((unsigned int)0xff);

	m_byteWriteFrame1[i++] = rand();
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;

	BYTE checkcode = 0;
	for (int j = 0; j < i; j++)
	{
		checkcode += m_byteWriteFrame1[j];
	}

	checkcode = ~checkcode + 1;
	checkcode = checkcode ^ 0x01;
	m_byteWriteFrame1[i++] = checkcode;
	m_nsMsgID++;
	m_unSendLenth = i;
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

LRESULT CNLUDlg::OnThreadNetWorkRXMessage(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
		case 0:
		{
			SOCKADDR_IN * m_addrClient = (SOCKADDR_IN *)wParam;
			char * m_cpAddr = inet_ntoa(m_addrClient->sin_addr);
			CString m_str;
			m_str.Format("%s 已连接\r\n", m_cpAddr);
			m_edit_wakeup.SetWindowTextA(m_str);
			break;
		}
		case 1:
		{
			m_edit_wakeup.SetWindowTextA("网络错误，客户端断开\r\n");
			break;
		}
		case 2:
		{
			m_edit_wakeup.SetWindowTextA("客户端断开");
			break;
		}
	}


	return 0;
}
LRESULT CNLUDlg::OnThreadRXMessage(WPARAM wParam, LPARAM lParam)
{
	//收到新的数据包重置定时器
	//KillTimer(1);
	//SetTimer(1, 1000 * 60, NULL);

	static int m_snc = 0;
	m_snc += wParam;
	
	CString m_temp, m_strshow;
	m_temp.Format("%d", m_snc);
	m_edit_rx_num.SetWindowTextA(m_temp);

	CString m_strTime;
	SYSTEMTIME m_st;
	GetLocalTime(&m_st);
	m_strTime.Format(_T("时间戳:%2d:%2d:%2d.%3d ::"), m_st.wHour, m_st.wMinute, m_st.wSecond, m_st.wMilliseconds);

	//数据解析

	switch (m_byteRXFrame[2])
	{


		case 0x01:		//握手请求
		{

					 break;
		}
		case 0x02:		//心跳消息
		{		
			if (m_button_no_ack.GetCheck() == 1)
				goto DEAL_FINISH;
			switch (m_byteRXFrame[7])
			{
			case 100:
				m_strshow = "正常\n";
				break;
			case 101:
				m_strshow = "程序异常\n";
				break;
			case 102:
				m_strshow = "网络连接异常\n";
				break;
			case 103:
				m_strshow = "服务器请求失败或者超时，但网络连接是正常\n";
				break;
			default:
				m_strshow = "未知故障代码\n";
			}
			m_strshow.Insert(0, "[ACK]:");
			m_strshow.Insert(0, m_strTime);
			m_richeditctrl_result.SetSel(-1, -1);
			m_richeditctrl_result.ReplaceSel(m_strshow, 0);
			m_richeditctrl_result.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
			goto DEAL_FINISH;
			return 0;
			break;
		}
		case 0x04:		//语音消息
		{
					 break;
		}
		case 0xff:		//确认消息
		{
			if (m_button_no_ack.GetCheck() == 1)
				goto DEAL_FINISH;
			switch (m_byteRXFrame[7])
			{
			case 100:
				m_strshow = "正常\n";
				break;
			case 101:
				m_strshow = "网络连接异常\n";
				break;
			case 102:
				m_strshow = "网络连接异常\n";
				break;
			case 103:
				m_strshow = "服务器连接异常\n";
				break;
			case 104:
				m_strshow = "系统忙，无法响应\n";
				break;
			case 105:
				m_strshow = "checkcode 校验失败,TVM 主控需要重新发起握手协议\n";
				break;
			default:
				m_strshow = "未知故障代码\n";
			}
			m_strshow.Insert(0,"[CONFIRM]:");
			m_strshow.Insert(0, m_strTime);
			m_richeditctrl_result.SetSel(-1, -1);
			m_richeditctrl_result.ReplaceSel(m_strshow, 0);
			m_richeditctrl_result.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
			goto DEAL_FINISH;
			return 0;
		}
	}

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
					if (m_nstatus == 1)	goto DEAL_FINISH;
					//获取intent,根据intent获取内容

					std::string m_strIntent = root["param"]["intent"].asString();		// 访问节点
					std::string m_strKey = root["param"].toStyledString();
					//std::string m_strKey = root["slots"].toStyledString();


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
				if (m_strevent == "Input")
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
				if (m_strevent == "OnInput")
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
				if (m_strevent == "Query")
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
				if (m_strevent == "Report")
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
				if (m_strevent == "videocall_status")
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
				if (m_strevent == "videostream_status")
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
				if (m_strevent == "oneyuvframe _status")
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

	free(out_stream);
DEAL_FINISH:
		m_bDealFinish = true;

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

	CString m_strtemp;
	m_richeditctrl_input.GetWindowTextA(m_strtemp);

	int m_nInputLenth = m_strtemp.GetLength()/2;

	unsigned char *m_ucData = (unsigned char *)malloc(m_nInputLenth);
	unsigned char *m_ucpStart = m_ucData;
	for (int i = 0; i < m_nInputLenth; i++)
	{
		*m_ucData = strtol(m_strtemp.Left(2), NULL, 16);
		m_ucData++;
		m_strtemp.Delete(0,2);
	}

	BYTE checkcode = 0;
	for (int j = 0; j < m_nInputLenth; j++)
	{
		checkcode += *m_ucpStart;
		m_ucpStart++;
	}

	checkcode = ~checkcode + 1;
	checkcode = checkcode ^ 0x01;

	CString m_strTime;
	SYSTEMTIME m_st;
	GetLocalTime(&m_st);
	m_strTime.Format(_T("时间戳:%2d:%2d:%2d.%3d ::"), m_st.wHour, m_st.wMinute, m_st.wSecond, m_st.wMilliseconds);

	CString m_strResultShow;
	m_strResultShow.Format("校验码计算结果:%02X \n", checkcode);
	m_strResultShow.Insert(0, m_strTime);

	m_richeditctrl_rx_value.SetSel(-1, -1);
	m_richeditctrl_rx_value.ReplaceSel(m_strResultShow, 0);
	m_richeditctrl_rx_value.PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);
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
		const char* m_cSend = (LPCTSTR)m_strtemp;
		int lengthOfWcs = MultiByteToWideChar(CP_ACP, 0, m_cSend, -1, NULL, 0);
		lengthOfWcs--;
		wchar_t *wcs = new wchar_t[lengthOfWcs];
		MultiByteToWideChar(CP_ACP, 0, m_cSend, -1, wcs, lengthOfWcs);

		int lengthOfMbs = WideCharToMultiByte(CP_UTF8, 0, wcs, lengthOfWcs, NULL, 0, 0, 0);
		char* utf8Str = new char[lengthOfMbs];
		WideCharToMultiByte(CP_UTF8, 0, wcs, lengthOfWcs, utf8Str, lengthOfMbs, 0, 0);

		//压缩
		// data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 
		Bytef m_buffer[BUFFERLENTH];
		memset(m_buffer, 0, BUFFERLENTH);
		uLong m_nLenth;
		int m_ret = gzcompress((Bytef *)utf8Str, lengthOfMbs, m_buffer, &m_nLenth);
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
			/*
			BYTE* out_stream = (BYTE *)malloc(BUFFERLENTH);
			memset(out_stream, 0, BUFFERLENTH);
			uLong m_lLenth;
			int ret = gzdecompress(m_buffer, m_nLenth, out_stream, &m_lLenth);
			if (ret == 0)	//解压成功
			{
				// VC++默认使用ANSI，故取第一个参数为CP_ACP
				DWORD dBufSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)out_stream, m_lLenth, NULL, 0);
				wchar_t* dBuf = new wchar_t[dBufSize];
				wmemset(dBuf, 0, dBufSize);
				int nRet = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)out_stream, m_lLenth, dBuf, dBufSize);
				dBuf[dBufSize] = '\0';
			}
			*/

			int i = 0;

			m_byteWriteFrame1[i++] = 0xa5;
			m_byteWriteFrame1[i++] = 0x01;
			m_byteWriteFrame1[i++] = 0x04;
			m_byteWriteFrame1[i++] = m_nLenth & 0x000000ff;
			m_byteWriteFrame1[i++] = (m_nLenth >> 8) & 0x000000ff;
			m_byteWriteFrame1[i++] = m_nsMsgID & 0x00ff;
			m_byteWriteFrame1[i++] = (m_nsMsgID >> 8) & 0x00ff;
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


void CNLUDlg::OnBnClickedButtonVideoStart()
{
	// TODO:  在此添加控件通知处理程序代码
	
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		KillTimer(2);
		return;
	}
	USES_CONVERSION;
	//获取输入字符串
	CString m_strtemp;

	m_strtemp = "{\"event\":\"videostream_start\"}";
	SendCommand(m_strtemp);

}


void CNLUDlg::OnBnClickedButtonVideoStop()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		KillTimer(2);
		return;
	}
	USES_CONVERSION;
	//获取输入字符串
	CString m_strtemp;
	m_strtemp = "{\"event\":\"videostream_stop\"}";
	SendCommand(m_strtemp);

}


void CNLUDlg::OnBnClickedButtonCustomerServiceStart()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		KillTimer(2);
		return;
	}
	USES_CONVERSION;
	//获取输入字符串
	CString m_strtemp;
	m_strtemp = "{\"event\":\"videocall_start\"}";
	SendCommand(m_strtemp);
}


void CNLUDlg::OnBnClickedButtonCustomServiceStop()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		KillTimer(2);
		return;
	}
	USES_CONVERSION;
	//获取输入字符串
	CString m_strtemp;
	m_strtemp = "{\"event\":\"videocall_stop\"}";
	SendCommand(m_strtemp);
}
void CNLUDlg::SendCommand(CString m_strcmd)
{

	if (!m_strcmd.IsEmpty())
	{
		const char* m_cSend = (LPCTSTR)m_strcmd;
		int lengthOfWcs = MultiByteToWideChar(CP_ACP, 0, m_cSend, -1, NULL, 0);
		lengthOfWcs--;
		wchar_t *wcs = new wchar_t[lengthOfWcs];
		MultiByteToWideChar(CP_ACP, 0, m_cSend, -1, wcs, lengthOfWcs);

		int lengthOfMbs = WideCharToMultiByte(CP_UTF8, 0, wcs, lengthOfWcs, NULL, 0, 0, 0);
		char* utf8Str = new char[lengthOfMbs];
		WideCharToMultiByte(CP_UTF8, 0, wcs, lengthOfWcs, utf8Str, lengthOfMbs, 0, 0);

		//压缩
		// data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 
		Bytef m_buffer[BUFFERLENTH];
		memset(m_buffer, 0, BUFFERLENTH);
		uLong m_nLenth;
		int m_ret = gzcompress((Bytef *)utf8Str, lengthOfMbs, m_buffer, &m_nLenth);
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
			m_byteWriteFrame1[i++] = (m_nLenth >> 8) & 0x000000ff;
			m_byteWriteFrame1[i++] = m_nsMsgID & 0x00ff;
			m_byteWriteFrame1[i++] = (m_nsMsgID >> 8) & 0x00ff;
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

	m_RichEditCtrl_loginfo.SetSel(-1, -1);
	m_RichEditCtrl_loginfo.ReplaceSel(m_strcmd, 0);
	m_RichEditCtrl_loginfo.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}


void CNLUDlg::OnBnClickedButtonEnvInit()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bConnection == FALSE)
	{
		AfxMessageBox(_T("串口未打开"));
		KillTimer(2);
		return;
	}
	USES_CONVERSION;
	//获取输入字符串
	CString m_strtemp;
	CString m_strip;
	char    HostName[100];
	gethostname(HostName, sizeof(HostName));// 获得本机主机名.

	hostent* hn;
	hn = gethostbyname(HostName);//根据本机主机名得到本机ip

	m_strip = inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);//把ip换成字符串形式

	m_strtemp.Format("{\"event\":\"Init\",\"param\":{\"ip\":\"%s\",\"mask\":\"255.255.255.0\",\"gateway\":\"192.168.1.1\",\"dns\":\"192.168.1.1\",\"deviceID\":\"1010XXXX\",\"deviceType\": \"XX\",\"deviceSN\": \"XX\",\"port\":\"18980\",\"ServerIP\":\"%s\"}}",m_strip,m_strip);

	//m_strtemp = "{\"event\":\"Init\",\"param\":{\"ip\":\"192.168.1.100\",\"mask\":\"255.255.255.0\",\"gateway\":\"192.168.1.1\",\"dns\":\"192.168.1.1\",\"deviceID\":\"1010XXXX\",\"deviceType\": \"XX\",\"deviceSN\": \"XX\",\"port\":\"18980\",\"ServerIP\":\"192.168.1.100\"}}";

	SendCommand(m_strtemp);

}
