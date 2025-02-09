// ������.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <thread>

using namespace std;

//void myprint(const int &i, char *pmybuf)
//void myprint(const int i, char *pmybuf)
void myprint01(const int i, const string &pmybuf)
{
	cout << i << endl;
	//������Ϊ��i������mvar�����ã�ʵ����ֵ���ݣ���ô������Ϊ���������߳�detach�����̣߳���ô���߳�����iֵӦ���ǰ�ȫ��
	// //cout << pmybuf << endl;
	//ָ����detach���߳�ʱ�����Ի�������
	cout << pmybuf.c_str() << endl;
	return;
}

int main01()
{
	//һ��������ʱ������Ϊ�̲߳���
	//��1.1��Ҫ��������壨����1��
	//��1.2��Ҫ��������壨����2��
	//��ʵ1��ֻҪ����ʱ�����A�������Ϊ�������ݸ��̣߳���ô��һ���ܹ������߳�ִ�����ǰ���̺߳����ĵڶ������������������Ӷ�ȷ����ʱdetach()���߳�Ҳ�ܹ���ȫ����
	//��1.3���ܽ�
	//(a)������int���ּ����Ͳ��������鶼��ֵ���ݣ���Ҫ�����á���ֹ������֦��
	//(b)����Ǵ�������󣬱�����ʽ����ת����ȫ�����ڴ����߳���һ�о͹�������ʱ��������Ȼ���ں������������������ӣ�����ϵͳ����๹��һ�ζ����˷ѣ�
	//�ռ����ۣ�
	//(c)���鲻ʹ��detach()��ֻ����join()�������Ͳ����ھֲ�����ʧЧ�����̶߳��ڴ�ķǷ��������⣻

	//������ʱ������Ϊ�̲߳�������������ʦ���ò��Դ󷢣�
	//��2.1���߳�id���id�Ǹ����֣�ÿ���̣߳����������̻߳������̣߳�ʵ���϶���Ӧ��һ�����֣�����ÿ�����ֶ�Ӧ��������ֶ���ͬ��
	//Ҳ���ǣ���ͬ���̣߳������߳�id�����֣���Ȼ�ǲ�ͬ��
	//�߳�id������C++��׼����ĺ�������ȡ��std::this_thread::get_id()����ȡ��

	//�����������������ָ����Ϊ�̲߳���
	//std::ref ����

	//�ģ��ó�Ա����ָ�����̺߳���operator()


	int mvar = 1;
	int &mvary = mvar;
	char mybuf[] = "this is a test!";
	//thread mytobj(myprint, mvar, mybuf);
	//����mybuf������ʲôʱ��ת����string��
	//��ʵ�ϴ��ڣ�mybuf���������ˣ�main����ִ�����ˣ���ϵͳ����mybufȥתstring�Ŀ����ԣ�
	thread mytobj(myprint01, mvar, string(mybuf));
	//��������ֱ�ӽ�mybufת����string�������� һ�����Ա�֤���߳��п϶���Ч�ġ�
	//myobj.join();
	mytobj.detach();
	//���̺߳����̷ֱ߳����
	cout << "I Love China!" << endl;

	return 0;
}



class A
{
public:
	//int m_i;
	mutable int m_i;
	//����ת�����캯�������԰�һ��intת����һ����A����
	A(int a) :m_i(a)
	{
		//cout << "[A::A(int a)���캯��ִ��]" << this  << endl;
		cout << "[A::A(int a)���캯��ִ��]" << this << " thread = " << std::this_thread::get_id() << endl;
	}
	A(const A &a) :m_i(a.m_i)
	{
		//cout << "[A::A(int a)�������캯��ִ��]" << this << endl;
		cout << "[A::A(int a)�������캯��ִ��]" << this << " thread = " << std::this_thread::get_id() << endl;
	}
	~A()
	{
		cout << "[A::A()��������ִ��]" << this << endl;
		cout << "[A::A()��������ִ��]" << this << " thread = " << std::this_thread::get_id() << endl;
	}

