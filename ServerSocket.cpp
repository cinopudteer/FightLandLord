#include "pch.h"
#include "ServerSocket.h"
#include "testDoc.h"
ServerSocket::ServerSocket(CtestDoc* pDoc)
{
	m_pDoc = pDoc;
}

ServerSocket::~ServerSocket()
{
    // �������пͻ�������
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
    // ���û��ຯ��
    CSocket::OnAccept(nErrorCode);

    // ����һ���µ� CClientSocket �������ں��¿ͻ���ͨ��
    ClientSocket* pClient = new ClientSocket(m_pDoc);

    // ������������
    if (Accept(*pClient))
    {
        // ������ܳɹ���������µ� Socket ָ����ӵ����ǵĹ����б���
        m_clientList.AddTail(pClient);

        // ֪ͨ Doc �����µĿͻ������ӳɹ���
        if (m_pDoc)
        {
            m_pDoc->OnClientConnected(pClient);
        }
    }
    else
    {
        // �������ʧ�ܣ�������ոմ����Ķ��󣬷�ֹ�ڴ�й©
        delete pClient;
        // ���Ը��� nErrorCode ��������Ӵ�����־
    }
}
