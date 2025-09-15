#include "pch.h"
#include "ClientSocket.h"
#include "testDoc.h"
ClientSocket::ClientSocket(CtestDoc* pDoc)
{
	m_pDoc = pDoc;
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::OnReceive(int nErrorCode)
{
    CSocket::OnReceive(nErrorCode);

    // ����һ���㹻��Ļ���������������
    char buffer[sizeof(GamePacket)];
    GamePacket* pPacket = (GamePacket*)buffer;

    // ��������
    int nBytesReceived = Receive(pPacket, sizeof(GamePacket));

    if (nBytesReceived == sizeof(GamePacket))
    {
        // ����ɹ����յ�һ�������İ�����֪ͨ Doc ����д���
        if (m_pDoc)
        {
            m_pDoc->OnReceivePacket(pPacket, this);
        }
    }
    else if (nBytesReceived == 0 || nBytesReceived == SOCKET_ERROR)
    {
        // ������ӱ��رջ��������򲻴���
        // OnClose �ᱻ�Զ�����
    }
}

void ClientSocket::OnClose(int nErrorCode)
{
    // ���û��ຯ��
    CSocket::OnClose(nErrorCode);

    // ֪ͨ Doc ���пͻ��˶Ͽ������ˣ��Ա��������
    /*if (m_pDoc)
    {
        m_pDoc->OnClientDisconnected(this);
    }*/
}
