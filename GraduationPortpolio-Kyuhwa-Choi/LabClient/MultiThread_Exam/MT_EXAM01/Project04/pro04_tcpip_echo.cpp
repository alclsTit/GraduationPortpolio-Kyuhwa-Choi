#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

const char SERVERIP[] = "127.0.0.1";
const unsigned short PORT = 31400;

using namespace std;


class Session
{
public:
	Session(boost::asio::io_service& io_service) : m_Socket(io_service)
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
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{

	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�." << endl;
			}
			else
			{
				cout << "error No: " << error.value() << " error Message: " << error.message() << endl;
			}
		}
		else
		{
			string strRecvMessage = m_ReceiveBuffer.data();
			
			cout << "Ŭ���̾�Ʈ���� ���� �޽���: " << strRecvMessage <<
				", ���� ũ��: " << bytes_transferred << endl;
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

	boost::asio::ip::tcp::socket m_Socket;
	string m_WriteMessage;
	array<char, 128> m_ReceiveBuffer;
};

class TCP_Server
{
public:
	TCP_Server(boost::asio::io_service& io_service) : m_acceptor(io_service,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT))
	{
		m_pSession = nullptr;
		StartAccept();
	}
	~TCP_Server()
	{
		if (m_pSession != nullptr)
		{
			delete m_pSession;
		}
	}

private:
	void StartAccept()
	{
		cout << "Ŭ���̾�Ʈ ���� ���...." << endl;
		m_pSession = new Session(m_acceptor.get_io_service());
		m_acceptor.async_accept(m_pSession->Socket(), boost::bind(&TCP_Server::handle_accept,
			this, m_pSession, boost::asio::placeholders::error));
	}

	void handle_accept(Session* pSession, const boost::system::error_code& error)
	{
		if (!error)
		{
			cout << "Ŭ���̾�Ʈ ���� ����" << endl;
			pSession->PostReceive();
		}
	}

	int m_nSeqNumber;
	boost::asio::ip::tcp::acceptor m_acceptor;
	Session* m_pSession;
};


int main()
{
	boost::asio::io_service io_service;
	TCP_Server server(io_service);
	io_service.run();

	cout << "��Ʈ��ũ ���� ����" << endl;
	getchar();

}

