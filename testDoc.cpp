
// testDoc.cpp: CtestDoc 类的实现
//

#include "pch.h"
#include "framework.h"
#include "NetProtocol.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "test.h"
#endif

#include "testDoc.h"

#include <propkey.h>

#include<random>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CtestDoc

IMPLEMENT_DYNCREATE(CtestDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestDoc, CDocument)
END_MESSAGE_MAP()


// CtestDoc 构造/析构

CtestDoc::CtestDoc() noexcept:m_isServer(TRUE),m_passCount(0),m_lastWinnerIndex(2)//暂且默认为2
{
	// TODO: 在此添加一次性构造代码
	m_currentRoundCards.InitHashTable(5);//哈希表
	m_currentRoundCards.SetAt(0, new CObList());
	m_currentRoundCards.SetAt(1, new CObList());
	m_currentRoundCards.SetAt(2, new CObList());
}

CtestDoc::~CtestDoc()
{
	for (int i = 0; i < 3; i++) {
		CObList* pList = nullptr;
		if (m_currentRoundCards.Lookup(i, pList)) {
			delete pList;
		}
	}
}

BOOL CtestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	InitializeGame();
	return TRUE;
}

void CtestDoc::InitializeGame(){
	CCard::LoadBackImage();
	CImage masterImage;//包含所有扑克牌的那张大图
	if (FAILED(masterImage.Load(_T("res/lCardBig.bmp")))) {
		return;
	}
	if (FAILED(backGround.Load(_T("res/ReviewBK.bmp")))) {
		return;
	}
	int masterWidth = masterImage.GetWidth();
	int masterHeight = masterImage.GetHeight();
	const int CardWidth = masterWidth / 13;
	const int CardHeight = masterHeight / 5;


	std::vector<CCard*> tempDeck;//将每张牌的图像加载出来
	for (int suit = 0; suit <= 4; suit++) {
		for (int rank = 0; rank < 13; rank++) {
			CCard* pCard = nullptr;
			if (suit == 4) {
				if (rank == 0) {
					pCard = new CCard(SUIT_JOKER, RANK_SK);
				}
				else if(rank==1) {
					pCard = new CCard(SUIT_JOKER, RANK_BK);
				}
				else if (rank == 2) {
					pCard = new CCard(SUIT_BACK, RANK_BACK);
				}
				else {

				}
			}
			else {
				if (rank < 2) {
					pCard = new CCard(suit, rank + 11);//创建一个CCard对象，充当添加到tempDeck的运输车
				}
				else{
					pCard = new CCard(suit, rank -2);//创建一个CCard对象，充当添加到tempDeck的运输车
				}
			}
			if (pCard != nullptr) {//下面将包含所有扑克牌的大图切割，一个一个添加到tempDeck
				pCard->m_image.Create(CardWidth, CardHeight, masterImage.GetBPP());//设置参数宽度为 cardWidth，高度为 cardHeight。图像的位深度与原始图片保持一致。
				CDC* pCardDC = CDC::FromHandle(pCard->m_image.GetDC());//获取上下文
				masterImage.BitBlt(pCardDC->GetSafeHdc(), 0, 0, CardWidth, CardHeight, rank * CardWidth, suit * CardHeight);//复制图像
				pCard->m_image.ReleaseDC();
				CPoint centerPoint(1000, 640);
				pCard->m_rect.SetRect(centerPoint.x - CardWidth / 2, centerPoint.y - CardHeight / 2,centerPoint.x + CardWidth / 2, centerPoint.y + CardHeight / 2);//刚开始把牌放在屏幕中央
				tempDeck.push_back(pCard);
			}

		}
	}
	std::random_device rd;//洗牌
	std::mt19937 g(rd());
	std::shuffle(tempDeck.begin(), tempDeck.end()-1, g);
	int i = 0;
	for (int j = 0; j < 51;j++) {
		if (i == 0) {
			m_leftPlayerHand.AddTail(tempDeck[j]);
		}
		else if (i == 1) {
			m_rightPlayerHand.AddTail(tempDeck[j]);
		}
		else {
			m_cardListHand.AddTail(tempDeck[j]);
		}
		i ++;
		i %= 3;
	}
	m_cardListAtTop.AddTail(tempDeck[51]);
	m_cardListAtTop.AddTail(tempDeck[52]);
	m_cardListAtTop.AddTail(tempDeck[53]);
	SortCards(m_leftPlayerHand);
	SortCards(m_rightPlayerHand);
	SortCards(m_cardListHand);
}

