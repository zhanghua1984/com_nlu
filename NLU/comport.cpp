#include "stdafx.h"
#include "comport.h"
#include "NLU.h"
HANDLE hCom; //串口
extern BOOL m_bReady;

BYTE	m_byteWriteFrame1[BUFFERLENTH];
//BYTE	m_byteWriteFrame2[BUFFERLENTH];
//BYTE	m_byteWriteFrame3[BUFFERLENTH];
//BYTE	m_byteWriteFrame4[BUFFERLENTH];

unsigned int m_unSendLenth;

UINT ThreadReadCOM(LPVOID lpParam);
UINT ThreadSendCOM(LPVOID lpParam);
UINT ThreadFrameScan(LPVOID lpParam);




CWinThread* pThreadRX;
CWinThread* pThreadTX;
CWinThread* pThreadFrameScan;

BOOL	m_bThreadRXrunning;
BOOL	m_bThreadTXrunning;
BOOL	m_bThreadFrameScanrunning;
BYTE	m_byteRXbuffer[BUFFERLENTH];
BYTE	m_byteRXFrame[BUFFERLENTH];
OVERLAPPED m_overlap;
OVERLAPPED m_osRead, m_osWrite;
BYTE	m_bytecounter;
BOOL	m_bSendPackage;
extern BOOL	m_bWorking;
extern	BOOL	m_bHighSpeed;
extern	BOOL m_bConnection;

#define MAXQSIZE BUFFERLENTH

/*利用循环队列实现循环缓冲*/

typedef BYTE QElemType; //定义新类型名 

typedef struct
{
	QElemType *base;  //循环队列的存储空间 （ 用数组指针指示存储实际长度10字节的字符串) 
	int front;  //队头元素下标（取元素） 
	int rear;  //队尾元素下标（插入元素） 
}SqQueue;

//建立循环队列（循环缓冲区）
int InitQueue(SqQueue &Q)
{
	Q.base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
	if (!Q.base)
		return -1;  //创建失败，返回-1 
	Q.front = 0;
	Q.rear = 0;
	return 0;    //创建成功，返回0 
}

//插入元素（入队列）
int EnQueue(SqQueue &Q, QElemType e)
{
	if ((Q.rear + 1) % MAXQSIZE == Q.front)	//下一个是否和读取指针重回
		return -1;  //队满
	//memcpy(Q.base[Q.rear],e);  //赋值
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % MAXQSIZE;  //下标加一 
	return 0;  //入队成功
}

//取元素(数组指针，指向取到的值） 
int DeQueue(SqQueue &Q, QElemType &e)
{
	if (Q.rear == Q.front)
		return -1;   //队空
	//memcpy(e,Q.base[Q.front]);
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % MAXQSIZE; //下标加一 
	return 0;  //成功
}
//跳过元素(数组指针，指向取到的值） 
int JumpQueue(SqQueue &Q,int n)
{
	if (Q.rear == Q.front)
		return -1;   //队空
	//memcpy(e,Q.base[Q.front]);
	//e = Q.base[Q.front];
	Q.front = (Q.front + n) % MAXQSIZE; //下标加一 
	return 0;  //成功
}
int ClearQueue(SqQueue &Q)
{
	Q.front = 0;
	Q.rear = 0;
	return 0;
}
int QueueLenth(SqQueue Q)
{
	return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}

SqQueue q;  //数据结构 环形缓冲区队列
int flag;
QElemType strIn, strOut;
bool m_bBufferReset;

comport::comport()
{
}


comport::~comport()
{
}
void comport::InitPort()
{
	m_nMinPortNumber = -1;
	//串口初始化,先搜索本机所有串口号,默认打开串口号最小的串口
	if (SearchPort())
	{
		//OpenPort(m_nMinPortNumber);
	}
	else
	{
		AfxMessageBox(_T("没有找到串口!"));
	}
}
BOOL comport::SearchPort()
{
	BOOL m_BHaveComPort = FALSE;
	//一般认为本机串口号不大于256
	CString m_strport;

	for (int i = 1; i <= 256; i++)
	{
		m_strport.Format(_T("\\\\.\\COM%d"), i);
		hCom = CreateFile(m_strport,//COM口
			GENERIC_READ | GENERIC_WRITE, //允许读和写
			0, //独占方式
			NULL,
			OPEN_EXISTING, //打开而不是创建
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //重叠方式
			NULL);
		//打开成功
		if (hCom != (HANDLE)-1)
		{
			if (m_BHaveComPort == FALSE)
			{
				m_BHaveComPort = TRUE;
				m_nMinPortNumber = i;
			}

			::PostMessage((HWND)AfxGetMainWnd()->m_hWnd, WM_FINDAVACOMPORT, 0, (LPARAM)i);
			CloseHandle(hCom);	//找到后先关闭串口以便下一个串口的查找
		}
	}
	return m_BHaveComPort;
}

