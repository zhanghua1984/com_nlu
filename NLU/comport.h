#pragma once
#define  DELAYTIMES 50
#define  SLEEPTIME 5
int		crc16(unsigned char *pchMsg, unsigned int wDataLen);
void	Delayms(int m_ns);

class comport
{
public:
	comport();
	virtual ~comport();
	void CloseComPort();
	BOOL OpenPort(int portnumber);
	BOOL SearchPort();
	BOOL WriteChar(BYTE* m_szWriteBuffer, DWORD m_nToSend);
	void InitPort();
	int m_nMinPortNumber;
};

