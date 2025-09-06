
// testDoc.h: CtestDoc 类的接口
//


#pragma once
#include "CCard.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "NetProtocol.h"
class CtestDoc : public CDocument
{
protected: // 仅从序列化创建
	CtestDoc() noexcept;
	DECLARE_DYNCREATE(CtestDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CtestDoc();
	           //中间没有发牌的牌堆
	CObList m_cardListHand;     //自己的牌
	CObList m_leftPlayerHand;   // 左边玩家手牌
	CObList m_rightPlayerHand;  // 右边玩家手牌
	//CObList m_playedCardList;   //已经打出去的牌
	CObList m_cardListAtTop;
	CImage backGround;//背景图

	ServerSocket m_serverSocket;//如果是主机，用这个监听
	CObList m_clientSockets;     //如果是主机，保存所有客户端连接
	ClientSocket m_clientSocket;//如果是客户端，用这个连接主机
	CMap <int, int, CObList*, CObList*> m_currentRoundCards;//用于储存本轮打出的牌
	CCard* FindAndRemoveCardFromHand(int& playerIndex, CardData& cards);

	int m_passCount;            //本轮“不要”人数
	int m_lastWinnerIndex;      //上一轮赢家的索引
	int m_currentPlayerIndex;   //当前轮到谁出牌
	int m_cardWidth = 80;
	int m_cardHeight = 105;

	BOOL m_isServer;            //标记自己是不是主机
	int playerIndex = 2;        //自己在一局游戏中的玩家序号
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	void InitializeGame();      // 初始化游戏和牌局
	void SortCards(CObList&);         // 洗牌
	void CleanAllList(); // 用于释放内存
	void CleanOneList(CObList& list);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CString StrName;
	virtual void DeleteContents();

	//网络操作
	void StartServer();
	void ConnectToServer(const CString& serverAddress);
	void BroadcastPacket(GamePacket* packet);//服务器广播
	void SendPacket(GamePacket* packet);//发送数据包
	void OnReceivePacket(GamePacket* packet, ClientSocket* pFromSocket);//接受数据包
	void ClearRound();   //一轮回合结束后清理？

};
