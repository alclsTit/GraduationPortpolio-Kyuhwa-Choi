#pragma once

#define SERVERPORT 9000
#define MAX_BUF_SIZE 256
#define MAX_PLAYER 3

#define DISCONNECTED 0
#define CONNECTED 1

#define MAX_FILE_BUFFER_SIZE 8

//추가
//레벨업 보상, 마법레벨

using Packet = unsigned char;

enum PACKET_PROTOCOL_TYPE
{
	// 캐릭터 정보 초기화
	INIT_CLIENT,
	INIT_OTHER_CLIENT,

	// 캐릭터 좌표 및 방향전환
	CHANGED_PLAYER_POSITION,
	CHANGED_PLAYER_DIRECTION,

	//연결끊어짐
	PLAYER_DISCONNECT
};

enum PLAYER_CLASS
{
	Luna,
	Frank,
	James,
	Lancy,
	Nau
};

enum OPTYPE {OP_SEND, OP_RECV};

struct OverlappedEx
{
	WSAOVERLAPPED over;
	WSABUF wsabuf;
	unsigned char IOCP_buf[MAX_BUF_SIZE];
	OPTYPE event_type;
	int event_target;
};

struct Position
{
	float		x{ 100 };
	float		y{ 100 };
	float		z{ 50 };
};

struct Player_Status
{
	int			attack{ 50 };
	int			defend{ 50 };
    int			speed{ 10 };
};

struct Player_LoginDB
{
	unsigned int mem_ID{ 0 };
	char name[MAX_FILE_BUFFER_SIZE];
	char password[MAX_FILE_BUFFER_SIZE];
};


struct Player_Info
{
	//PLAYER_CLASS player_class;
	int				origin_hp{ 300 };
	int				cur_hp{ 300 };
	int				origin_mp{ 100 };
	int				cur_mp{ 100 };
	unsigned char	level{ 1 };
	unsigned short	exp{ 0 };
	Player_Status	player_status;
};

struct Player_Data
{
	unsigned int	ID{ 0 };
	Position		Pos;
	Player_Info		UserInfo;
	char			Dir

};

typedef struct Server_To_Client_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + 2 * sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_ClientInit;


typedef struct Server_To_Client_Player_TO_Other_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + 2 * sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_OtherClientInit;

typedef struct Server_To_Client_Player_Disconnected_Info
{
	unsigned char pack_size = 2 * sizeof(unsigned char) + sizeof(unsigned int);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT;
	unsigned int id;

}STC_Disconnected;


typedef struct Server_To_Client_Player_Position_Changed
{
	unsigned char packet_size = sizeof(unsigned int) + sizeof(Position) + 2 * sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
	unsigned int id;
	Position pos;
}STC_ChangedPos;