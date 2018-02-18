#pragma once
#include "stdafx.h"

using BOOST_SOCKET = boost::asio::ip::tcp::socket;

enum Player_State
{
	IDLE,
	MOVE,
	ATTACK,
	DEAD
};

class Player_Session
{
private:
	BOOST_SOCKET m_socket;

	bool m_isConnected{ true };
	unsigned int m_ID{ 0 };

	//버퍼 변수
	Packet m_RecvBuf[MAX_BUF_SIZE]{ 0 };
	Packet m_DataBuf[MAX_BUF_SIZE]{ 0 };
	unsigned int m_prev_packet_size{ 0 };
	unsigned int m_curr_packet_size{ 0 };

	Player_Data m_PlayerData;
	int m_CurState;

private:
	void m_RecvPacket();

public:
	Player_Session(BOOST_SOCKET socket, const int& index)
		: m_socket(move(socket)), m_ID(index) {};

	lua_State *L;

	// 플레이어 상태 초기화
	void Init();

	int SetState(int state) { if (m_CurState != -1) m_CurState = state; }
	int GetState() const { return m_CurState; }

	bool GetConnectedState() const { return m_isConnected; }
	void SetConnectedState(bool flag) { m_isConnected = flag; }

	unsigned int GetID() const { return m_ID; }
	Player_Data* GetPlayerData() { return &m_PlayerData; }

	void SendPacket(Packet *packet);

	bool SetCharHp(int hp) { m_PlayerData.UserInfo.cur_hp = hp; return true; }
	int GetCharHp() const { return m_PlayerData.UserInfo.cur_hp; }

	bool SetCharMp(int mp) { m_PlayerData.UserInfo.cur_mp = mp; return true; }
	int SetCharMp() const { return m_PlayerData.UserInfo.cur_mp; }

	int GetCharOriginalHp() const { m_PlayerData.UserInfo.origin_hp; }
	int GetCharOriginalMp() const { m_PlayerData.UserInfo.origin_mp; }
	~Player_Session();
};