BOOL comport::OpenPort(int portnumber)
{
	CString m_strport;
	m_strport.Format(_T("\\\\.\\COM%d"), portnumber);
	if (portnumber != -1)
	{
		hCom = CreateFile(m_strport,//COM口
			GENERIC_READ | GENERIC_WRITE, //允许读和写
			0, //独占方式
			NULL,
			OPEN_EXISTING, //打开而不是创建
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //重叠方式
			NULL);
		//打开成功
		if (hCom == (HANDLE)-1)
		{
			AfxMessageBox(_T("打开指定串口失败!"));
			m_bConnection = FALSE;
			return FALSE;
		}
		m_bConnection = TRUE;
		SetupComm(hCom, BUFFERLENTH, BUFFERLENTH); //输入缓冲区和输出缓冲区的大小都是10000
		COMMTIMEOUTS TimeOuts;
		//设定读超时
		TimeOuts.ReadIntervalTimeout = 2000;
		TimeOuts.ReadTotalTimeoutMultiplier = 50;
		TimeOuts.ReadTotalTimeoutConstant = 5000;
		//在读一次输入缓冲区的内容后读操作就立即返回，
		//而不管是否读入了要求的字符。

		//设定写超时
		TimeOuts.WriteTotalTimeoutMultiplier = 0;
		TimeOuts.WriteTotalTimeoutConstant = 0;
		SetCommTimeouts(hCom, &TimeOuts); //设置超时
		DCB dcb;
		GetCommState(hCom, &dcb);
		dcb.BaudRate = 115200; //波特率为115200
		dcb.ByteSize = 8; //每个字节有8位
		dcb.Parity = NOPARITY; //无奇偶校验位
		dcb.StopBits = ONESTOPBIT; //一个停止位
		dcb.fDtrControl = 0;
		// set DCB to configure the serial port
		SetCommState(hCom, &dcb);
		PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);

		//初始化环形缓冲队列，该数据结构必须直接接收串口数据，否则造成数据丢失
		if (InitQueue(q) == -1)
		{
			AfxMessageBox(_T("串口接收数据队列申请失败！"));
		}

		m_bThreadRXrunning = TRUE;
		m_bThreadTXrunning = TRUE;
		m_bThreadFrameScanrunning = TRUE;
		m_bBufferReset = FALSE;
		//	接收线程
		pThreadRX = AfxBeginThread(ThreadReadCOM, 0);
		pThreadRX->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);
		//  发送线程
		pThreadTX = AfxBeginThread(ThreadSendCOM, 0);
		pThreadTX->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);

		//  帧扫描线程
		pThreadTX = AfxBeginThread(ThreadFrameScan, 0);
		pThreadTX->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);
	}
	return TRUE;
}

