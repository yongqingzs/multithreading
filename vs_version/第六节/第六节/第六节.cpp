// ������.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <list>
#include <mutex>

using namespace std;

class A
{
public:
	//���յ�����Ϣ���������뵽һ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;

			//std::lock_guard<std::mutex> sbguard1(my_mutex1);
			my_mutex1.lock();
			//Ҫ��lock������������unique_lock��std::adopt_lock����
			std::unique_lock<std::mutex> sbguard1(my_mutex1, std::adopt_lock);

			msgRecvQueue.push_back(i);
			//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			//......
			//����������룻
		}
	}

	bool outMsgLULProc(int &command)
	{
		//std::lock_guard<std::mutex> sbguard1(my_mutex1);
		std::unique_lock<std::mutex> sbguard1(my_mutex1);

		if (!msgRecvQueue.empty())
		{
			//��Ϣ��Ϊ��
			command = msgRecvQueue.front();
			//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();
			//�Ƴ���һ��Ԫ�أ��������أ�
			return true;
		}
		return false;
	}

	//�����ݴ���Ϣ������ȡ�����̣߳�
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << command << endl;
				//���Կ��ǽ���������ݣ�����
				//.....
			}
			else
			{
				//��Ϣ����Ϊ��
				cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
			}
		}
		cout << endl;
	}

private:
	std::list<int> msgRecvQueue;
	//������ר�����ڴ�����Ҹ����Ƿ��͹��������
	std::mutex my_mutex1;
	//����һ����������һ����ͷ��
};

int main01()
{
	//һ��unique_lockȡ��lock_guard
	//unique_lock�Ǹ���ģ�壬�����У�һ��lock_guard���Ƽ�ʹ�ã���lock_grardȡ����mutex��lock()��unlock()��
	//unique_lock��lock_guard���ܶࣻЧ���ϲ�һ�㣬�ڴ�ռ�ö�һ�㡣
	//����unique_lock�ĵڶ�������
	//lock_guard���Դ��ڶ���������

	//std::lock_guard<std::mutex> sbguard1(my_mutex1, std::adopt_lock);
	//adopt_lock������ã�
	//��2.1��std::adopt_lock����ʾ����������Ѿ���lock�ˣ������������������ǰlock�ˣ�����ᱨ�쳣��
	//std::adopt_lock��ǵ�Ч�����ǡ�������÷� �߳��Ѿ�ӵ���˻��������Ȩ���Ѿ�lock()�ɹ��ˣ���
	//֪ͨlock_guard����Ҫ�ڹ��캯����lock����������ˣ�
	//unique_lockҲ���Դ�std::adopt_lock��ǣ�������ͬ�����ǲ�ϣ����unique_lock()�Ĺ��캯����lock���mutex��
	//�����adopt_lock��ǰ���ǣ�����Ҫ�Լ��Ȱ�mutex��lock�ϣ�

	A myobja;
	std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);
	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();


	return 0;
}




class AA
{
public:
	//���յ�����Ϣ���������뵽һ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;

			//std::lock_guard<std::mutex> sbguard1(my_mutex1);

			//my_mutex1.lock();
			//Ҫ��lock������������unique_lock��std::adopt_lock����
			//std::unique_lock<std::mutex> sbguard1(my_mutex1, std::adopt_lock);

			std::unique_lock<std::mutex> sbguard1(my_mutex1, std::try_to_lock);
			if (sbguard1.owns_lock())
			{
				//�õ�����
				msgRecvQueue.push_back(i);
				//�����������i�������յ��������ֱ��Ū����Ϣ�����������
				//......
				//����������룻
			}
			else
			{
				//û�õ���
				cout << "inMsgRecvQueue()ִ�У���û�õ�����ֻ�ܸɵ�����" << i << endl;
			}
		}
	}

	bool outMsgLULProc(int &command)
	{
		//std::lock_guard<std::mutex> sbguard1(my_mutex1);
		std::unique_lock<std::mutex> sbguard1(my_mutex1);

		std::chrono::milliseconds durn(20000);
		//1�� = 1000���룬����20000���� = 20��
		std::this_thread::sleep_for(durn);
		//��Ϣһ����ʱ��

		if (!msgRecvQueue.empty())
		{
			//��Ϣ��Ϊ��
			command = msgRecvQueue.front();
			//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();
			//�Ƴ���һ��Ԫ�أ��������أ�
			return true;
		}
		return false;
	}

	//�����ݴ���Ϣ������ȡ�����̣߳�
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << command << endl;
				//���Կ��ǽ���������ݣ�����
				//.....
			}
			else
			{
				//��Ϣ����Ϊ��
				cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
			}
		}
		cout << endl;
	}

private:
	std::list<int> msgRecvQueue;
	//������ר�����ڴ�����Ҹ����Ƿ��͹��������
	std::mutex my_mutex1;
	//����һ����������һ����ͷ��
};