void CtestDoc::SortCards(CObList& cardList)
{
	if (cardList.IsEmpty()) {
		return;
	}
	CArray<CCard*> sortArray;
	sortArray.SetSize(cardList.GetCount());
	int i=0;
	POSITION pos = cardList.GetHeadPosition();
	while (pos) {
		sortArray[i++] = (CCard*)cardList.GetNext(pos);
	}
	
	int n = sortArray.GetSize();
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (*sortArray[j] < *sortArray[j + 1]) {
				CCard* temp = sortArray[j];
				sortArray[j] = sortArray[j + 1];
				sortArray[j + 1] = temp;
			}
		}
	}
	
	cardList.RemoveAll();
	for (int i = 0; i < n; i++) {
		cardList.AddTail(sortArray[i]);
	}
}

void CtestDoc::CleanAllList()
{
	//CleanOneList(m_playedCardList);
	CleanOneList(m_leftPlayerHand);
	CleanOneList(m_rightPlayerHand);
	CleanOneList(m_cardListHand);
}
void CtestDoc::CleanOneList(CObList& list) {
	POSITION pos = list.GetHeadPosition();
	while (pos) {
		delete list.GetNext(pos);
	}
	list.RemoveAll();
}

// CtestDoc 序列化

void CtestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CtestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CtestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}



void CtestDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CtestDoc 诊断

#ifdef _DEBUG
CCard* CtestDoc::FindAndRemoveCardFromHand(int& playerIndex1, CardData& cards)
{
	CObList* pHandList = nullptr;
	switch ((2 + playerIndex1 - playerIndex) % 3) {
	case 0: pHandList = &m_leftPlayerHand; break;
	case 1: pHandList = &m_rightPlayerHand; break;
	case 2: pHandList = &m_cardListHand; break;
	}
	if (!pHandList) {
		return nullptr;
	}
	POSITION pos = pHandList->GetHeadPosition();
	POSITION prevPos = pos; // CObList 删除时需要前一个位置

	while (pos)
	{
		CCard* pCard = (CCard*)pHandList->GetNext(pos);

		// 3. 比较花色和点数
		if (pCard->m_suit == cards.suit && pCard->m_rank == cards.rank)
		{
			pHandList->RemoveAt(prevPos);
			return pCard;
		}

		prevPos = pos; // 更新 prevPos
	}

	return nullptr;
}
void CtestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestDoc 命令

void CtestDoc::DeleteContents()
{
	// TODO: 在此添加专用代码和/或调用基类
	CleanAllList();
	CDocument::DeleteContents();
}

void CtestDoc::StartServer()
{
	// TODO: 在此处添加实现代码.
	m_isServer = TRUE;
	if (!m_serverSocket.Create(12345)) {
		AfxMessageBox(_T("创建网络服务器失败！"));
	}
	if (!m_serverSocket.Listen()) {
		AfxMessageBox(_T("端口监听失败！"));
	}
}

void CtestDoc::ConnectToServer(const CString& serverAddress)
{
	m_isServer = FALSE;
	if (!m_clientSocket.Create()) {
		AfxMessageBox(_T("创建客户端Socket失败！"));
		return;
	}
	if (!m_clientSocket.Connect(serverAddress, 12345)) {
		// 注意: Connect是非阻塞的，结果会在OnConnect回调中处理
	}
}

void CtestDoc::BroadcastPacket(GamePacket* packet)
{
	if (!m_isServer) return;
	POSITION pos = m_clientSockets.GetHeadPosition();
	while (pos) {
		ClientSocket* pClient = (ClientSocket*)m_clientSockets.GetNext(pos);
		pClient->Send(packet, sizeof(GamePacket));
	}
	OnReceivePacket(packet, nullptr);//服务器自己也要处理这个消息
}

void CtestDoc::SendPacket(GamePacket* packet)
{
	if (m_isServer) {
		OnReceivePacket(packet, nullptr);//主机直接处理自己的操作
	}
	else {
		m_clientSocket.Send(packet, sizeof(GamePacket));
	}
}

