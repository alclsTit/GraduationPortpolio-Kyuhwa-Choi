#include <iostream>
//#include <boost\array.hpp>

#include <boost/asio.hpp>
#include <boost\bind.hpp>

const char SERVERIP[] = "127.0.0.1";
const unsigned short PORT = 31400;
using namespace std;

class TCP_Client
{
public:
	TCP_Client(boost::asio::io_service& io_service)
		: m_io_service(io_service),
		m_Socket(io_service),
		m_nSeqNumber(0)
	{}

	void Connect(boost::asio::ip::tcp::endpoint& endpoint)
	{
		m_Socket.async_connect(endpoint, boost::bind(&TCP_Client::handle_connect,
			this, boost::asio::placeholders::error));
	}

private:
	boost::asio::io_service& m_io_service;
	boost::asio::ip::tcp::socket m_Socket;
	int m_nSeqNumber;
	array<char, 128> m_ReceiveBuffer;
	string m_WriteMessage;

	void PostWrite()
	{
		if (m_Socket.is_open() == false)
		{
			return;
		}


		if (m_nSeqNumber > 7)
		{
			m_Socket.close();
			return;
		}

		++m_nSeqNumber;

		string second_msg = "-Send Message";
		string first_msg = to_string(m_nSeqNumber);
		auto show_msg = first_msg + second_msg;

		m_WriteMessage = show_msg;

		boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
			boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

		PostReceive();
	}

	void PostReceive()
	{
		memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

		m_Socket.async_read_some(boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&TCP_Client::handle_receive, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void handle_connect(const boost::system::error_code& error)
	{
		if (error)
		{
			cout << " connect failed: " << error.message() << endl;
		}
		else
		{
			cout << " connected " << endl;
			PostWrite();
		}
	}

	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{

	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				cout << "서버와 연결이 끊어졌습니다. " << endl;
			}
			else
			{
				cout << "error No: " << error.value() << " error Message: "
					<< error.message() << endl;
			}
		}
		else
		{
			string strRecvMessage = m_ReceiveBuffer.data();
			cout << "서버에서 받은 메시지: " << strRecvMessage << ", 받은 크기: "
				<< bytes_transferred << endl;

			PostWrite();
		}
	}
};
int main()
{
	boost::asio::io_service io_service;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVERIP), PORT);

	TCP_Client client(io_service);
	client.Connect(endpoint);
	io_service.run();
	cout << "네트워크 접속 종료" << endl;
	getchar();

}

