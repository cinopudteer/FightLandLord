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
	MSG_GAME_OVER,//游戏结束
	MSG_PASS_TURN,//玩家选择“不要”消息
	MSG_ROUND_CLEAR,//服务器通知清空本轮出牌
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
	};
	struct {
		int nextPlayerIndex;//通知下一个出牌者是谁
	}roundInfo;
    // 修正构造函数初始化列表中的拼写错误，将 magType 改为 msgType
    GamePacket() : msgType(MSG_INVALID) {
        deal.cardCount = 0;
        play.cardCount = 0;
    }
};



class NetProtocol
{
};

