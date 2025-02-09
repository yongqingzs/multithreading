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

using namespace std;

class A
{
public:

	void outMsgRecvQueue()
	{
		int command = 0;
		while (true)
		{
			std::unique_lock<std::mutex> sbguard1(my_mutex1);	//�ٽ��ȥ
			my_cond.wait(sbguard1, [this] {	//this�����Բο� δ����֪ʶ�㣬�ڰ˽�
				if (!msgRecvQueue.empty())
					return true;	//��lambda����true����wait�ͷ��أ��������������������������߳��õ���
				return false;	//���������ߣ�����wait�ȴ����ٴλ���
			});
			//���ڻ��������ţ������������ˣ������������ݣ�
			command = msgRecvQueue.front();	//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();	//�Ƴ���һ��Ԫ�أ��������أ�
			sbguard1.unlock();	//��Ϊunique_lock������ԣ��������ǿ�����ʱ��unlock������������ס̫��ʱ��
			cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��: " << command << "threadid = " << std::this_thread::get_id() << endl;
		}  //end while
	}

	void inMsgRecvQueue()	//unlock()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;
			std::unique_lock<std::mutex> sbguard1(my_mutex1);
			msgRecvQueue.push_back(i);	//�����������i�������յ��������ֱ��Ū����Ϣ�����������
			my_cond.notify_one();	//���ǳ��԰�wait()���̻߳��ѣ���ʵ����outMsgRecvQueue()�е�my_cond.wait()�Ѿ����ˣ���������
		}
		return;
	}

private:
	std::list<int> msgRecvQueue;	//������ר�����ڴ�����Ҹ����Ƿ��͹��������
	std::mutex my_mutex1;	//����һ����������һ����ͷ��
	std::condition_variable my_cond;	//����һ��������������
};

int main01()
{
	//һ������һЩ֪ʶ��
	//��1.1����ٻ��ѣ�wait��Ҫ�еڶ�������lambda���������lambda��Ҫ��ȷ�ж�Ҫ����Ĺ��������Ƿ���ڣ�
	//wait()��notify_one()��notify_all()

	A myobja;
	std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();

	system("pause");
	return 0;
}




class AA
{
public:
	std::atomic<int> atm;
	AA()
	{
		atm = 0;
		//auto amt2 = atm;	//���ֶ���ʱ��ʼ��������������ʾ������������ɾ���ĺ������������ڲ��϶��ѿ������캯�����ɵ���
		//atomic<int> atm3 = atm;
		//atomic<int> atm2;
		//atm2 = atm;	//����������ɾ���ĺ������������������Ҳ������
		//load()����ԭ�ӷ�ʽ��atomic�����ֵ
		atomic<int> atm2(atm.load());	//��
		auto atm3(atm.load());
		//store()��ԭ�ӷ�ʽд������
		atm2.store(12);
		atm2 = 12;
	}

	void outMsgRecvQueue()
	{
		int command = 0;
		while (true)
		{
			cout << atm << endl;	//��atm�Ǹ�ԭ�Ӳ���������������һ�д��벢���Ǹ�ԭ�Ӳ�����
		}  
	}

	void inMsgRecvQueue()	//unlock()
	{
		for (int i = 0; i < 1000000; ++i)
		{
			atm += 1;	//ԭ�Ӳ���
			//atm = atm + 1;	//����ԭ�Ӳ���
		}
		return;
	}

private:
	std::list<int> msgRecvQueue;	//������ר�����ڴ�����Ҹ����Ƿ��͹��������
	std::mutex my_mutex1;	//����һ����������һ����ͷ��
	std::condition_variable my_cond;	//����һ��������������
};

int main()
{
	//��1.2��atomic��10��11�ڶ��н���


	//����ǳ̸�̳߳�
	//��2.1����������
	//����������--���ͻ��ˣ�ÿ�� һ���ͻ��ˣ��ʹ��� һ�����߳�Ϊ�ÿͻ��ṩ����
	//a��������Ϸ��2����Ҳ����ܸ�ÿ����Ҵ��������̣߳��˳���д�������ֳ����²�ͨ��
	//b�������ȶ������⣺��д�Ĵ����У�ż������һ���߳����ִ��룬����д���������˸е�������
	//�̳߳أ���һ���߳�Ū��һ��ͳһ��������ͳһ������ȣ�ѭ�������̵߳ķ�ʽ���ͽ��̳߳أ�
	//��2.2��ʵ�ַ�ʽ
	//�ڳ�������ʱ����һ���ԵĴ�����һ���������̡߳�10��8��100-200�������˷��ģ����ó��������ȶ���


	//�����̴߳�������̸
	//��3.1���߳̿��������������⣬2000���̻߳������Ǽ��ޣ��ٴ����߳̾ͱ�����
	//��3.2���̴߳�������������
	//a������ĳЩ�����������򣻽ӿ��ṩ�̽����� �����߳����� = cpu������cpu * 2��cpu * 2 + 2������רҵ�����ָʾ����רҵ���ȷ�������Чִ��
	//b���������߳����ҵ�� һ���̵߳���һ��ִ��ͨ·��100Ҫ������ֵ���������￪110���̣߳����Ǻܺ��ʵģ�
	//c��1800���̣߳����飬�߳�����������Ҫ����500�����ܹ�������200��֮�ڣ�


	//�ģ�C++11���߳��ܽ�
	//windows��linux��

	AA myobja;
	std::thread myOutnMsgObj(&AA::outMsgRecvQueue, &myobja);	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myInMsgObj(&AA::inMsgRecvQueue, &myobja);
	std::thread myInMsgObj2(&AA::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();
	myInMsgObj2.join();

	system("pause");
	return 0;
}