// �����.cpp : Defines the entry point for the console application.
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

			//my_mutex.lock();
			{
				std::lock_guard<std::mutex> sbguard(my_mutex);
				msgRecvQueue.push_back(i);
				//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			}
			//my_mutex.unlock();
			//......
			//����������룻
		}
	}

	bool outMsgLULProc(int &command)
	{
		std::lock_guard<std::mutex> sbguard(my_mutex);
		//sbguard����ʦ�����Ķ�����
		//lock_guard���캯����ִ����mutext::lock()��
		//lock_guard����������ִ����mutext::unlock()��
		//my_mutex.lock();
		if (!msgRecvQueue.empty())
		{
			//��Ϣ��Ϊ��
			command = msgRecvQueue.front();
			//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();
			//�Ƴ���һ��Ԫ�أ��������أ�
			//my_mutex.unlock();
			return true;
		}
		//my_mutex.unlock();
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
	std::mutex my_mutex;
	//����һ����������һ����ͷ��
};

int main01()
{
	//�����������ݣ�����ʱ��ĳ���߳� �ô���ѹ���������ס���������ݡ�������
	//����������������ݵ��̱߳���ȴ�����������ס��������������
	//����������
	//һ����������mutex���Ļ�������
	//�������Ǹ����������һ����������̳߳�����lock()��Ա���������������ͷ��ֻ��һ���߳��������ɹ����ɹ��ı�־��lock()�������أ�
	//���û���ɹ�����ô���̿���lock()���ﲻ�ϵĳ���ȥ�������ͷ������unlock()
	//������ʹ��ҪС�ģ��������ݲ���Ҳ���٣����ˣ�û�ﵽ����Ч�������ˣ�Ӱ��Ч��

	//�������������÷�
	//��2.1��lock()��unlock()
	//���裺��lock()�������������ݣ�unlock()��
	//lock()��unlock()Ҫ�ɶ�ʹ�ã���lock()��ȻҪ��unlock()��ÿ����һ��lock()����ȻӦ�õ���һ��unlock()��
	//��Ӧ��Ҳ���������1��lock()ȴ����2��unlock()��Ҳ���������2��lock()ȴ����1��unlock()����Щ�ǶԳ������ĵ��ö��ᵼ�´��벻�ȶ�����������
	//��lock������unlock�����⣬�ǳ����Ų飻
	//Ϊ�˷�ֹ�������unlock()��������һ����std::lock_guard����ģ�壺������unlock��Ҫ����������unlock()��
	//ѧϰ������ָ��(unique_ptr<>)���������ͷ��ڴ治Ҫ�����Ҹ����ͷţ�	��ķ��

		//��2.2��std::lock_guard��ģ�壺ֱ��ȡ��lock()��unlock()����Ҳ����˵��������lock_gruard֮���ٲ���ʹ��lock()��unlock()

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

			std::lock_guard<std::mutex> sbguard1(my_mutex1);
			std::lock_guard<std::mutex> sbguard2(my_mutex2);
			//my_mutex1.lock();
			//ʵ�ʹ�����������ͷ��һ�����ţ�����������Ҫ������ͬ�����ݹ���飻
			////����������//
			//my_mutex2.lock();
			msgRecvQueue.push_back(i);
			//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			//my_mutex2.unlock();
			//my_mutex1.unlock();
			//......
			//����������룻
		}
	}

	bool outMsgLULProc(int &command)
	{
		std::lock_guard<std::mutex> sbguard1(my_mutex1);
		//sbguard����ʦ�����Ķ�����
		//lock_guard���캯����ִ����mutext::lock()��
		//lock_guard����������ִ����mutext::unlock()��3
		std::lock_guard<std::mutex> sbguard2(my_mutex2);

		//my_mutex2.lock();
		//my_mutex1.lock();
		//my_mutex1.lock();
		//my_mutex2.lock();
		if (!msgRecvQueue.empty())
		{
			//��Ϣ��Ϊ��
			command = msgRecvQueue.front();
			//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();
			//�Ƴ���һ��Ԫ�أ��������أ�
			//my_mutex1.unlock();
			//my_mutex2.unlock();
			return true;
		}
		//my_mutex1.unlock();
		//my_mutex2.unlock();
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
	std::mutex my_mutex2;
	//����һ��������
};

