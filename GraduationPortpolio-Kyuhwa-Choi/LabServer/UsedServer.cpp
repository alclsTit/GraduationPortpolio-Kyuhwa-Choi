#include "stdafx.h"
#include "UsedServer.h"



void UsedServer::InitNPC_AI()
{

}

void UsedServer::Init()
{
	InitNPC_AI();

	//한글출력
	wcout.imbue(locale("korean"));
	WSADATA wsadata;
	
	if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{
		int error = WSAGetLastError();
		err_quit(L"WSAStartUp Error", error);
	}

	m_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);

	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(SERVERPORT);

	::bind(m_socket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (listen(m_socket, SOMAXCONN) != 0)
		err_display("Listen Error: ", WSAGetLastError(), 0);

	for (auto i = 0; i < MAX_PLAYER; ++i)
	{
		g_clients[i]->SetConnectedState(false);
		g_clients.
	}

}

void UsedServer::CheckCPUCoreCount()
{
}

void UsedServer::AcceptThread()
{
}

void UsedServer::err_display(char * msg, int error, int line)
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

void UsedServer::err_quit(wchar_t * msg, int error)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

UsedServer::UsedServer()
{
}


void UsedServer::SendPacket(int c_num,Packet * packet)
{
	int psize = reinterpret_cast<unsigned char *>(packet)[0];
	int ptype = reinterpret_cast<unsigned char *>(packet)[1];
	OverlappedEx *over = new OverlappedEx;
	over->event_type = OP_SEND;
	memcpy(over->IOCP_buf, packet, psize);
	ZeroMemory(&over->over, sizeof(over->over));
	over->wsabuf.buf = reinterpret_cast<char *>(over->IOCP_buf);
	over->wsabuf.len = psize;
	int retval = WSASend(g_clients[c_num]->GetSocket(), &over->wsabuf, 1, NULL,
		0, &over->over, NULL);
	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING)
			err_display("Error in SendPacket :", err_no, 0);
	}
}

UsedServer::~UsedServer()
{
}
