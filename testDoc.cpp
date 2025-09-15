
// testDoc.cpp: CtestDoc 类的实现
//

#include "pch.h"
#include "framework.h"
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

<<<<<<< Updated upstream
CtestDoc::CtestDoc() noexcept
=======
CtestDoc::CtestDoc() noexcept:m_serverSocket(this)
,m_clientSocket(this)
,m_isServer(FALSE)
,m_passCount(0)
,m_lastWinnerIndex(100)
,playerIndex(-1)
,m_connectedClients(0)
, m_pSetupDlg(nullptr)
,m_bGameReady(FALSE)
, m_nInitialHandsReceived(0)
>>>>>>> Stashed changes
{
	// TODO: 在此添加一次性构造代码
	StrName = _T("This is a test!");
}

CtestDoc::~CtestDoc()
{
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
<<<<<<< Updated upstream
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
	ShuffleDeck(m_leftPlayerHand);
	ShuffleDeck(m_rightPlayerHand);
	ShuffleDeck(m_cardListHand);
=======
>>>>>>> Stashed changes
}

void CtestDoc::ShuffleDeck(CObList& cardList)
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

void CtestDoc::DealCardsAndBroadcast()
{
	std::random_device rd;//洗牌
	std::mt19937 g(rd());
	std::shuffle(tempDeck.begin(), tempDeck.end() - 1, g);
	int i = 0;
	for (int j = 0; j < 51; j++) {
		if (i == 0) {
			m_leftPlayerHand.AddTail(tempDeck[j]);
		}
		else if (i == 1) {
			m_rightPlayerHand.AddTail(tempDeck[j]);
		}
		else {
			m_cardListHand.AddTail(tempDeck[j]);
		}
		i++;
		i %= 3;
	}
	m_cardListAtTop.AddTail(tempDeck[51]);
	m_cardListAtTop.AddTail(tempDeck[52]);
	m_cardListAtTop.AddTail(tempDeck[53]);
	SortCards(m_leftPlayerHand);
	SortCards(m_cardListHand);
	SortCards(m_rightPlayerHand);
	//洗牌完成，开始发牌
	// 发送玩家0的手牌
	SendCardsPacket(m_leftPlayerHand, 0);

	// 发送玩家1的手牌
	SendCardsPacket(m_rightPlayerHand, 1);

	// 发送玩家2的手牌
	SendCardsPacket(m_cardListHand, 2);

	// 这里用 -1 代表底牌。
	SendCardsPacket(m_cardListAtTop, -1);


}

void CtestDoc::SendCardsPacket(CObList& cardList, int forPlayerIndex)
{
	GamePacket packet;
	packet.msgType = MSG_DEAL_CARDS;
	packet.playerIndex = forPlayerIndex; // 关键！指明这是谁的牌
	packet.deal.cardCount = 0; // 先清零
	POSITION pos = cardList.GetHeadPosition();
	while (pos)
	{
		CCard* pCard = (CCard*)cardList.GetNext(pos);

		// 复制牌的纯数据
		packet.deal.cards[packet.deal.cardCount].suit = pCard->m_suit;
		packet.deal.cards[packet.deal.cardCount].rank = pCard->m_rank;
		packet.deal.cardCount++;
	}

	// 广播这个包含了一组牌的数据包
	BroadcastPacket(&packet);
}