int main02()
{
	//��2.2��std::try_to_lock
	//���ǻ᳢��mutex��lock()ȥ����mutex�������û�������ɹ�����Ҳ���������أ����������������
	//�����try_to_lock��ǰ�������Լ�������ȥlock��

	AA myobja;
	std::thread myOutnMsgObj(&AA::outMsgRecvQueue, &myobja);
	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myInMsgObj(&AA::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();

	return 0;
}




class AAA
{
public:
	std::unique_lock<std::mutex> rtn_unique_lock()
	{
		std::unique_lock<std::mutex> tmpguard(my_mutex1);
		return tmpguard;
		//�Ӻ�������һ���ֲ���unique_lock�����ǿ��Եġ�����ʮ�Ľڽ�����ƶ����캯����
		//�������־ֲ�����tmpguard�ᵼ��ϵͳ������ʱunique_lock���󣬲�����unique_lock���ƶ����캯��
	}

	//���յ�����Ϣ���������뵽һ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;

			//std::unique_lock<std::mutex> sbguard1(my_mutex1, std::defer_lock);
			//û�м�����my_mutex1
			//sbguard1.lock();
			//���ǲ����Լ�unlock
			//���������
			//sbguard1.unlock();
			//����һЩ�ǹ������
			//sbguard1.lock();
			//���������

			//std::unique_lock<std::mutex> sbguard1(my_mutex1);
			//std::unique_lock<std::mutex> sbguard2(my_mutex1);
			//std::unique_lock<std::mutex> sbguard2(sbguard1);
			//��������Ȩ�ǷǷ���
			//std::unique_lock<std::mutex> sbguard2(std::move(sbguard1));
			//�ƶ����壬�����൱��sbguard2��my_mutex1�󶨵�һ����
			//����sbguard1ָ��գ�sbguard2ָ����my_mutex1


			std::unique_lock<std::mutex> sbguard1 = rtn_unique_lock();


			//std::mutex *ptx = sbguard1.release();
			//�������������Լ��������my_mutex1��

			//if (sbguard1.try_lock() == true)
			//����true��ʾ�õ�����
			//{
			msgRecvQueue.push_back(i);
			//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			//  //......
			//  //����������룻
			//}
			//else
			//{
			//  //û�õ���
			//  cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
			//}

			//sbguard1.unlock();
			//�������㣬��Ҳ����
			//ptx->unlock();
			//�Լ�����mutex��unlock

			//�����ĺ͹����޹صĴ���
			//....
			//....
		}
	}

	bool outMsgLULProc(int &command)
	{
		//std::lock_guard<std::mutex> sbguard1(my_mutex1);
		std::unique_lock<std::mutex> sbguard1(my_mutex1);

		//std::chrono::milliseconds durn(20000);
		//1�� = 1000���룬����20000���� = 20��
		//std::this_thread::sleep_for(durn);
		//��Ϣһ����ʱ��

		if (!msgRecvQueue.empty())
		{
			//��Ϣ��Ϊ��
			command = msgRecvQueue.front();
			//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();
			//�Ƴ���һ��Ԫ�أ��������أ�
			return true;
		}
		return false;
	}

	//�����ݴ���Ϣ������ȡ�����̣߳�
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << command << endl;
				//���Կ��ǽ���������ݣ�����
				//.....
			}
			else
			{
				//��Ϣ����Ϊ��
				cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
			}
		}
		cout << endl;
	}

private:
	std::list<int> msgRecvQueue;
	//������ר�����ڴ�����Ҹ����Ƿ��͹��������
	std::mutex my_mutex1;
	//����һ����������һ����ͷ��
};

int main()
{
	//��2.3��std::defer_lock
	//�����defer_lock��ǰ���� �㲻���Լ���lock������ᱨ�쳣��
	//defer_lock����˼ ���� ��û�и�mutex��������ʼ����һ��û�м�����mutex��
	//���ǽ���defer_lock�Ļ��⣬������һЩunique_lock����Ҫ��Ա����

	//����unique_lock�ĳ�Ա����
	//��3.1��lock()������
	//��3.2��unlock()������
	//��3.3��try_lock(),���Ը�����������������ò��������򷵻�false������õ�����������true����������������ģ�
	//��3.4��release()���������������mutex����ָ�룬���ͷ�����Ȩ��Ҳ������Ŷ�����unique_lock��mutex�����й�ϵ��
	//�ϸ�����unlock()��release()�����𣬲�Ҫ������
	//���ԭ��mutex�����ڼ���״̬���������νӹܹ����������������rlease���ص���ԭʼmutex��ָ�룩
	//Ϊʲô��ʱ����Ҫunlock()����Ϊ��lock��ס�Ĵ����Խ�٣�ִ��Խ�죬������������Ч��Խ�ߡ�
	//����Ҳ����ͷ��ס�Ĵ������ ��Ϊ���� ���ȣ�����һ���ô�ϸ��������
	//a����ס�Ĵ����٣�������Ƚ�ϸ��ִ��Ч�ʸߣ�
	//b����ס�Ĵ���࣬���Ƚд֣���ִ��Ч�ʾ͵ͣ�
	//Ҫѧ�ᾡ��ѡ��������ȵĴ�����б���������̫ϸ������©���������ݵı���������̫�֣�Ӱ��Ч�ʡ�
	//ѡ����ʵ����ȣ��Ǹ߼�����Ա��������ʵ�������֣�

	//�ģ�unique_lock����Ȩ�Ĵ��� mutex
	//std::unique_lock<std::mutex> sbguard1(my_mutex1);
	//����Ȩ����
	//sbguard1ӵ��my_mutex1������Ȩ
	//sbguard1���԰��Լ���mutex(my_mutex1)������Ȩת�Ƹ�������unique_lock����
	//���ԣ�unique_lock�������mutex������Ȩ������ ����ת�ƣ����ǲ��ܸ��ơ�	5��6, 7�ڽ�����ָ��unique_ptr

	//a��std::move
	//b��return std::unique_lock<std::mutex>

	AAA myobja;
	std::thread myOutnMsgObj(&AAA::outMsgRecvQueue, &myobja);
	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myInMsgObj(&AAA::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();

	return 0;
}