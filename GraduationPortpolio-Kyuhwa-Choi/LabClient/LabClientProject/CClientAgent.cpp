#include "stdafx.h"
#include "CClientAgent.h"

using namespace std;

int CClientAgent::recvn()
{
	int recived;
	Packet *ptr = m_recvBuf;
	int left = MAX_BUF_SIZE;

	while (left > 0)
	{
		recived = recv(m_sock, reinterpret_cast<char*>(ptr), left, 0);
		if (recived == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (recived == 0)
			break;
		
		left -= recived;
		ptr += recived;
	}

	return (MAX_BUF_SIZE - left);
}

//LPVOID -> WCHAR 
//에러메시지에 대한 내용을 매개변수로받음
void CClientAgent::err_display(char * msg, int error, int line)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
#ifdef _DEBUG
	printf("[ %s - %d ]", msg, line);
	wprintf(L"에러 %s\n", lpMsgBuf);
#endif
	LocalFree(lpMsgBuf);
}

void CClientAgent::err_quit(wchar_t * msg, int error)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

CClientAgent::CClientAgent()
{
}

//클라이언트
void CClientAgent::Init(const HWND & hWnd, const HINSTANCE & hInstance)
{
	//DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);

	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		int error = WSAGetLastError();
		err_quit(L"WSAStartUp Error", error);
	}

	//1.소켓 생성
	//m_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		err_quit(L"Socket()", error);
	}

	//2.대상 연결
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(m_serverIP);
	serveraddr.sin_port = htons(SERVERPORT);

	//m_retval = WSAConnect(m_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);
	m_retval = connect(m_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (m_retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		err_quit(L"Connect()", error);
	}


	//로그인
}

void CClientAgent::InputServerIP()
{
	ifstream getServerIP;
	getServerIP.open("ServerIP_TEXT", ios::app);
	getServerIP.getline(m_serverIP, 16);
	getServerIP.close();
}


CClientAgent::~CClientAgent()
{
}