	void thread_work(int num)
		//��������
	{
		cout << "�����߳�thread_workִ�С�" << this << " thread = " << std::this_thread::get_id() << endl;
	}

	void operator() (int num)
	{
		cout << "�����߳�()ִ�С�" << this << " thread = " << std::this_thread::get_id() << endl;
	}
};

void myprint02(const int i, const A &pmybuf)
{
	cout << &pmybuf << endl;
	//�����ӡ����pmybuf����ĵ�ַ
	return;
}

int main02()
{
	int mvar = 1;
	int mysecondpar = 12;
	//thread mytobj(myprint, mvar, mysecondpar);
	// ������ϣ��mysecondparת����A���Ͷ��󴫵ݸ�myprint�ĵڶ�������
	thread mytobj(myprint02, mvar, A(mysecondpar));
	// �ڴ����̵߳�ͬ�¹�����ʱ����ķ������ݲ����ǿ��еģ�
	//mytobj.join();
	mytobj.detach();
	//���̺߳����̷ֱ߳�ִ�С�
	cout << "I Love China!" << endl;

	return 0;
}



void myprint(const A &pmybuf)
{
	cout << "���߳�myprint2�Ĳ�����ַ��: " << &pmybuf << " thread = " << std::this_thread::get_id() << endl;
	//�����������Ȼ�������߳��й����A�����
	//������ʱ��������е�A�������main()�����о��Ѿ����������
	return;
}

int main03()
{
	cout << "���߳�id��: " << std::this_thread::get_id() << endl;
	int mvar = 1;
	//std::thread mytobj(myprtint, mvar);
	std::thread mytobj(myprint, A(mvar));
	//mytobj.join();
	mytobj.detach();
	//���̺߳����̷ֱ߳�ִ�С�
	//cout << "I Love China!" << endl;

	return 0;
}




void myprint03(const A &pmybuf)
{
	pmybuf.m_i = 199;
	//�����޸ĸ�ֵ����Ӱ�쵽main����
	cout << "���߳�myprint2�Ĳ�����ַ��: " << &pmybuf << " thread = " << std::this_thread::get_id() << endl;
	return;
}

void myprint04(A &pmybuf)
{
	pmybuf.m_i = 199;
	//�����޸ĸ�ֵ����Ӱ�쵽main����
	cout << "���߳�myprint2�Ĳ�����ַ��: " << &pmybuf << " thread = " << std::this_thread::get_id() << endl;
	return;
}

int main04()
{
	A myobj(10);
	//����һ�������
	//std::thread mytobj(myprint02, myobj);
	//mytobj���������Ϊ�̲߳���
	std::thread mytobj(myprint04, std::ref(myobj));

	mytobj.join();

	return 0;
}



void myprint05(unique_ptr<int> &pzn)
{
	return;
}

int main05()
{
	unique_ptr<int> myp(new int(100));
	std::thread mytobj(myprint05, std::move(myp));

	mytobj.join();
	//ֻ����join��������detach
	//mytobj.detach();
	//���̺߳����̷ֱ߳�ִ�С�

	return 0;
}



int main06()
{
	A myobj(10);
	//����һ�������
	std::thread mytobj(&A::thread_work, myobj, 15);
	//join��detach������ 
	//std::thread mytobj(&A::thread_work, std::ref(myobj), 15);
	//ֻ����join
	//std::thread mytobj(&A::thread_work, &myobj, 15);
	//&myobj == std;:ref(myobj)

	mytobj.join();

	return 0;
}



int main()
{
	A myobj(10);
	//����һ�������
	//std::thread mytobj(myobj, 15);
	std::thread mytobj(std::ref(myobj), 15);
	//�����ÿ������캯���ˣ���ô�����������mytobj.detach()�Ͳ���ȫ�ˣ�

	mytobj.join();

	return 0;
}
