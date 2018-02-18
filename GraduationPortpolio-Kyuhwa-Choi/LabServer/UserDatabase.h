#pragma once
#include <fstream>
#include <tchar.h>

#define MAX_FILE_BUFFER_SIZE 8


class UserDatabase
{
public:
	UserDatabase();

	void FILE_INIT(const string& fname);
	void FILE_UPDATE();

	void DB_Init();
	void DB_Update() ;
	~UserDatabase();
};

