#pragma once
#include "NetProtocol.h"
#include <afxsock.h>
//客户端通信
class CtestDoc;
class ClientSocket : public CSocket
{
public:
    // 构造函数，必须传入 CMyDoc 的指针，以便进行回调
    ClientSocket(CtestDoc* pDoc);
    virtual ~ClientSocket();

    // CSocket 的虚函数重载
public:
    // 当收到网络数据时，此函数被框架调用
    virtual void OnReceive(int nErrorCode);
    // 当连接关闭时，此函数被框架调用
    virtual void OnClose(int nErrorCode);

private:
    // 指向文档类的指针，用于调用文档中的数据处理函数
    CtestDoc* m_pDoc;
};

