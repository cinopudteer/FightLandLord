#pragma once
#include "CCard.h"
//ͨ��Э��

//�������㲥��Ϣ����
enum MessageType {
	MSG_INVALID = 0,
	MSG_DEAL_CARDS,//����������
	MSG_PLAY_CARD,//����������
	MSG_UPDATE_STATE,//�������㲥��Ϸ״̬����
	MSG_WHOSE_TURN,//�ֵ�˭����
	MSG_GAME_OVER,//��Ϸ����
	MSG_PASS_TURN,//���ѡ�񡰲�Ҫ����Ϣ
	MSG_ROUND_CLEAR,//������֪ͨ��ձ��ֳ���
};

struct CardData {
	int suit;
	int rank;
};

#define MAX_CARDS_TO_DEAL 20
//����ͨ�Ű��ṹ
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
		int nextPlayerIndex;//֪ͨ��һ����������˭
	}roundInfo;
    // �������캯����ʼ���б��е�ƴд���󣬽� magType ��Ϊ msgType
    GamePacket() : msgType(MSG_INVALID) {
        deal.cardCount = 0;
        play.cardCount = 0;
    }
};



class NetProtocol
{
};