void comport::CloseComPort()
{
	CloseHandle(hCom);
}
UINT ThreadReadCOM(LPVOID lpParam)
{
	//中断方式串口收数
	static unsigned int m_srxcounter = 0;
	DWORD nBytesRead, dwEvent, dwError;
	COMSTAT cs;

	cs.cbInQue = 0;
	
	SetCommMask(hCom, EV_RXCHAR);
	while (m_bThreadRXrunning == TRUE)
	{
		memset(&m_osRead, 0, sizeof(OVERLAPPED));//注意每次读取串口时都要初始化OVERLAPPED
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (WaitCommEvent(hCom, &dwEvent, NULL))
		{
			//执行成功返回非零值
			ClearCommError(hCom, &dwError, &cs);
			if (!cs.cbInQue) continue;	//读缓冲区没有字节
			if (dwEvent&EV_RXCHAR&&cs.cbInQue)
			{
				//读出的数据错误原因可能是缓冲区大小引起的
				int m_nread_num = ReadFile(hCom, m_byteRXbuffer, cs.cbInQue, &nBytesRead, &m_osRead);
				if (m_nread_num > 0)
				{
					//::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_RX, cs.cbInQue, 0);
						//将接收的串口数，全部入队
					for (unsigned int i = 0; i < cs.cbInQue; i++)
					{
						EnQueue(q, m_byteRXbuffer[i]);
					}
				}
				else
				{
					if (GetLastError() == ERROR_IO_PENDING) 
					{
						GetOverlappedResult(hCom, &m_osRead, &nBytesRead, TRUE); // GetOverlappedResult函数的最后一个参数设为TRUE，函数会一直等待，直到读操作完成或由于错误而返回。 
						//::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_RX, cs.cbInQue, 0);
						for (unsigned int i = 0; i < cs.cbInQue; i++)
						{
							EnQueue(q, m_byteRXbuffer[i]);
						}
					}
				}
			}
		}
		else
		{
			//执行失败
			int m_nError;
			m_nError = GetLastError();
			PurgeComm(hCom, PURGE_RXCLEAR);
		}
	}
	return 0;
}
BOOL WriteChar(BYTE* m_szWriteBuffer, DWORD m_nToSend)
{
	OVERLAPPED osWrite = { 0 };
	DWORD dwWritten;
	DWORD dwRes;
	BOOL fRes;
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL)
	{
		return FALSE;
	}
	if (!WriteFile(hCom, m_szWriteBuffer, m_nToSend, &dwWritten, &osWrite))
	{
		int m_nError = GetLastError();
		if (m_nError != ERROR_IO_PENDING)
		{
			fRes = FALSE;
		}
		else
		{
			dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
			switch (dwRes)
			{
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(hCom, &osWrite, &dwWritten, FALSE))
				{
					fRes = FALSE;
				}
				else
				{
					if (dwWritten != m_nToSend)
					{
						fRes = FALSE;
					}
					else
					{
						fRes = TRUE;
					}
				}
				break;
			default:
			{
				fRes = FALSE;
				break;
			}
			}
		}
	}
	else
	{
		//写操作完成
		if (dwWritten != m_nToSend)
		{
			fRes = FALSE;
		}
		else
		{
			fRes = TRUE;
		}
	}
	//PurgeComm(hCom, PURGE_TXABORT|PURGE_TXCLEAR);
	CloseHandle(osWrite.hEvent);
	return fRes;
}

