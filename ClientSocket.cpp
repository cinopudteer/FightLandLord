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

    // 创建一个足够大的缓冲区来接收数据
    char buffer[sizeof(GamePacket)];
    GamePacket* pPacket = (GamePacket*)buffer;

    // 接收数据
    int nBytesReceived = Receive(pPacket, sizeof(GamePacket));

    if (nBytesReceived == sizeof(GamePacket))
    {
        // 如果成功接收到一个完整的包，则通知 Doc 类进行处理
        if (m_pDoc)
        {
            m_pDoc->OnReceivePacket(pPacket, this);
        }
    }
    else if (nBytesReceived == 0 || nBytesReceived == SOCKET_ERROR)
    {
        // 如果连接被关闭或发生错误，则不处理
        // OnClose 会被自动调用
    }
}

void ClientSocket::OnClose(int nErrorCode)
{
    // 调用基类函数
    CSocket::OnClose(nErrorCode);

    // 通知 Doc 类有客户端断开连接了，以便进行清理
    /*if (m_pDoc)
    {
        m_pDoc->OnClientDisconnected(this);
    }*/
}
