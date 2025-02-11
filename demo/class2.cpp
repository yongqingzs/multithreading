// project4.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <thread>

using namespace std;

// �Լ��������߳�ҲҪ��һ����������ʼ��������ʼ���У�
void myprint()
{
	cout << "My thread starts" << endl;
	//....
	//....
	cout << "My thread is over 1" << endl;
	cout << "My thread is over 2" << endl;
	cout << "My thread is over 3" << endl;
	cout << "My thread is over 4" << endl;
	cout << "My thread is over 5" << endl;
	cout << "My thread is over 6" << endl;
	cout << "My thread is over 7" << endl;
	cout << "My thread is over 8" << endl;
	cout << "My thread is over 9" << endl;
	cout << "My thread is over 10" << endl;
}

// int main()
int main01()
{
	//һ��������ʾ�߳����еĿ�ʼ�ͽ���
	//������������������һ�����̣��ý������������߳̿�ʼ�Զ����У�
	//cout << "I Love China!" << endl;
	//ʵ������������߳������У����̴߳�main()�������أ�����������ִ����ϡ�

	//���̴߳�main()��ʼִ�У���ô�����Լ��������̣߳�Ҳ��Ҫ��һ��������ʼ���У���ʼ��������һ���������������ϣ��ʹ�������������߳����н�����

	//���������Ƿ�ִ����ϵı�־�� ���߳��Ƿ�ִ���꣬������߳�ִ������ˣ��ʹ�����������ִ������ˡ�
	//��ʱ��һ������£�����������̻߳�û��ִ����ϣ���ô��Щ���߳�Ҳ�ᱻ����ϵͳǿ����ֹ��
	//���ԣ�һ������£����ǵõ�һ�����ۣ��������뱣�����̣߳��Լ��ô��봴�����̣߳�������״̬�Ļ�����ô���Ҫ�����߳�һֱ���� ���У���Ҫ�����߳�������ϣ�
	//���������������⣬��ʦ����������������⣬���Ŀǰ���������ͼ��䡿
	//a������ һ��ͷ�ļ�threadҪ��������
	//b����ʼ����Ҫд��
	//c��main��Ҫ��ʼд����
	//��ұ�����ȷһ�㣺�������߳����ܣ��൱���������ִ������������ͬʱ�ߣ����ԣ�����ͬʱ�������£���ʹһ���߱���ס�ˣ���һ���߻��ǿ���ͨ�еġ�����Ƕ��߳�
	//��1.1��thread���Ǹ���׼�������
	//��1.2��join()������/��ϣ�˵���˾����������������̣߳������̵߳ȴ����߳�ִ����ϣ�Ȼ�����̺߳����̻߳�ϣ�Ȼ�����߳��������ߡ�
	//������߳�ִ������ˣ������߳�ûִ����ϣ����ֳ���Ա�ǲ��ϸ�ģ�д�����ĳ���Ҳ�ǲ��ȶ��ģ�
	//һ����д���õĳ���Ӧ�������߳� �ȴ����߳�ִ����Ϻ��Լ����������˳���
	//��1.3��detach()����ͳ���̳߳������߳�Ҫ�ȴ����߳�ִ����ϣ�Ȼ���Լ�������˳���
	//detach�����룬Ҳ�������̲߳������̻߳���ˣ������߳�ִ����ģ������߳�ִ���ҵģ������߳�Ҳ���ص������߳����н������������ִ�н������Ⲣ��Ӱ�������̵߳�ִ��
	//Ϊʲô����detach()�����Ǵ����˺ܶ����̣߳������߳�����ȴ����߳̽��������ֱ�ɷ�����̫�ã�����������detach();
	//һ��detach()֮����������̹߳�����thread����ͻ�ʧȥ��������̵߳Ĺ�������ʱ������߳̾ͻ�פ���ں�̨���У����̸߳����߳�ʧȥ��ϵ��
	//������߳̾��൱�ڱ�C++����ʱ��ӹܣ���������߳�ִ����Ϻ�������ʱ�⸺��������߳���ص���Դ���ػ��̣߳���
	//detach()ʹ�߳�myprintʧȥ�����Լ��Ŀ��ơ�
	//��1.4��joinable()���ж��Ƿ���Գɹ�ʹ��join()����detach()�ģ�����true������join����detach������false������join����detach��

	//myprint�ɵ��ö���
	thread mytobj(myprint);
	//(1)�������̣߳��߳�ִ����㣨��ڣ�myprint()��(2)myprint�߳̿�ʼִ�С�
	if (mytobj.joinable())
	{
		mytobj.join();
		cout << "1:joinable() == true" << endl;
	}
	else
	{
		cout << "2:joinable() == false" << endl;
	}

	// �������̲߳��ȴ�myprint���߳�ִ����
	//mytobj.join();
	//���߳�����������ȴ�myprint()ִ���꣬�����߳�ִ����ϣ����join()��ִ����ϣ����߳̾ͼ���������
	// mytobj.detach();
	//һ��������detach()���Ͳ�������join()������ϵͳ�ᱨ���쳣��
	//mytobj.join();
	if (mytobj.joinable())
	{
		mytobj.detach();
		cout << "1:joinable() == true" << endl;
	}
	else
	{
		cout << "2:joinable() == false" << endl;
	}

	cout << "I Love China!1" << endl;
	cout << "I Love China!2" << endl;
	cout << "I Love China!3" << endl;
	cout << "I Love China!4" << endl;
	cout << "I Love China!5" << endl;

	return 0;
}