void CtestDoc::OnReceivePacket(GamePacket* packet, ClientSocket* pFromSocket)
{
	switch (packet->msgType) {
		case MSG_DEAL_CARDS: {
			//我是客户端，我收到了服务器发来的牌
			m_cardListHand.RemoveAll();//清空手牌
			for (int i = 0; i < packet->deal.cardCount; i++) {
				CardData& cd = packet->deal.cards[i];
				//需要一个机制从suit/rank重新创建或找到CCard对象
				CCard* pCard = new CCard(cd.suit, cd.rank);
				//TODO:加一个加载图片函数
				pCard->m_isFaceUp = TRUE;
				m_cardListHand.AddTail(pCard);
			}
			SortCards(m_cardListHand);
			break;
		}
		case MSG_PLAY_CARD: {//服务器收到出牌请求
			if (!m_isServer) {//客户端不处理这个
				return;
			}
			//TODO:规则验证
			// 1. 检查是不是轮到 packet->playerIndex 出牌 (m_currentPlayerIndex)
			// 2. 检查出的牌 packet->play.cards 是否比当前场上牌大
			// 3. 检查牌型是否合法 (对子，顺子等)
			bool isValidPlay = TRUE;//牌型是否有效的表示，默认为TRUE
			if (isValidPlay) {
				//验证通过
				m_passCount = 0;//重置pass计数
				m_lastWinnerIndex = packet->playerIndex;//记录最后出牌的人
				m_currentPlayerIndex = (packet->playerIndex + 1) % 3;

				//广播状态更新
				GamePacket updatePacket = *packet;
				updatePacket.msgType = MSG_UPDATE_STATE;
				BroadcastPacket(&updatePacket);
			}
			break;
		}
		case MSG_PASS_TURN: {//服务器收到不要请求
			if (!m_isServer) {
				return;//客户端不处理
			}
			//TODO:验证是不是该玩家
			m_passCount++;
			m_currentPlayerIndex = (packet->playerIndex + 1) % 3;

			//广播这个pass行为 (也可以合并到UPDATE_STATE里)
			GamePacket updatePacket = *packet;
			updatePacket.msgType = MSG_UPDATE_STATE;
			BroadcastPacket(&updatePacket);

			if (m_passCount >= 2) {//两个人都不要，本轮结束
				ClearRound();
			}
			break;
		}

		case MSG_UPDATE_STATE: {//客户端和服务器收到这个
			int player = packet->playerIndex;
			CObList* pRoundList = nullptr;
			m_currentRoundCards.Lookup(player, pRoundList);

			//清空玩家上次出的牌
			pRoundList->RemoveAll();

			if (packet->play.cardCount > 0) {//出牌更新
				//从玩家手中移除这些牌，并且加入出牌链表
				for (int i = 0; i < packet->play.cardCount; i++) {
					//TODO:实现FindAndRemoveCardFromHand
					CCard* pCard = FindAndRemoveCardFromHand(player, packet->play.cards[i]);
					if (pCard) {
						pRoundList->AddTail(pCard);
					}
				}
			}
			// 如果 cardCount == 0，说明是Pass，上面清空列表就够了
			m_currentPlayerIndex = (player + 1) % 3;
			break;
		}
		case MSG_ROUND_CLEAR: // 所有客户端和服务器收到
		{
			// 清理本地的本轮出牌数据
			for (int i = 0; i < 3; ++i)
			{
				CObList* pList = nullptr;
				if (m_currentRoundCards.Lookup(i, pList)) pList->RemoveAll();
			}
			m_currentPlayerIndex = packet->roundInfo.nextPlayerIndex;
			AfxMessageBox(_T("本轮结束，轮到玩家 %d 出牌"), m_currentPlayerIndex);
			break;
		}
	}
	UpdateAllViews(NULL);
}
//清理回合的函数，仅在服务器调用
void CtestDoc::ClearRound()
{
	if (!m_isServer) return;
	//将本轮的牌移入总的弃牌堆 (可选，也可以直接删除)
	for (int i = 0; i < 3; i++) {
		CObList* pList = nullptr;
		if (m_currentRoundCards.Lookup(i, pList)) {
			//m_playedCardList.AddTail(pList);
			pList->RemoveAll();// 清空，但不删除CCard对象???
		}
	}
	m_passCount = 0;
	m_currentPlayerIndex = m_lastWinnerIndex;
	GamePacket clearPacket;
	clearPacket.msgType = MSG_ROUND_CLEAR;
	clearPacket.roundInfo.nextPlayerIndex = m_currentPlayerIndex;
	BroadcastPacket(&clearPacket);
}
