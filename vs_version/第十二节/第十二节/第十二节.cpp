// ��ʮ����.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <list>
#include <mutex>
#include <future>
#include <windows.h>

using namespace std;

//#define __WINDOWSJQ_

//���������Զ��ͷ�windows�µ��ٽ�������ֹ����LeaveCriticalSection������������ķ�����������c++11�е�std::lock_guard<std::mutex>
class CWinLock	//��RAII�ࣨResource Acquisition is initialization�����ġ���Դ��ȡ����ʼ����
					//����������ָ�������࣬������RAII��
{
public:
	CWinLock(CRITICAL_SECTION *pCritmp)	//���캯��
	{
		m_pCritical = pCritmp;
		EnterCriticalSection(m_pCritical);
	}

	~CWinLock()	//��������
	{
		LeaveCriticalSection(m_pCritical);
	}

private:
	CRITICAL_SECTION *m_pCritical;
};

class A
{
public:
	//���յ�����Ϣ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;

#ifdef __WINDOWSJQ_
			//EnterCriticalSection(&my_winsec);	//�����ٽ�����������
			//EnterCriticalSection(&my_winsec);
			CWinLock wlock(&my_winsec);		//wlock��wlock2 ������RAII����
			CWinLock wlock2(&my_winsec);	//���ö��Ҳû���⣻
			msgRecvQueue.push_back(i);
			//LeaveCriticalSection(&my_winsec);	//�뿪�ٽ�����������
			//LeaveCriticalSection(&my_winsec);
#else
			//my_mutex.lock();
			//my_mutex.lock();	//���쳣����windows������
			std::lock_guard<std::mutex> sbguard(my_mutex);
			//std::lock_guard<std::mutex> sbguard1(my_mutex);
			msgRecvQueue.push_back(i);	//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			//my_mutex.unlock();
			//my_mutex.unlock();
#endif
		}
	}

	bool outMsgLULProc(int &command)
	{
#ifdef __WINDOWSJQ_
		EnterCriticalSection(&my_winsec);
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();	//���ص�һ��Ԫ�ص������Ԫ�ش������
			msgRecvQueue.pop_front();
			LeaveCriticalSection(&my_winsec);
			return true;
		}
		LeaveCriticalSection(&my_winsec);
#else
		my_mutex.lock();
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();	//���ص�һ��Ԫ�ص������Ԫ�ش������
			msgRecvQueue.pop_front();
			my_mutex.unlock();
			return true;
		}
		my_mutex.unlock();
#endif

		return false;
	}

	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << command << endl;
				//������Կ��Ǵ�������
				//.....
			}
			else
			{
				cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
			}
		}
		cout << "end" << endl;
	}

	A()
	{
#ifdef __WINDOWSJQ_
		InitializeCriticalSection(&my_winsec);	//���ٽ���֮ǰҪ�ȳ�ʼ��
#endif
	}


private:
	std::list<int> msgRecvQueue;	//������ר�����ڴ�����Ҹ����Ƿ��͹���������
	std::mutex my_mutex;	//����������

#ifdef __WINDOWSJQ_
	CRITICAL_SECTION my_winsec;	//windows�ֵ��ٽ������ǳ�������C++11�ֵ�mutex
#endif
};

int main01()
{
	//һ��windows�ٽ���
	//������ν����ٽ�������
	//��ͬһ���̣߳���ͬ�߳̾ͻῨס�ȴ����У�windows�еġ���ͬ�ٽ���������������ٽ����Ľ��루EnterCriticalSection�����Ա���ε���
		//����������˼���EnterCriticalSection����͵õ��ü���LeaveCriticalSection(&my_winsec);
		//����c++11�У������� ͬһ�߳���lockͬһ����������Σ������쳣

	//�����Զ���������
	//std::lock_guard<std::mutex>

	//�ģ�recursive_mutex�ݹ�Ķ�ռ������

	A myobja;
	std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);	//ע������ڶ�����������ʱ���ã����ܱ�֤�߳����õ���ͬһ������
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
	myInMsgObj.join();
	myOutnMsgObj.join();

	system("pause");
	return 0;
}




class AA
{
public:
	//���յ�����Ϣ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;

			//my_mutex.lock();
			//my_mutex.lock();	//���쳣����windows������
			std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
			testfunc1();	//���������������쳣����ֻҪlock����һ�ξͱ��쳣��

			//std::lock_guard<std::mutex> sbguard1(my_mutex);
			msgRecvQueue.push_back(i);	//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			//my_mutex.unlock();
			//my_mutex.unlock();
		}
	}

	bool outMsgLULProc(int &command)
	{
		my_mutex.lock();
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();	//���ص�һ��Ԫ�ص������Ԫ�ش������
			msgRecvQueue.pop_front();
			my_mutex.unlock();
			return true;
		}
		my_mutex.unlock();

		return false;
	}

	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << command << endl;
				//������Կ��Ǵ�������
				//.....
			}
			else
			{
				cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
			}
		}
		cout << "end" << endl;
	}

	void testfunc1()
	{
		std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
		//......�ɸ�������
		testfunc2();	//�����ˣ�������
	}

	void testfunc2()
	{
		std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
		/*std::lock_guard<std::recursive_mutex> sbguard2(my_mutex);
		std::lock_guard<std::recursive_mutex> sbguard3(my_mutex);
		std::lock_guard<std::recursive_mutex> sbguard4(my_mutex);
		std::lock_guard<std::recursive_mutex> sbguard5(my_mutex);*/


		//......�ɸ�������һЩ����
	}


private:
	std::list<int> msgRecvQueue;	//������ר�����ڴ�����Ҹ����Ƿ��͹���������
	//std::mutex my_mutex;	//����������
	std::recursive_mutex my_mutex;	//�ݹ��ռ������
};

int main02()
{
	//�ģ�recursive_mutex�ݹ�Ķ�ռ������
	//std::mutex����ռ���������Լ�lockʱ����lock���ˣ�
	//std::recursive_mutex���ݹ�Ķ�ռ������������ͬһ�̣߳�ͬһ����������α���lock()��Ч���ϱ�mutex����һЩ��
		//recursive_mutexҲ��lock��Ҳ��unlock()��
			//���Ǵ����Ƿ����Ż��ռ䡣
		//�ݹ������˵�����ƣ��ڹ�̨̫����ܱ��쳣��

	AA myobja;
	std::thread myOutnMsgObj(&AA::outMsgRecvQueue, &myobja);	//ע������ڶ�����������ʱ���ã����ܱ�֤�߳����õ���ͬһ������
	std::thread myInMsgObj(&AA::inMsgRecvQueue, &myobja);
	myInMsgObj.join();
	myOutnMsgObj.join();

	system("pause");
	return 0;
}





class AAA
{
public:
	//���յ�����Ϣ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;

			std::chrono::milliseconds timeout(100);	//�ȴ�100����
			//if (my_mutex.try_lock_for(timeout))	//�ȴ�100���������� ��ȡ��
			if (my_mutex.try_lock_until(chrono::steady_clock::now() + timeout))
			{
				//����100����֮���õ�����
				msgRecvQueue.push_back(i);	//�����������i�������յ��������ֱ��Ū����Ϣ�����������
				my_mutex.unlock();	//������Ҫ������
			}
			else
			{
				//���û�õ���ͷ
				std::chrono::milliseconds sleeptime(100);
				std::this_thread::sleep_for(sleeptime);
			}
		}
	}

	bool outMsgLULProc(int &command)
	{
		my_mutex.lock();

		std::chrono::milliseconds sleeptime(200);
		std::this_thread::sleep_for(sleeptime);

		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();	//���ص�һ��Ԫ�ص������Ԫ�ش������
			msgRecvQueue.pop_front();
			my_mutex.unlock();
			return true;
		}
		my_mutex.unlock();

		return false;
	}

	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << command << endl;
				//������Կ��Ǵ�������
				//.....
			}
			else
			{
				cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
			}
		}
		cout << "end" << endl;
	}

private:
	std::list<int> msgRecvQueue;	//������ר�����ڴ�����Ҹ����Ƿ��͹���������
	std::timed_mutex my_mutex;	//����ʱ���ܵĶ�ռ��������
};

int main()
{
	//�壺����ʱ�Ļ�����std::timed_mutex��std::recursive_timed_mutex
	//std::timed_mutex���Ǵ���ʱ���ܵĶ�ռ��������
		//try_lock_for()��������һ��ʱ�䣬�ǵȴ�һ��ʱ�䡣������õ����������ߵȴ�����ʱ��δ�õ���������������
		//try_lock_until()��������һ��δ����ʱ��㣬�����δ����ʱ��û����ʱ����ڣ�������õ���������ô����������
																				//���ʱ�䵽�ˣ�û�õ�������������Ҳ��������
	//std::recursive_timed_mutex������ʱ���ܵĵݹ��ռ������������ͬһ�̶߳�λ�ȡ�����������

	AAA myobja;
	std::thread myOutnMsgObj(&AAA::outMsgRecvQueue, &myobja);	//ע������ڶ�����������ʱ���ã����ܱ�֤�߳����õ���ͬһ������
	std::thread myInMsgObj(&AAA::inMsgRecvQueue, &myobja);
	myInMsgObj.join();
	myOutnMsgObj.join();

	system("pause");
	return 0;
}