#pragma once
#include "NetProtocol.h"
#include <afxsock.h>
#pragma once
class CtestDoc;
class ClientSocket;
class ServerSocket : public CSocket
{
public:
    // 构造函数，需要传入 Doc 指针
    ServerSocket(CtestDoc* pDoc);
    virtual ~ServerSocket();

    // CSocket 的虚函数重载
public:
    // 当有新的连接请求时，此函数被框架调用
    virtual void OnAccept(int nErrorCode);

public:
    // 存储所有已连接的客户端 CClientSocket 对象指针
    CObList m_clientList;

private:
    // 指向文档类的指针
    CtestDoc* m_pDoc;
};