#pragma once
#include "NetProtocol.h"
#include <afxsock.h>
//�ͻ���ͨ��
class CtestDoc;
class ClientSocket : public CSocket
{
public:
    // ���캯�������봫�� CMyDoc ��ָ�룬�Ա���лص�
    ClientSocket(CtestDoc* pDoc);
    virtual ~ClientSocket();

    // CSocket ���麯������
public:
    // ���յ���������ʱ���˺�������ܵ���
    virtual void OnReceive(int nErrorCode);
    // �����ӹر�ʱ���˺�������ܵ���
    virtual void OnClose(int nErrorCode);

private:
    // ָ���ĵ����ָ�룬���ڵ����ĵ��е����ݴ�����
    CtestDoc* m_pDoc;
};

