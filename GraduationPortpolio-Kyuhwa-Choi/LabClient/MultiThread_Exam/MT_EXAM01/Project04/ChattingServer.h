#pragma once
#include "stdafx.h"
#include <list>
#include <vector>
#include <queue>

class Session
{
public:
	Session(int count, boost::asio::io_service& io_service, Session* ses) : m_Socket(io_service)
	{}

	boost::asio::ip::tcp::socket& Socket()
	{
		return m_Socket;
	}

	void PostReceive()
	{
		memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));
		m_Socket.async_read_some
		(
			boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&Session::handle_receive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
		);
	}

private:
	boost::asio::ip::tcp::socket m_Socket;
	string m_WriteMessage;
	array<char, 128> m_ReceiveBuffer;

private:
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{

	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				cout << "클라이언트와 연결이 끊어졌습니다." << endl;
			}
			else
			{
				cout << "error No: " << error.value() << " error Message: " << error.message() << endl;
			}
		}
		else
		{
			string strRecvMessage = m_ReceiveBuffer.data();

			cout << "클라이언트에서 받은 메시지: " << strRecvMessage <<
				", 받은 크기: " << bytes_transferred << endl;
			string first_msg = "Re: ";
			auto show_msg = first_msg + strRecvMessage;

			m_WriteMessage = show_msg;

			boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
				boost::bind(&Session::handle_write, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

			PostReceive();
		}
	}
};


class ChatServer
{
private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	vector<Session*> m_SessionList;
	vector<int> m_SessionQueue;

public:
	void Init(const int nMaxSessionCount);
};