class TA
{
public:
	int m_i;
	TA(int &i) : m_i(i)
	{
		cout << "TA() constructor is executed" << endl;
	}
	TA(const TA &ta) :m_i(ta.m_i)
	{
		cout << "TA() copy constructor is executed" << endl;
	}
	~TA()
	{
		cout << "TA() destructor is executed" << endl;
	}
	void operator() ()
		// ��������
	{
		//cout << "�ҵ�operator()��ʼִ����" << endl;
		////...
		//cout << "�ҵ�operator()����ִ����" << endl;
		cout << "m_i1:" << m_i << endl;
		// ��������Ԥ�ϵĽ����
		cout << "m_i2:" << m_i << endl;
		cout << "m_i3:" << m_i << endl;
		cout << "m_i4:" << m_i << endl;
		cout << "m_i5:" << m_i << endl;
		cout << "m_i6:" << m_i << endl;
	}

	void what_can_i_say()
	{
		cout << "m_i1:" << m_i << endl;
		// ��������Ԥ�ϵĽ����
		cout << "m_i2:" << m_i << endl;
		cout << "m_i3:" << m_i << endl;
		cout << "m_i4:" << m_i << endl;
		cout << "m_i5:" << m_i << endl;
		cout << "m_i6:" << m_i << endl;
	}
};

int main()
// int main02()
{
	//�������������̵߳��ַ�
	//��2.1��������󣨿ɵ��ö��񣩣��Լ�һ�����ⷶ��
	//��ҿ��ܻ���һ�����ʣ�һ��������detach()���������߳�ִ�н����ˣ��������õ�ta����������𣿣��������ˣ�
	//�������ʵ�����Ǳ� ���� ���߳���ȥ��ִ�������̺߳�ta�ᱻ���٣����������Ƶ�TA�������ɴ��ڡ�
	//���ԣ�ֻҪ�����TA�������û�����ã�û��ָ�룬��ô�Ͳ���������⣻
	int myi = 6;
	TA ta(myi);
	thread mytobj3(ta);
	//ta:�ɵ��ö���
	mytobj3.join();
	// �ȴ����߳�ִ�н���
	// mytobj3.detach();

	cout << "I Love China!" << endl;

	return 0;
}




int main03()
{
	//��2.2����lanbda���ʽ
	auto mylamthread = [] {
		cout << "�ҵ��߳�3��ʼִ����" << endl;
		//....
		cout << "�ҵ��߳�3ִ�н�����" << endl;
	};
	thread mytobj4(mylamthread);
	//mytobj4.join();
	mytobj4.detach();

	return 0;
}