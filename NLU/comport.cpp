#include "stdafx.h"
#include "comport.h"
#include "NLU.h"
HANDLE hCom; //����
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

/*����ѭ������ʵ��ѭ������*/

typedef BYTE QElemType; //������������ 

typedef struct
{
	QElemType *base;  //ѭ�����еĴ洢�ռ� �� ������ָ��ָʾ�洢ʵ�ʳ���10�ֽڵ��ַ���) 
	int front;  //��ͷԪ���±꣨ȡԪ�أ� 
	int rear;  //��βԪ���±꣨����Ԫ�أ� 
}SqQueue;

//����ѭ�����У�ѭ����������
int InitQueue(SqQueue &Q)
{
	Q.base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
	if (!Q.base)
		return -1;  //����ʧ�ܣ�����-1 
	Q.front = 0;
	Q.rear = 0;
	return 0;    //�����ɹ�������0 
}

//����Ԫ�أ�����У�
int EnQueue(SqQueue &Q, QElemType e)
{
	if ((Q.rear + 1) % MAXQSIZE == Q.front)	//��һ���Ƿ�Ͷ�ȡָ���ػ�
		return -1;  //����
	//memcpy(Q.base[Q.rear],e);  //��ֵ
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % MAXQSIZE;  //�±��һ 
	return 0;  //��ӳɹ�
}

