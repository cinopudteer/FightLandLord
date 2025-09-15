#pragma once
#include "NetProtocol.h"
#include <afxsock.h>
#pragma once
class CtestDoc;
class ClientSocket;
class ServerSocket : public CSocket
{
public:
    // ���캯������Ҫ���� Doc ָ��
    ServerSocket(CtestDoc* pDoc);
    virtual ~ServerSocket();

    // CSocket ���麯������
public:
    // �����µ���������ʱ���˺�������ܵ���
    virtual void OnAccept(int nErrorCode);

public:
    // �洢���������ӵĿͻ��� CClientSocket ����ָ��
    CObList m_clientList;

private:
    // ָ���ĵ����ָ��
    CtestDoc* m_pDoc;
};