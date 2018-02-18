#pragma once

//������ ��� ������ �����Ѵ�.
//1. ���� ������
//2. ��Ƽ�ھ� ���� Ȯ��
//3. ��Ƽ�����带 �̿��� �����۵�

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

