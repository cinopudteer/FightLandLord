#pragma once
#include "CCard.h"
//通信协议

//服务器广播消息类型
enum MessageType {
	MSG_INVALID = 0,
	MSG_DEAL_CARDS,//服务器发牌
	MSG_PLAY_CARD,//服务器出牌
	MSG_UPDATE_STATE,//服务器广播游戏状态更新
	MSG_WHOSE_TURN,//轮到谁出牌
	MSG_PASS_TURN,//玩家选择“不要”消息
	MSG_ROUND_CLEAR,//服务器通知清空本轮出牌
	MSG_PLAYER_ASSIGN,  //服务器告诉客户端它的playerIndex
	MSG_GAME_OVER,      //服务器宣布游戏结束和胜利者
};

struct CardData {
	int suit;
	int rank;
};

#define MAX_CARDS_TO_DEAL 20
//网络通信包结构
struct GamePacket {
	MessageType msgType;
	int playerIndex;
	union {
		struct {
			int cardCount;
			CardData cards[MAX_CARDS_TO_DEAL];
		}deal;
		struct {
			int cardCount;
			CardData cards[MAX_CARDS_TO_DEAL];
		}play;
		//用于 MSG_PLAYER_ASSIGN
		struct {
			int assignedPlayerIndex;
		} playerAssign;

		//用于 MSG_GAME_OVER
		struct {
			int winnerIndex;
		} gameOver;
		struct {
			int nextPlayerIndex;//通知下一个出牌者是谁
		}roundInfo;
	};
};



class NetProtocol
{
};