UINT ThreadSendCOM(LPVOID lpParam)
{
	//loop test data
	//A5 01 04 28 00 2B 00 1F 8B 08 00 00 00 00 00 00 00 AB 56 4A 2D 4B CD 2B 51 B2 52 F2 CF 0B 4F CC 4E 2D 2D 50 AA 05 00 1E 13 7B B7 14 00 00 00 3D
	//A5 01 04 14 00 01 00 7B 22 65 76 65 6E 74 22 3A 22 4F 6E 57 61 6B 65 75 70 22 7D 3A 
	/*
	int i=0;
	m_byteWriteFrame1[i++] = 0xa5;
	m_byteWriteFrame1[i++] = 0x01;
	m_byteWriteFrame1[i++] = 0x04;
	m_byteWriteFrame1[i++] = 0x14;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x01;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x7b;
	m_byteWriteFrame1[i++] = 0x22;
	m_byteWriteFrame1[i++] = 0x65;
	m_byteWriteFrame1[i++] = 0x76;
	m_byteWriteFrame1[i++] = 0x65;
	m_byteWriteFrame1[i++] = 0x6e;
	m_byteWriteFrame1[i++] = 0x74;
	m_byteWriteFrame1[i++] = 0x22;
	m_byteWriteFrame1[i++] = 0x3a;
	m_byteWriteFrame1[i++] = 0x22;
	m_byteWriteFrame1[i++] = 0x4f;
	m_byteWriteFrame1[i++] = 0x6e;
	m_byteWriteFrame1[i++] = 0x57;
	m_byteWriteFrame1[i++] = 0x61;
	m_byteWriteFrame1[i++] = 0x6b;
	m_byteWriteFrame1[i++] = 0x65;
	m_byteWriteFrame1[i++] = 0x75;
	m_byteWriteFrame1[i++] = 0x70;
	m_byteWriteFrame1[i++] = 0x22;
	m_byteWriteFrame1[i++] = 0x7d;
	m_byteWriteFrame1[i++] = 0x3a;
	*/

	/*
	m_byteWriteFrame1[i++] = 0x28;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x2b;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x1f;
	m_byteWriteFrame1[i++] = 0x8b;
	m_byteWriteFrame1[i++] = 0x08;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0xab;
	m_byteWriteFrame1[i++] = 0x56;
	m_byteWriteFrame1[i++] = 0x4a;
	m_byteWriteFrame1[i++] = 0x2d;
	m_byteWriteFrame1[i++] = 0x4b;
	m_byteWriteFrame1[i++] = 0xcd;
	m_byteWriteFrame1[i++] = 0x2b;
	m_byteWriteFrame1[i++] = 0x51;
	m_byteWriteFrame1[i++] = 0xb2;
	m_byteWriteFrame1[i++] = 0x52;
	m_byteWriteFrame1[i++] = 0xf2;
	m_byteWriteFrame1[i++] = 0xcf;
	m_byteWriteFrame1[i++] = 0x0b;
	m_byteWriteFrame1[i++] = 0x4f;
	m_byteWriteFrame1[i++] = 0xcc;
	m_byteWriteFrame1[i++] = 0x4e;
	m_byteWriteFrame1[i++] = 0x2d;
	m_byteWriteFrame1[i++] = 0x2d;
	m_byteWriteFrame1[i++] = 0x50;
	m_byteWriteFrame1[i++] = 0xaa;
	m_byteWriteFrame1[i++] = 0x05;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x1e;
	m_byteWriteFrame1[i++] = 0x13;
	m_byteWriteFrame1[i++] = 0x7b;
	m_byteWriteFrame1[i++] = 0xb7;
	m_byteWriteFrame1[i++] = 0x14;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x00;
	m_byteWriteFrame1[i++] = 0x3d;
	*/
	while (m_bThreadTXrunning == TRUE)
	{
		//发送一包数据
		if (m_bSendPackage == TRUE)
		{
			WriteChar(m_byteWriteFrame1, m_unSendLenth);
			

			//WORD	m_ncrch16;

			//填入序列号和校验
			//第一帧
			/*
			m_byteWriteFrame1[0x04] = m_bytecounter++;
			m_ncrch16 = crc16(m_byteWriteFrame1, 22);
			m_byteWriteFrame1[0x16] = m_ncrch16 & 0x00ff;
			m_ncrch16 = m_ncrch16 & 0xff00;
			m_ncrch16 = m_ncrch16 >> 8;
			m_byteWriteFrame1[0x17] = (unsigned char)m_ncrch16;
			WriteChar(m_byteWriteFrame1, 24);

			if (m_bHighSpeed == FALSE)
			{
				Sleep(SLEEPTIME);
			}
			else
			{
				Delayms(DELAYTIMES);
			}
			*/

			/*
			//第二帧
			m_byteWriteFrame2[0x04] = m_bytecounter++;
			m_ncrch16 = crc16(m_byteWriteFrame2, 22);
			m_byteWriteFrame2[0x16] = m_ncrch16 & 0x00ff;
			m_ncrch16 = m_ncrch16 & 0xff00;
			m_ncrch16 = m_ncrch16 >> 8;
			m_byteWriteFrame2[0x17] = (unsigned char)m_ncrch16;
			WriteChar(m_byteWriteFrame2, 24);
			if (m_bHighSpeed == FALSE)
			{
				Sleep(SLEEPTIME);
			}
			else
			{
				Delayms(DELAYTIMES);
			}
			//第三帧
			m_byteWriteFrame3[0x04] = m_bytecounter++;
			m_ncrch16 = crc16(m_byteWriteFrame3, 22);
			m_byteWriteFrame3[0x16] = m_ncrch16 & 0x00ff;
			m_ncrch16 = m_ncrch16 & 0xff00;
			m_ncrch16 = m_ncrch16 >> 8;
			m_byteWriteFrame3[0x17] = (unsigned char)m_ncrch16;
			WriteChar(m_byteWriteFrame3, 24);
			if (m_bHighSpeed == FALSE)
			{
				Sleep(SLEEPTIME);
			}
			else
			{
				Delayms(DELAYTIMES);
			}
			//第四帧
			m_byteWriteFrame4[0x04] = m_bytecounter++;
			m_ncrch16 = crc16(m_byteWriteFrame4, 22);
			m_byteWriteFrame4[0x16] = m_ncrch16 & 0x00ff;
			m_ncrch16 = m_ncrch16 & 0xff00;
			m_ncrch16 = m_ncrch16 >> 8;
			m_byteWriteFrame4[0x17] = (unsigned char)m_ncrch16;
			WriteChar(m_byteWriteFrame4, 24);
			if (m_bHighSpeed == FALSE)
			{
				Sleep(SLEEPTIME);
			}
			else
			{
				Delayms(DELAYTIMES);
			}
			*/
			//发送消息给主对话框，显示发送信息
			::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_TX, m_unSendLenth, 0);
			m_bSendPackage = FALSE;
		}
		if (m_bWorking == FALSE)
		{
			Sleep(SLEEPTIME);
		}
	}
	return 0;
}
int crc16(unsigned char *pchMsg, unsigned int wDataLen)
{
	unsigned  int wCRCTalbeAbs[] =
	{
		0x0000, 0xCC01, 0xD801, 0x1400, 0xF001,
			0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00,
			0x7800, 0xB401, 0x5000, 0x9C01, 0x8801,
			0x4400
	};
	unsigned  int wCRC = 0xFFFF;
	unsigned  int i = 0;
	unsigned char chChar = 0;

	for (i = 0; i < wDataLen; i++)
	{
		chChar = *pchMsg++;
		wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
		wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
	}
	return wCRC;
}
void Delayms(int m_ns)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	do
	{
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	} while (dfTim < 0.0000173575*m_ns);
}
UINT ThreadFrameScan(LPVOID lpParam)
{
	while (m_bThreadFrameScanrunning == TRUE)
	{
		if (QueueLenth(q) < 8)	//消息最短长度
		{
			//Delayms(DELAYTIMES);
			Sleep(SLEEPTIME);
			continue;
		}
		//数据解析
		//判定数据解析条件，有可能数据没有接收完成，不可取出判定
		//Q.base[Q.front]


		if ((q.base[q.front] == 0xa5) && (q.base[q.front + 1] == 0x01))	//帧头判定
		{
			// 读取数据长度校验数据包是否有完整的可能
			unsigned int m_unDataLenth = q.base[(q.front + 4) % MAXQSIZE] << 8;
			m_unDataLenth += q.base[(q.front + 3) % MAXQSIZE];
			unsigned int lenth = QueueLenth(q);

			//if (m_unDataLenth >= 0x10000)
			//	m_unDataLenth = m_unDataLenth;

			if (QueueLenth(q) >= (m_unDataLenth + 8))	// 总数据包长度=数据+8
			{
				/*
				for (int i = 0; i < m_unDataLenth + 8; i++)
				{
					m_byteRXFrame[i] = q.base[q.front + i];

				}
				ClearQueue(q);
				::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_RX, m_unDataLenth + 8, m_unDataLenth);	//数据总长度   data数据长度
				*/
				
				//开始校验整包
				BYTE checkcode = 0;
				for (int i = 0; i < m_unDataLenth + 8; i++)
				{
					m_byteRXFrame[i] = q.base[(q.front + i)% MAXQSIZE];
					
				}
				for (int i = 0; i < m_unDataLenth + 7; i++)
				{
					checkcode += m_byteRXFrame[i];
				}

				checkcode = ~checkcode + 1;
				checkcode = checkcode ^ 0x01;
				if (checkcode == m_byteRXFrame[m_unDataLenth + 7])	//校验成功
				{
					//取出数据
					JumpQueue(q, m_unDataLenth + 8);	//队列跳过已处理的数据，正确包
					//ClearQueue(q);
					::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_RX, m_unDataLenth + 8, m_unDataLenth);	//数据总长度   data数据长度
				}
				else
				{
					//帧头正确,校验未通过
					//JumpQueue(q, m_unDataLenth + 8);	//队列跳过已处理的数据，错误包
					ClearQueue(q);
				}
				
			}
			else  //数据不全
			{
				Sleep(SLEEPTIME);
				continue;
			}
		}
	}
	return 0;
}