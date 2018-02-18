#include "stdafx.h"
#include "UserDatabase.h"


UserDatabase::UserDatabase()
{

}

void UserDatabase::FILE_INIT(const string & fname)
{
	ifstream fin(fname, ios::app);

	if (!fin.is_open())
	{
		cout << "읽을 수 없습니다." << endl;
		exit(1);
	}
	else
	{
		cout << "Input ID & Password " << endl;

		unsigned memid{ 0 };
		char id[MAX_FILE_BUFFER_SIZE]{ "guest" };
		char pw[MAX_FILE_BUFFER_SIZE]{ "guest" };

		while (fin >> memid >> id >> pw)
		{
			Player_LoginDB login_data;
			login_data.mem_ID = memid;
			memcpy(login_data.name, id, sizeof(login_data.name));
			memcpy(login_data.password, pw, sizeof(login_data.password));


			//cout << memid << " +++ " << id << " +++ " << pw << " +++ " << endl;
		}
	}

	fin.close();
}

void UserDatabase::FILE_UPDATE()
{

}

void UserDatabase::DB_Init()
{

}

void UserDatabase::DB_Update()
{
}


UserDatabase::~UserDatabase()
{

}
