#pragma once

using Packet = unsigned char;

class CClientAgent
{
private:
	// 통신 관련
	WSADATA m_wsaData;
	SOCKET m_sock;
	int m_retval{ 0 };

	// 윈도우 창 관련 
	WNDCLASS m_wndClass;
	HWND hWnd;
	MSG m_Msg;

	char m_serverIP[16]{ 0 };
	Packet m_recvBuf[MAX_BUF_SIZE]{ 0 };
	Packet m_sendBuf[MAX_BUF_SIZE]{ 0 };



	int recvn();

	//
	void err_display(char *msg, int error, int line);
	void err_quit(wchar_t *msg, int error);

public:
	CClientAgent();
	
	void Init(const HWND& hWnd, const HINSTANCE& hInstance);
	void InputServerIP();
	

	~CClientAgent();
};

