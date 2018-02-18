#include "Player_Session.h"

void Player_Session::Init()
{
	// ĳ���ͺ��� �ٸ� ����ġ�� DB�� �����س��� ���߿� �̸� �̾Ƽ� �����

	// �⺻���� �ʱ�ȭ ���� ����

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
				//����¿���
				return;
			}

			cout << "Client No. [ " << m_ID << " ] Disconnected - IP: " << m_socket.remote_endpoint().address().to_string()
				<< " ---  Port: " << m_socket.remote_endpoint().port() << endl;

			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			SetState(-1);
			m_isConnected = DISCONNECTED;

			//db���� ������� �÷��̾� ���� ����

			STC_Disconnected disconnected;
			disconnected.id = m_ID;

			//������ ���� �÷��̾ ���͵��� �þ߿��� ���ֹ���
		}
		else
		{
			int curr_data_size = bytes_transferred;
			Packet *recvBuf = m_RecvBuf;
			while (curr_data_size > 0)
			{
				if (m_curr_packet_size == 0)
				{

				}
			}
		}
	}
}

Player_Session::~Player_Session()
{
}
