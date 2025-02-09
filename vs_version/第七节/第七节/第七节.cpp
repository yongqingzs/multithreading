// ���߽�.cpp : Defines the entry point for the console application.
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

//class A
//{
//public:
//};

std::mutex resource_mutex;
std::once_flag g_flag; //���Ǹ�ϵͳ����ı��

class MyCAS //����һ��������
{
	static void CreateInstance()
	//ֻ������1��
	{
		std::chrono::milliseconds dura(2000);
		//��Ϣ20s
		std::this_thread::sleep_for(dura);

		cout << "CreateInstance()��ִ����" << endl;

		m_instance = new MyCAS();
		static CGarhuishou c1;
	}

private:
	MyCAS() {}
	//˽�л��˹��캯��

private:
	static MyCAS *m_instance;
	//��̬��Ա����

public:
	static MyCAS *GetInstance()
	{
		//���Ч�ʡ�
		//a����� if (m_instance != NULL) ������������϶���ʾm_instance�Ѿ���new���ˣ�
		//b����� if (m_instance != NULL)��������m_instanceһ��û��new����

		//if (m_instance == NULL)
		//˫��������˫�ؼ�飩
		//{
		//  std::unique_lock<std::mutex> mymutex(resourec_mutex);
		//�Զ����� 
		//  if (m_instance == NULL)
		//  {
		//  m_instance = new MyCAS();
		//  static CGarhuishou cl;
		//  }
		//}

		std::call_once(g_flag, CreateInstance);
		//�����߳�ͬʱִ�е��������һ���߳�Ҫ������һ���߳�ִ�����CreateInstance()��
		return m_instance;
	}

	class CGarhuishou
		//�������࣬�����ͷŶ���
	{
	public:
		~CGarhuishou()
			//�������������
		{
			if (MyCAS::m_instance)
			{
				delete MyCAS::m_instance;
				MyCAS::m_instance = NULL;
			}
		}
	};

	void func()
	{
		cout << "����" << endl;
	}
};

//�ྲ̬������ʼ��
MyCAS *MyCAS::m_instance = NULL;

//�߳���ں���
void mythread()
{
	cout << "�ҵ��߳̿�ʼִ����" << endl;
	MyCAS *p_a = MyCAS::GetInstance();
	//������ܾ� ��������
	p_a->func();
	cout << "�ҵ��߳�ִ�������" << endl;
	return;
}

int main()
{
	//һ�����ģʽ���̸
	//�����ģʽ���������һЩд������Щд��������д������ôһ������������ά���������ܷ��㣬���Ǳ��˽ӹܡ��Ķ����붼���ʹ�ࣻ
	//�á����ģʽ������д�����Ĵ���ܻ�ɬ�ģ���head first��
	//���� Ӧ���ر�����Ŀ��ʱ�� ����Ŀ�Ŀ������顢ģ�黮�־��飬�ܽ������ ���ģʽ�����п������󣬺��������ܽ��������
	//���ģʽ�õ��й�������̫һ��������һ��������Ŀ�������ģʽ���ף�һ��СС����Ŀ������ҪŪ�������ģ�ͽ�ȥ����ĩ����
	//���ģʽ�϶��������ص��ŵ㣬Ҫ��ѧ���ã���Ҫ�������У�����Ӳ�ף�

	//�����������ģʽ
	//�������ģʽ��ʹ�õ�Ƶ�ʱȽ� �ߣ�
	//������������Ŀ�У���ĳ������ĳЩ������࣬���ڸ���Ķ�����ֻ�ܴ���1���������Ҵ������ˣ�
	//�����ࣻ
	/*MyCAS a;
	MyCAS a2;*/
	MyCAS *p_a = MyCAS::GetInstance();
	//�������󣬷��ظ��ࣨMyCAS�������ָ��
	//MyCAS *p_b = MyCAS::GetInstance();
	p_a->func();
	MyCAS::GetInstance()->func();
	//��װ�ص�����װ��


	//�����������ģʽ��������������������
	//���ٵ����⣺��Ҫ�������Լ��������̣߳����������̣߳��д���MyCAS���������Ķ��������߳̿��ܲ�ֹһ��������2������
	//���ǿ��ܻ�����GetInstance()���ֳ�Ա������Ҫ���⣻
	//��Ȼ�������߳���ͬһ����ں����������ǧ��Ҫ��ס�����������̣߳�������������������̣�����ͨ·��ͬʱִ��
	std::thread mytobj1(mythread);
	std::thread mytobj2(mythread);
	mytobj1.join();
	mytobj2.join();


	//�ģ�std::call_once()��C++11����ĺ������ú����ĵڶ������� ��һ��������a()��
	//call_once�������ܹ���֤����a()ֻ������һ�Ρ�
	//call_once�߱���������������������Ч���ϣ��Ȼ��������ĵ���Դ���٣�
	//call_once()��Ҫ��һ����ǽ��ʹ�ã�������std::once_flag����ʵonce_flag��һ���ṹ��
	//call_once()����ͨ����������������Ӧ�ĺ���a()�Ƿ�ִ�У�����call_once()�ɹ���call_once()�Ͱ�����������Ϊһ���ѵ���״̬��
	//�����ٴε���call_once()����Ҫonce_flag������"�ѵ���"״̬����ô���Եĺ���a()�Ͳ����ٱ�ִ���ˣ�


	return 0;
}
