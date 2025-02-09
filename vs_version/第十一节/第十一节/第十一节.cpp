// ��ʮһ��.cpp : �������̨Ӧ�ó������ڵ㡣
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

std::atomic<int> g_mycout = 0;	//���Ǹ�ԭ���������ͱ�����������ʹ�����ͱ���һ��ʹ��
void mythread1()
{
	for (int i = 0; i < 10000000; i++)
	{
		g_mycout++;	//��Ӧ�Ĳ�������ԭ�Ӳ��������ᱻ��ϣ�
		g_mycout += 1;
		//g_mycout = g_mycout + 1;	//�������
	}
	return;
}


int main01()
{
	//һ��ԭ�Ӳ���std::atomic��̸
	//һ��atomicԭ�Ӳ��������++��--��+=��&=��|=��^=��֧�ֵġ������Ŀ��ܲ�֧�֡�

	std::thread mytobj1(mythread1);
	std::thread mytobj2(mythread1);
	mytobj1.join();
	mytobj2.join();
	cout << "�����̶߳�ִ����ϣ����յ�g_mycout�Ľ����" << g_mycout << endl;

	return 0;
}




int mythread2()
{
	cout << "mythread start" << " threadid = " << std::this_thread::get_id() << endl;
	return 1;
}

int main02()
{
	//����std::async����̸
	//��2.1��std::async����������async��������һ���첽����
	cout << "main start" << " threadid = " << std::this_thread::get_id() << endl;
	//std::future<int> result = std::async(std::launch::deferred, mythread2);	//deferred�ӳٵ��ã����Ҳ��������̣߳��ӳٵ�future�������.get()����.wait()��ʱ���ִ��mythread()
	//std::future<int> result = std::async(std::launch::async, mythread2);
	//std::future<int> result = std::async(std::launch::async | std::launch::deferred, mythread2);
	std::future<int> result = std::async(mythread2);
	cout << result.get() << endl;

	//��ʦ����������� std::launch::deferred���ӳٵ��á����Լ�std::launch::async��ǿ�ƴ���һ���̡߳�
	//std::thread() ���ϵͳ��Դ���ţ���ô���ܴ����߳̾ͻ�ʧ�ܣ���ôִ��std::thread()ʱ����������ܻ������
	//std::async()����һ�㲻�д����̣߳�����async�ܹ������̣߳�������һ��������� һ���첽����
	//std::async��std::thread�����ԵĲ�ͬ������async��ʱ�򲢲��������̡߳�
	//a���������std::launch::deferred������async����ô����
	//std::launch::deferred�ӳٵ��ã����Ҳ��������̣߳��ӳٵ�future�������.get()����.wait()��ʱ���ִ��mythread()�����û�е���get����wait����ô���mythread�Ͳ���ִ�С�
	//b��std::launch::async��ǿ������첽���������߳���ִ�У�����ζ�ţ�ϵͳ����Ҫ���Ҵ��������߳�������mythread()��
	//c��std::launch::async | std::launch::deferred
	//�������|����ζ�ŵ���async����Ϊ������ ���������̲߳�����ִ�С� ���� 
											//��û�д������̲߳����ӳٵ����� result.get()�ſ�ʼִ��������ں�����	���߾���һ��
	
	//d�����ǲ������������ֻ��һ��async����һ�� ��ں�������
	//�ھŽڿ���ʦ������һ��С������ʵ��Ĭ��ֵӦ����std::launch::async | std::launch::deferred����c��Ч����ȫһ�¡�
		//���仰˵��ϵͳ�����о������첽���������̣߳�����ͬ�������������̣߳���ʽ���С�

	//���о�����ɶ��˼��ϵͳ��ξ����� �첽���������̣߳�����ͬ�������������̣߳���ʽ����
	//��2.2��std::async��std::thread������
	//std::thread�����̣߳����ϵͳ��Դ���ţ������߳�ʧ�ܣ���ô��������ͻᱨ�쳣��������Ƣ����
	//int mythread(){return 1;}
	//std::thread mytobj(mythread);
	//mytobj.join();
	//std::thread�����̵߳ķ�ʽ������̷߳���ֵ�������õ����ֵҲ�����ף�
	//std::async�����첽���񡣿��ܴ���Ҳ���ܲ������̡߳�����async���÷����������õ��߳���ں����ķ���ֵ��
	//����ϵͳ��Դ���ƣ�
	//��1�������std::thread�������߳�̫�࣬����ܴ���ʧ�ܣ�ϵͳ�����쳣��������
	//��2�������std::async��һ��Ͳ��ᱨ�쳣�����������Ϊ�����ϵͳ��Դ���ŵ����޷��������̵߳�ʱ��
		//std::async���ֲ��Ӷ�������ĵ��� �Ͳ��ᴴ�����̡߳����Ǻ���˭������result.get()����������
			//��ô����첽����mythread��������ִ������get()���ڵ��߳��ϡ�
	//�����ǿ��std::asyncһ�� Ҫ�������̣߳���ô�ͱ���ʹ�� std::launch::async�����ܵĴ��۾���ϵͳ��Դ����ʱ�����������
	//��3�����飺һ�������߳��������˳���100-200��ʱ��Ƭ��

	return 0;
}




int mythread3()
{
	cout << "mythread start" << " threadid = " << std::this_thread::get_id() << endl;

	std::chrono::milliseconds dura(5000);	//1�� = 1000���룬����5000���� = 5��
	std::this_thread::sleep_for(dura);		//��Ϣһ����ʱ��

	return 1;
}

int main()
{
	//��2.3��std::async��ȷ��������Ľ��
	//���Ӷ��������std::async���ã���ϵͳ���о����Ƿ񴴽����̡߳�
	//���⽹������ std::future<int> result = std::async(mythread)��д��
	//����첽���񵽵���û�б��Ƴ�ִ�У���std::launch::async����std::launch::deferred��
	//std::future�����wait_for��������10�ڽ�����

	cout << "main start" << " threadid = " << std::this_thread::get_id() << endl;
	std::future<int> result = std::async(mythread3);	//���ж�async������û�д������߳�����ִ�л����ӳ٣�û�������̣߳�ִ�С�

	std::future_status status = result.wait_for(std::chrono::seconds(0));	
	if (status == std::future_status::deferred)
	{
		//�̱߳��ӳ�ִ���ˣ�ϵͳ��Դ�����ˣ������Ҳ���std::launch::deferred�����ˣ�
		cout << result.get() << endl;	//���ʱ���ȥ������mythread()��
	}
	else
	{
		//����û�б��Ƴ٣��Ѿ���ʼ�������£��̱߳������ˣ�
		if (status == std::future_status::ready)
		{
			//�̳߳ɹ�����
			cout << "�̳߳ɹ�ִ����ϲ����أ�" << endl;
			cout << result.get() << endl;
		}
		else if (status == std::future_status::timeout)
		{
			//��ʱ�̻߳�ûִ����
			cout << "��ʱ�̻߳�ûִ����!" << endl;
			cout << result.get() << endl;
		}
	}

	return 0;
}