#include "pch.h"
#include "ServerSocket.h"
#include "testDoc.h"
ServerSocket::ServerSocket(CtestDoc* pDoc)
{
	m_pDoc = pDoc;
}

ServerSocket::~ServerSocket()
{
    // 清理所有客户端连接
    POSITION pos = m_clientList.GetHeadPosition();
    while (pos)
    {
        ClientSocket* pClient = (ClientSocket*)m_clientList.GetNext(pos);
        pClient->Close();
        delete pClient;
    }
    m_clientList.RemoveAll();
}

void ServerSocket::OnAccept(int nErrorCode)
{
    // 调用基类函数
    CSocket::OnAccept(nErrorCode);

    // 创建一个新的 CClientSocket 对象用于和新客户端通信
    ClientSocket* pClient = new ClientSocket(m_pDoc);

    // 接受连接请求
    if (Accept(*pClient))
    {
        // 如果接受成功，将这个新的 Socket 指针添加到我们的管理列表中
        m_clientList.AddTail(pClient);

        // 通知 Doc 类有新的客户端连接成功了
        if (m_pDoc)
        {
            m_pDoc->OnClientConnected(pClient);
        }
    }
    else
    {
        // 如果接受失败，清理掉刚刚创建的对象，防止内存泄漏
        delete pClient;
        // 可以根据 nErrorCode 在这里添加错误日志
    }
}