void CtestDoc::CleanAllList()
{
	CleanOneList(m_playedCardList);
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
<<<<<<< Updated upstream
=======
CCard* CtestDoc::FindAndRemoveCardFromHand(int& playerIndex1, CardData& cards)
{
	CObList* pHandList = nullptr;
	switch ((playerIndex1 + 2 - playerIndex) % 3) {
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
// 新增：网络初始化函数
BOOL CtestDoc::InitializeNetwork(BOOL isServer, CString ipAddress,UINT port)
{
	m_isServer = isServer;
	if (m_isServer)
	{
		playerIndex = 2; // 服务器固定是玩家2 (地主位)
		m_serverSocket.Create(port);
		if (m_serverSocket.Listen() == FALSE)
		{
			AfxMessageBox(_T("启动服务器失败!"));
			m_serverSocket.Close();
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		m_clientSocket.Create();
		if (m_clientSocket.Connect(ipAddress, port) == FALSE)
		{
			// 这里可以显示一个“加载中/连接中”的非模态对话框
			AfxMessageBox(_T("连接服务器失败，请检查IP地址或服务器状态。"));
			m_clientSocket.Close();
			return FALSE;
		}
		return TRUE;
	}
}
void CtestDoc::SetSetupDialog(Login* pDlg)
{
	m_pSetupDlg = pDlg;
}
void CtestDoc::OnClientConnected(ClientSocket* pClient)
{
	if (!m_isServer) return;
	if (m_connectedClients < 2)
	{
		// 分配 playerIndex (0 和 1)
		int newPlayerIndex = m_connectedClients;
		m_connectedClients++;

		// 发送分配消息给这个新客户端
		GamePacket packet;
		packet.msgType = MSG_PLAYER_ASSIGN;
		packet.playerAssign.assignedPlayerIndex = newPlayerIndex;
		pClient->Send(&packet, sizeof(GamePacket));

		// << 关键 >> 回调对话框更新UI
		if (m_pSetupDlg)
		{
			m_pSetupDlg->UpdateStatus(m_connectedClients);
		}
		// 如果人齐了，就开始游戏发牌
		if (m_connectedClients == 2)
		{

			// 人满了，通知对话框可以开始游戏了
			if (m_pSetupDlg)
			{
				m_pSetupDlg->EnableStartButton();
			}
			// 注意：发牌逻辑要等到服务器点击“开始游戏”后再执行，
			// 所以我们把 DealCardsAndBroadcast() 从这里移走。
		}
	}
	else
	{
		// 游戏已满，拒绝连接
		pClient->Close();
		delete pClient;
	}
}
BOOL CtestDoc::FindCard(const CardData& data, CCard*& pCard)
{
	CMap<int, int, CCard*, CCard*> cardMap;
	for (size_t i = 0; i < tempDeck.size(); ++i)
	{
		CCard* ppCard = tempDeck[i];
		int cardKey = ppCard->m_suit * 100 + ppCard->m_rank;
		cardMap.SetAt(cardKey, ppCard);
	}
	int cardKey = data.suit * 100 + data.rank;
	// 使用 Map 快速找到对应的 CCard* 对象
	if (cardMap.Lookup(cardKey, pCard)) {
		return TRUE;
	}
	return FALSE;
}
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======

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

void CtestDoc::BroadcastPacket(GamePacket* packet, ClientSocket* pExcludeClient)
{
	if (!m_isServer) return;

	POSITION pos = m_serverSocket.m_clientList.GetHeadPosition();
	while (pos)
	{
		ClientSocket* pClient = (ClientSocket*)m_serverSocket.m_clientList.GetNext(pos);
		if (pClient != pExcludeClient)
		{
			pClient->Send(packet, sizeof(GamePacket));
		}
	}
	// 服务器自己也要处理这个消息
	OnReceivePacket(packet, nullptr);
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
	if (m_isServer) {
		switch (packet->msgType) {
		case MSG_PLAY_CARD: {//服务器收到出牌请求
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
		}
	}
	switch (packet->msgType) {
	case MSG_PLAYER_ASSIGN: {
		playerIndex = packet->playerAssign.assignedPlayerIndex;
		CString msg;
		msg.Format(_T("连接成功！你是玩家 %d"), playerIndex);
		AfxMessageBox(msg);
		break;
	}
		case MSG_DEAL_CARDS: {
			if (!m_isServer) {
				m_nInitialHandsReceived++;
				// --- 准备阶段：清空旧数据
				if (m_nInitialHandsReceived == 1) {
					m_cardListHand.RemoveAll();
					m_leftPlayerHand.RemoveAll();
					m_rightPlayerHand.RemoveAll();
					m_cardListAtTop.RemoveAll();
				}
				// --- 核心逻辑：高效查找并分配卡牌 ---
				int i = packet->playerIndex;
				// 遍历当前玩家(i)的所有手牌
				for (int j = 0; j < packet->deal.cardCount; j++)
				{
					// 从 packet 中获取一张牌的纯数据 (花色和点数)
					const CardData& data = packet->deal.cards[j];
					CCard* pCard = nullptr;
					if (FindCard(data, pCard))
					{
						// 判断这张牌属于哪个玩家列表
						if (i == playerIndex)
						{
							// A. 如果是自己的牌 (服务器的玩家索引 i == 我的玩家索引)
							m_cardListHand.AddTail(pCard);
						}
						else if (i == -1) {
							m_cardListAtTop.AddTail(pCard);//加底牌
						}
						else if ((i + 2 - playerIndex) % 3 == 1)
						{
							// B. 如果是右边玩家的牌
							m_rightPlayerHand.AddTail(pCard);
						}
						else
						{
							// C. 如果是左边玩家的牌
							m_leftPlayerHand.AddTail(pCard);
						}
					}
				}
			}
			if (m_nInitialHandsReceived == 4)
			{
				// 检查对话框指针是否仍然有效
				if (m_pSetupDlg)
				{
					// 使用PostMessage发送关闭命令，这是线程安全的
					::PostMessage(m_pSetupDlg->GetSafeHwnd(), WM_CLOSE_LOGIN, 0, 0);
				}
			}
			// 5. 通知视图(View)重绘，显示新的手牌
			UpdateAllViews(NULL);
			break;
		}

		case MSG_UPDATE_STATE: {//客户端和服务器收到这个
			// 完全信任服务器的数据，更新本地的出牌列表和手牌
			// 之前的 FindAndRemoveCardFromHand 逻辑在这里完美适用
			int player = packet->playerIndex;
			CObList* pRoundList = nullptr;
			m_currentRoundCards.Lookup(player, pRoundList);
			pRoundList->RemoveAll();

			if (packet->play.cardCount > 0)
			{
				const int overlapX = m_cardWidth/4; // 水平出牌时的重叠宽度
				const int overlapY = m_cardHeight/4; // 垂直出牌时的重叠高度
				// 定义三个出牌区域的基准点 (x, y)
				// (这些坐标需要您根据自己的窗口布局进行精确调整)
				CPoint myPlayPoint(2000 / 2, 400);      // 我（下方）的出牌点
				CPoint leftPlayPoint(400, 250);         // 左侧玩家的出牌点
				CPoint rightPlayPoint(2000 - 400, 250); // 右侧玩家的出牌点

				CPoint targetPoint;
				bool isHorizontal = false; // 标记是水平放置还是垂直放置

				if (player == playerIndex) {
					// 是“我”出的牌，放在下方，水平排列
					targetPoint = myPlayPoint;
					isHorizontal = true;
				}
				else if ((player + 2 - playerIndex) % 3==0) {
					// 是“左边”的玩家出的牌，放在左边，垂直排列
					targetPoint = leftPlayPoint;
					isHorizontal = false;
				}
				else { // playerIndex == rightPlayerIndex
					// 是“右边”的玩家出的牌，放在右边，垂直排列
					targetPoint = rightPlayPoint;
					isHorizontal = false;
				}
				for (int i = 0; i < packet->play.cardCount; ++i)
				{
					CCard* pCard = FindAndRemoveCardFromHand(player, packet->play.cards[i]);
					if (pCard) {
						// a. 设置为正面显示
						pCard->m_isFaceUp = TRUE;

						// b. 【核心改动】根据不同的布局计算每张牌的最终Rect
						int cardLeft, cardTop;
						if (isHorizontal) {
							// 水平居中排列
							int totalWidth = m_cardWidth + (packet->play.cardCount - 1) * overlapX;
							cardLeft = targetPoint.x - (totalWidth / 2) + i * overlapX;
							cardTop = targetPoint.y;
						}
						else {
							// 垂直排列
							cardLeft = targetPoint.x - (m_cardWidth / 2); // 水平方向上居中对齐
							cardTop = targetPoint.y + i * overlapY;
						}
						pCard->m_rect.SetRect(cardLeft, cardTop, cardLeft + m_cardWidth, cardTop + m_cardHeight);

						// c. 将更新好的牌加入到本轮出牌列表
						pRoundList->AddTail(pCard);
					}
				}
			}
			m_currentPlayerIndex = (player + 1) % 3;
			UpdateAllViews(NULL);
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
			break;
		}
		case MSG_GAME_OVER:
		{
			CString strWinner;
			if (packet->gameOver.winnerIndex == playerIndex)
			{
				strWinner = "你赢了！";
			}
			else
			{
				strWinner.Format(_T("游戏结束，玩家 %d 胜利！"), packet->gameOver.winnerIndex);
			}
			AfxMessageBox(strWinner);
			// 这里可以做重置游戏的操作
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
>>>>>>> Stashed changes
