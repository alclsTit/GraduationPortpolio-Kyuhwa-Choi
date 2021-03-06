// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#include "targetver.h"
#include <windows.h>

#include <winsock.h>
#include <WinSock2.h>

#include <thread>
#include <mutex>

#include <chrono>
#include <vector>
#include <iostream>

extern "C" {
#include "LUA\lua.h"
#include "LUA\lauxlib.h"
#include "LUA\lualib.h"
}


#include "../boost_1_66_0/boost/asio.hpp"
#include "../boost_1_66_0/boost/bind.hpp"

using namespace std;
using namespace chrono;

#define MAX_PLAYER 2

#define INIT_LEFT_CHAR_POSX 60
#define INIT_RIGHT_CHAR_POSX 640
#define INIT_CHAR_POSY 400

#define MAX_COUNT 10
#define EXCUTE_COUNT 1


//클라이언트와 연동되는 protocol 헤더파일
#include "protocol.h"

#include "Player_Session.h"

vector<Player_Session*> g_clients;

struct Pos
{
	int x;
	int y;
};

#pragma pack(1)
struct CharacterInfo
{
	Pos char_pos;
	int id;
};

struct CharID
{
	int id;
};
#pragma pack()





// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
