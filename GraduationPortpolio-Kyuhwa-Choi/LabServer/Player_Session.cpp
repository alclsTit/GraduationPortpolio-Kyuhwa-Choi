#include "Player_Session.h"

void Player_Session::Init()
{
	// 캐릭터별로 다른 상태치를 DB에 저장해놓고 나중에 이를 뽑아서 써야함

	// 기본적인 초기화 정보 설정

	m_isConnected = true;

	m_PlayerData.ID = m_ID;

	m_PlayerData.Pos.x = 160;
	m_PlayerData.Pos.y = 400;
	m_PlayerData.Pos.z = 100;

	m_PlayerData.UserInfo.cur_hp = 300;
	m_PlayerData.UserInfo.cur_mp = 100;
	m_PlayerData.UserInfo.origin_hp = 300;
	m_PlayerData.UserInfo.origin_mp = 100;

	m_PlayerData.UserInfo.exp = 0;
	m_PlayerData.UserInfo.level = 1;

	m_PlayerData.UserInfo.player_status.attack = 50;
	m_PlayerData.UserInfo.player_status.defend = 30;
	m_PlayerData.UserInfo.player_status.speed = 10;

	STC_ClientInit init_player;
	init_player.pack_size = sizeof(STC_ClientInit);
	init_player.pack_type = INIT_CLIENT;
	init_player.player_data = m_PlayerData;
	
}


void Player_Session::SendPacket(Packet * packet)
{
	int packet_size = packet[0];
	Packet *sendBuf = new Packet[packet_size];
	//unique_ptr<Packet> sendBuf = make_unique<Packet>(packet_size);

	memcpy(sendBuf, packet, packet_size);

	m_socket.async_write_some(boost::asio::buffer(sendBuf,packet_size),
		[=](const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (packet_size != bytes_transferred)
			{
				cout << "Client No. [ " << m_ID << " ] async_write_some packet bytes was NOT SAME !!" << endl;
			}
			delete[] sendBuf;
		}
	});
}

void Player_Session::m_RecvPacket()
{
	m_socket.async_read_some(boost::asio::buffer(m_RecvBuf, MAX_BUF_SIZE),
		[&](const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error.value() == boost::asio::error::operation_aborted)
			{
				//입출력오류
				return;
			}

			cout << "Client No. [ " << m_ID << " ] Disconnected - IP: " << m_socket.remote_endpoint().address().to_string()
				<< " ---  Port: " << m_socket.remote_endpoint().port() << endl;

			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			SetState(-1);
			m_isConnected = DISCONNECTED;

			//db에서 연결끊긴 플레이어 정보 삭제

			STC_Disconnected disconnected;
			disconnected.id = m_ID;

			//연결이 끊긴 플레이어를 몬스터들의 시야에서 없애버림
		}
		
		int curr_data_processing = bytes_transferred;
		Packet *recvBuf = m_RecvBuf;
		while (curr_data_processing > 0)
		{
			if (m_curr_packet_size == 0)
			{
				m_curr_packet_size = recvBuf[0];
				//if ()
			}

			auto left = m_curr_packet_size - m_prev_packet_size;
			if (left <= curr_data_processing)
			{
				memcpy(m_DataBuf + m_prev_packet_size, recvBuf, left);

				Processing(m_DataBuf);

				m_curr_packet_size = 0;
				m_prev_packet_size = 0;
				curr_data_processing -= left;
				recvBuf += left;
			}
			else
			{
				memcpy(m_DataBuf + m_prev_packet_size, recvBuf, curr_data_processing);
				m_prev_packet_size += curr_data_processing;
				curr_data_processing = 0;
				recvBuf += curr_data_processing;
			}
		}
		
		m_RecvPacket();
	});
}

void Player_Session::Processing(Packet * packet)
{
	// 패킷 0 - 패킷 사이즈
	// 패킷의 1번째 - 속성에 따라서 처리
	switch (packet[1])
	{
	case CHANGED_PLAYER_POSITION: {
		if (m_CurState == DEAD) break;
		else m_CurState = MOVE;

		m_PlayerData.Pos = *(reinterpret_cast<Position*>(&packet[2]));

		STC_ChangedPos changedPos;
		changedPos.id = m_ID;
		changedPos.pos = m_PlayerData.Pos;
		
		break;
	}
	case CHANGED_PLAYER_DIRECTION: {
		if (m_CurState == DEAD) break;
		
		m_PlayerData.dir = 


		break;
	}

	}
}

Player_Session::~Player_Session()
{
}
