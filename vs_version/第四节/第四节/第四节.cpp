// ���Ľ�.cpp : Defines the entry point for the console application.
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

//�߳���ں���
void myprint01(int inum)
{
	cout << "myprint�߳̿�ʼִ���ˣ��̱߳�� = " << inum << endl;
	//....�ɸ�������
	cout << "myprint�߳̽���ִ���ˣ��̱߳�� = " << inum << endl;
	return;
}

int main01()
{
	//һ�������͵ȴ�����߳�
	vector<thread> mythreads;
	//����10���̣߳��߳���ں���ͳһʹ�� myprint��
	//a)������߳�ִ��˳�����ҵģ�������ϵͳ�ڲ����̵߳����е��Ȼ����йأ�
	//b)�����̵߳ȴ��������߳����н�����������߳̽�������ʦ�Ƽ�����join��д����������д���ȶ��ĳ���
	//c)�����ǰ�thread������뵽��������������������thread�������飬����������δ����������̲߳��Դ����߳̽��й���ܷ��㡣

	for (int i = 0; i < 10; i++)
	{
		mythreads.push_back(thread(myprint01, i));
		//����10���̣߳�ͬʱ��10���߳��Ѿ���ʼִ��
	}
	for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter)
	{
		iter->join();
		//�ȴ�10���̶߳�����
	}
	cout << "I Love China!" << endl;
	//���ִ����䣬���������˳�

	return 0;
}



vector<int> g_v = { 1, 2, 3 }; //��������

//�߳���ں���
void myprint02(int inum)
{
	cout << "idΪ" << std::this_thread::get_id() << "���߳� ��ӡg_vֵ" << g_v[0] << g_v[1] << g_v[2] << endl;
	return;
}

int main02()
{
	//�������ݹ����������
	//��2.1��ֻ�������ݣ��ǰ�ȫ�ȶ��ģ�����Ҫ�ر�ʲô�����ֶΡ�ֱ�Ӷ��Ϳ���
	//��2.2���ж���д��2���߳�д��8���̶߳����������û���ر�Ĵ����ǳ���϶�������
	//��򵥵Ĳ�������������ʱ����д��д��ʱ���ܶ���2���̲߳���ͬʱд��8���̲߳���ͬʱ����
	//д�Ķ�����10С�������������л������¸��ֹ������鷢��������ܵĹ������黹�Ǳ�������
	//��2.3����������
	//���ݹ���
	//����--���� �� T123��10����Ʊ���� ��Ʊ��	1, 2   ͬʱ��Ҫ��   99��

	vector<thread> mythreads;
	for (int i = 0; i < 10; i++)
	{
		mythreads.push_back(thread(myprint02, i));
		//����10���̣߳�ͬʱ��10���߳��Ѿ���ʼִ��
	}
	for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter)
	{
		iter->join();
		//�ȴ�10���̶߳�����
	}
	cout << "I Love China!" << endl;
	//���ִ����䣬���������˳�

	return 0;
}



class A
{
public:
	//���յ�����Ϣ���������뵽һ�����е��߳�
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;
			msgRecvQueue.push_back(i);
			//�����������i�������յ��������ֱ��Ū����Ϣ�����������
		}
	}

	//�����ݴ���Ϣ������ȡ�����̣߳�
	void outMsgRecvQueue()
	{
		for (int i = 0; i < 100000; ++i)
		{
			if (!msgRecvQueue.empty())
			{
				//��Ϣ��Ϊ��
				int command = msgRecvQueue.front();
				//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
				msgRecvQueue.pop_front();
				//�Ƴ���һ��Ԫ�أ��������أ�
				//����Ϳ��Ƕȴ�������...
				//........
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
};

int main()
{
	//�����������ݵı�����������
	//������Ϸ�������������Լ��������̣߳�һ���߳��ռ��������(��һ�����ִ�����ҷ���������)��������������д��һ�������С�
	//  ��һ���̴߳Ӷ�����ȡ����ҷ����������������Ȼ��ִ�������Ҫ�Ķ�����
	//vector��list��list��vector���ơ�list��Ƶ���ذ�˳������ɾ������ʱƵ�ʸߡ�vector��������Ĳ����ɾ������Ч�ʸߡ�
	//׼���ó�Ա������Ϊ�̺߳����ķ�����д�̣߳�
	//���뻯������⣻ ��ʦ ����һ��C++������̱߳���������������ĵ�һ������������������Դ��������ʣ�

	A myobja;
	std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);
	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();

	return 0;
}
