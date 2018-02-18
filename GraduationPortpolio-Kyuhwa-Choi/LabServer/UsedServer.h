#pragma once

//서버의 모든 동작을 관여한다.
//1. 서버 아이피
//2. 멀티코어 갯수 확인
//3. 멀티스레드를 이용한 서버작동

class UsedServer
{
private:
	SOCKET m_socket;
	HANDLE m_hiocp;
	
	void m_RecvPacket();

	void Init();
	void InitNPC_AI();
	void CheckCPUCoreCount();
	void AcceptThread();
	void err_display(char *msg, int error, int line);
	void err_quit(wchar_t *msg, int error);



public:
	UsedServer();

	void SendPacket(int c_num, Packet* packet);
	~UsedServer();
};