int main02()
{
	//��������
	//������վ�ڱ��� �����ģ���Ų�ѣ�
	//���ģ�վ������ ����������Ų�ѣ�
	//C++�У�
	//��������������������������� ��������������ͷ�����������������ܲ�������������JinLock����������YinLock��
	//�����߳�A��B
	//(1)�߳�Aִ�е�ʱ������߳����� �������ѽ���lock()�ɹ��ˣ�Ȼ������Ҫȥlock����������
	//�������������л�
	//(2)�߳�Bִ���ˣ�����߳����� ��������Ϊ������û�б���������������lock()�ɹ����߳�BҪȥlock��������������
	//��ʱ�˿̣������Ͳ����ˣ�
	//(3)�߳�A��Ϊ�ò�������ͷ�������߲���ȥ�����к�ߴ����н�������ͷ�ĵ��������߲���ȥ�����Խ���ͷ�ⲻ����
	//(4)�߳�B��Ϊ�ò�������ͷ�������߲���ȥ�����к�ߴ����н�������ͷ�ĵ��������߲���ȥ����������ͷ�ⲻ����
	//��Ҷ������������ң��ҵ��㣻

	//��3.1��������ʾ
	//��3.2��������һ��������
	//ֻҪ��֤������������������˳��һ�¾Ͳ���������

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
	//���յ�����Ϣ���������뵽һ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;

			std::lock(my_mutex1, my_mutex2);
			//�൱��ÿ����������������lock()��

			std::lock_guard<std::mutex> sbguard1(my_mutex1, std::adopt_lock);
			std::lock_guard<std::mutex> sbguard2(my_mutex2, std::adopt_lock);

			msgRecvQueue.push_back(i);
			//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			//my_mutex2.unlock();
			//my_mutex1.unlock();
			//......
			//����������룻
		}
	}

	bool outMsgLULProc(int &command)
	{
		std::lock(my_mutex1, my_mutex2);

		std::lock_guard<std::mutex> sbguard1(my_mutex1, std::adopt_lock);
		std::lock_guard<std::mutex> sbguard2(my_mutex2, std::adopt_lock);

		if (!msgRecvQueue.empty())
		{
			//��Ϣ��Ϊ��
			command = msgRecvQueue.front();
			//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();
			//�Ƴ���һ��Ԫ�أ��������أ�
			//my_mutex1.unlock();
			//my_mutex2.unlock();
			return true;
		}
		//my_mutex1.unlock();
		//my_mutex2.unlock();
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
	std::mutex my_mutex2;
	//����һ��������
};

int main()
{
	//��3.3��std::lock()����ģ�壺������������������ʱ��ų���
	//������һ����ס���������������ϵĻ��������������������˲��ޣ�1�����У���
	//��������������Ϊ�ڶ���߳��� ��Ϊ����˳�����⵼�������ķ������⣻
	//std::lock()���������������һ��û��ס��������������ţ������л���������ס�������������ߣ����أ���
	//Ҫô��������������ס��Ҫô������������û��ס�����ֻ����һ��������һ��û���ɹ��������������Ѿ���ס�Ľ�����

	//��3.4��std::lock_guard��std::adopt_lock����
	//std::adopt_lock�Ǹ��ṹ�������һ��������ã����þ��Ǳ�ʾ����������Ѿ�lock()��
	//����Ҫ��std::lock_guard<std::mutex>���캯�������mutex��������ٴ�lock()�ˣ�
	//�ܽ᣺std::lock()һ���������������������ʹ�ã�����һ��һ��������

	AAA myobja;
	std::thread myOutnMsgObj(&AAA::outMsgRecvQueue, &myobja);
	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myInMsgObj(&AAA::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();

	return 0;
}