//ȡԪ��(����ָ�룬ָ��ȡ����ֵ�� 
int DeQueue(SqQueue &Q, QElemType &e)
{
	if (Q.rear == Q.front)
		return -1;   //�ӿ�
	//memcpy(e,Q.base[Q.front]);
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % MAXQSIZE; //�±��һ 
	return 0;  //�ɹ�
}
//����Ԫ��(����ָ�룬ָ��ȡ����ֵ�� 
int JumpQueue(SqQueue &Q,int n)
{
	if (Q.rear == Q.front)
		return -1;   //�ӿ�
	//memcpy(e,Q.base[Q.front]);
	//e = Q.base[Q.front];
	Q.front = (Q.front + n) % MAXQSIZE; //�±��һ 
	return 0;  //�ɹ�
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

SqQueue q;  //���ݽṹ ���λ���������
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
	//���ڳ�ʼ��,�������������д��ں�,Ĭ�ϴ򿪴��ں���С�Ĵ���
	if (SearchPort())
	{
		//OpenPort(m_nMinPortNumber);
	}
	else
	{
		AfxMessageBox(_T("û���ҵ�����!"));
	}
}
BOOL comport::SearchPort()
{
	BOOL m_BHaveComPort = FALSE;
	//һ����Ϊ�������ںŲ�����256
	CString m_strport;

	for (int i = 1; i <= 256; i++)
	{
		m_strport.Format(_T("\\\\.\\COM%d"), i);
		hCom = CreateFile(m_strport,//COM��
			GENERIC_READ | GENERIC_WRITE, //�������д
			0, //��ռ��ʽ
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ���
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�ص���ʽ
			NULL);
		//�򿪳ɹ�
		if (hCom != (HANDLE)-1)
		{
			if (m_BHaveComPort == FALSE)
			{
				m_BHaveComPort = TRUE;
				m_nMinPortNumber = i;
			}

			::PostMessage((HWND)AfxGetMainWnd()->m_hWnd, WM_FINDAVACOMPORT, 0, (LPARAM)i);
			CloseHandle(hCom);	//�ҵ����ȹرմ����Ա���һ�����ڵĲ���
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
		hCom = CreateFile(m_strport,//COM��
			GENERIC_READ | GENERIC_WRITE, //�������д
			0, //��ռ��ʽ
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ���
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�ص���ʽ
			NULL);
		//�򿪳ɹ�
		if (hCom == (HANDLE)-1)
		{
			AfxMessageBox(_T("��ָ������ʧ��!"));
			m_bConnection = FALSE;
			return FALSE;
		}
		m_bConnection = TRUE;
		SetupComm(hCom, BUFFERLENTH, BUFFERLENTH); //���뻺����������������Ĵ�С����10000
		COMMTIMEOUTS TimeOuts;
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout = 2000;
		TimeOuts.ReadTotalTimeoutMultiplier = 50;
		TimeOuts.ReadTotalTimeoutConstant = 5000;
		//�ڶ�һ�����뻺���������ݺ���������������أ�
		//�������Ƿ������Ҫ����ַ���

		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier = 0;
		TimeOuts.WriteTotalTimeoutConstant = 0;
		SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ
		DCB dcb;
		GetCommState(hCom, &dcb);
		dcb.BaudRate = 115200; //������Ϊ115200
		dcb.ByteSize = 8; //ÿ���ֽ���8λ
		dcb.Parity = NOPARITY; //����żУ��λ
		dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
		dcb.fDtrControl = 0;
		// set DCB to configure the serial port
		SetCommState(hCom, &dcb);
		PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);

		//��ʼ�����λ�����У������ݽṹ����ֱ�ӽ��մ������ݣ�����������ݶ�ʧ
		if (InitQueue(q) == -1)
		{
			AfxMessageBox(_T("���ڽ������ݶ�������ʧ�ܣ�"));
		}

		m_bThreadRXrunning = TRUE;
		m_bThreadTXrunning = TRUE;
		m_bThreadFrameScanrunning = TRUE;
		m_bBufferReset = FALSE;
		//	�����߳�
		pThreadRX = AfxBeginThread(ThreadReadCOM, 0);
		pThreadRX->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);
		//  �����߳�
		pThreadTX = AfxBeginThread(ThreadSendCOM, 0);
		pThreadTX->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);

		//  ֡ɨ���߳�
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
	//�жϷ�ʽ��������
	static unsigned int m_srxcounter = 0;
	DWORD nBytesRead, dwEvent, dwError;
	COMSTAT cs;

	cs.cbInQue = 0;
	
	SetCommMask(hCom, EV_RXCHAR);
	while (m_bThreadRXrunning == TRUE)
	{
		memset(&m_osRead, 0, sizeof(OVERLAPPED));//ע��ÿ�ζ�ȡ����ʱ��Ҫ��ʼ��OVERLAPPED
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (WaitCommEvent(hCom, &dwEvent, NULL))
		{
			//ִ�гɹ����ط���ֵ
			ClearCommError(hCom, &dwError, &cs);
			if (!cs.cbInQue) continue;	//��������û���ֽ�
			if (dwEvent&EV_RXCHAR&&cs.cbInQue)
			{
				//���������ݴ���ԭ������ǻ�������С�����
				int m_nread_num = ReadFile(hCom, m_byteRXbuffer, cs.cbInQue, &nBytesRead, &m_osRead);
				if (m_nread_num > 0)
				{
					//::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_RX, cs.cbInQue, 0);
						//�����յĴ�������ȫ�����
					for (unsigned int i = 0; i < cs.cbInQue; i++)
					{
						EnQueue(q, m_byteRXbuffer[i]);
					}
				}
				else
				{
					if (GetLastError() == ERROR_IO_PENDING) 
					{
						GetOverlappedResult(hCom, &m_osRead, &nBytesRead, TRUE); // GetOverlappedResult���������һ��������ΪTRUE��������һֱ�ȴ���ֱ����������ɻ����ڴ�������ء� 
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
			//ִ��ʧ��
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
		//д�������
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
		//����һ������
		if (m_bSendPackage == TRUE)
		{
			WriteChar(m_byteWriteFrame1, m_unSendLenth);
			

			//WORD	m_ncrch16;

			//�������кź�У��
			//��һ֡
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
			//�ڶ�֡
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
			//����֡
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
			//����֡
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
			//������Ϣ�����Ի�����ʾ������Ϣ
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
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	do
	{
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//�����ֵֹ
		dfMinus = (double)(QPart2 - QPart1);
		dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	} while (dfTim < 0.0000173575*m_ns);
}
UINT ThreadFrameScan(LPVOID lpParam)
{
	while (m_bThreadFrameScanrunning == TRUE)
	{
		if (QueueLenth(q) < 8)	//��Ϣ��̳���
		{
			//Delayms(DELAYTIMES);
			Sleep(SLEEPTIME);
			continue;
		}
		//���ݽ���
		//�ж����ݽ����������п�������û�н�����ɣ�����ȡ���ж�
		//Q.base[Q.front]


		if ((q.base[q.front] == 0xa5) && (q.base[q.front + 1] == 0x01))	//֡ͷ�ж�
		{
			// ��ȡ���ݳ���У�����ݰ��Ƿ��������Ŀ���
			unsigned int m_unDataLenth = q.base[(q.front + 4) % MAXQSIZE] << 8;
			m_unDataLenth += q.base[(q.front + 3) % MAXQSIZE];
			unsigned int lenth = QueueLenth(q);

			//if (m_unDataLenth >= 0x10000)
			//	m_unDataLenth = m_unDataLenth;

			if (QueueLenth(q) >= (m_unDataLenth + 8))	// �����ݰ�����=����+8
			{
				/*
				for (int i = 0; i < m_unDataLenth + 8; i++)
				{
					m_byteRXFrame[i] = q.base[q.front + i];

				}
				ClearQueue(q);
				::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_RX, m_unDataLenth + 8, m_unDataLenth);	//�����ܳ���   data���ݳ���
				*/
				
				//��ʼУ������
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
				if (checkcode == m_byteRXFrame[m_unDataLenth + 7])	//У��ɹ�
				{
					//ȡ������
					JumpQueue(q, m_unDataLenth + 8);	//���������Ѵ�������ݣ���ȷ��
					//ClearQueue(q);
					::PostMessage(AfxGetMainWnd()->m_hWnd, WM_COM_RX, m_unDataLenth + 8, m_unDataLenth);	//�����ܳ���   data���ݳ���
				}
				else
				{
					//֡ͷ��ȷ,У��δͨ��
					//JumpQueue(q, m_unDataLenth + 8);	//���������Ѵ�������ݣ������
					ClearQueue(q);
				}
				
			}
			else  //���ݲ�ȫ
			{
				Sleep(SLEEPTIME);
				continue;
			}
		}
	}
	return 0;
}