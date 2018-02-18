#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <iostream>
#include <chrono>
#include <queue>

using namespace std;
using namespace chrono;
using namespace this_thread;

queue<int> g_que;
mutex g_mtx;
condition_variable g_quevar;

void Print(const int& n, const int& v);

//val �ð��� ���̸� �ΰ� ������ ������ ť�� val + i �����͸� �������
void provider(int val)
{
	for (auto i = 0; i < 10; ++i)
	{
		{
			lock_guard<mutex> lg(g_mtx);	//���ȹ��
			g_que.push(val + i);
		}									// �������

		g_quevar.notify_one();				//�ϳ��� �����带 ����

		sleep_for(milliseconds(val));		//val�ð� ���� ���(�и�������)
	}
}

// ���� ������
void consumer(int num)
{	
	while (true)
	{
		int val;
		{
			unique_lock<mutex> ul(g_mtx);
			g_quevar.wait(ul, []() { return !g_que.empty(); });		// ť�� �����Ͱ� �ϳ��� �� ������ ��ٸ�
			val = g_que.front();
			g_que.pop();	
		}															// �������

		Print(num,val);
	}

}

void Print(const int& n, const int& v)
{
	unique_lock<mutex> ul(g_mtx);
	if (ul.owns_lock())
	{
		cout << "consumer :" << n << " : " << v << endl;
	}
	else
	{
		cout << "���ȹ�����" << endl;
	}
}

int main()
{
	auto p1 = async(launch::async, provider, 100);
	auto p2 = async(launch::async, provider, 300);
	auto p3 = async(launch::async, provider, 500);

	auto c1 = async(launch::async, consumer, 1);
	auto c2 = async(launch::async, consumer, 2);
	
}