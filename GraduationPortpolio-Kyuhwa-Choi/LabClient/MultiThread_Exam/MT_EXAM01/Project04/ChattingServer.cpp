#include "ChattingServer.h"

void ChatServer::Init(const int nMaxSessionCount)
{
	for (auto i = 0; i < nMaxSessionCount; ++i)
	{
		Session *pNewSession = new Session(i, m_acceptor.get_io_service(), this);
		m_SessionList.push_back(pNewSession);
		m_SessionQueue.push_back(i);

	}
}
