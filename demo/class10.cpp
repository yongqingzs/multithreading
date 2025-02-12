// ��ʮ��.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <list>
#include <mutex>
#include <future>

using namespace std;

int mythread()
{
    cout << "mythread() start" << " threadid = " << std::this_thread::get_id() << endl; // ���߳�id
    std::chrono::milliseconds dura(1000); // ��һ��5���ʱ��
    std::this_thread::sleep_for(dura); // ��Ϣ��һ��ʱ��
    cout << "mythread() end" << " threadid = " << std::this_thread::get_id() << endl;
    return 5;
}

// int main()
int main01()
{
    // һ��std::future��������Ա������get()����ת������

    cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
    std::future<int> result = std::async(mythread);
    // �̲߳����Ῠ������
    // std::future<int> result = std::async(std::launch::deferred, mythread);
    cout << "continue...!" << endl;
    // cout << result.get() << endl;
    // ��������ȴ��߳�ִ���꣬
    // ��������get��ΪһЩ�ڲ��������������get��Σ�ֻ��getһ��
    // ö�����ͣ�
    // wait_for���ȴ�һ����ʱ�䣩
    std::future_status status = result.wait_for(std::chrono::seconds(2));
    // �ȴ�1��
    if (status == std::future_status::timeout)
        // ��ʱ������ȴ���1���ӣ�ϣ���㷵�أ���û�з��أ���ôstatus = timeout
    {
        // ��ʾ�̻߳�ûִ���ꣻ
        cout << "Timeout, thread has not finished yet" << endl;
    }
    else if (status == std::future_status::ready)
    {
        // ��ʾ�̳߳ɹ�����
        cout << "Thread finished successfully, returning" << endl;
        cout << result.get() << endl;
    }
    else if (status == std::future_status::deferred)
    {
        // ���async�ĵ�һ������������Ϊstd::launch::deferred������������
        cout << "Thread execution deferred" << endl;
        cout << result.get() << endl;
    }

    cout << "I Love China!" << endl;
    return 0;
}

int mythread1(int mypar)
{
    cout << "mythread1() start " << mypar << " threadid = " << std::this_thread::get_id() << endl; // ���߳�id
    std::chrono::milliseconds dura(1000); // ��һ��5���ʱ��
    std::this_thread::sleep_for(dura); // ��Ϣ��һ��ʱ��
    cout << "mythread1() end " << mypar << " threadid = " << std::this_thread::get_id() << endl;
    return 5;
}

// void mythread2(std::future<int> &tmpf)  // ע�����
void mythread2(std::shared_future<int> &tmpf)
{
    cout << "mythread2() start" << " threadid = " << std::this_thread::get_id() << endl;
    auto result = tmpf.get();
    // ��ȡֵ��ֻ��getһ�η���ᱨ�쳣
    // result = tmpf.get();
    // Ϊʲô�ڶ���get���future���ǻ�õ�һ���쳣����Ҫ����Ϊget��������ƣ���һ���ƶ�����
    cout << "mythread2 result: " << result << endl;
    return;
}

// int main()
int main02()
{
    // ����std::shared_future��Ҳ�Ǹ���ģ�壬get()������������

    cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
    std::packaged_task<int(int)> mypt(mythread1);
    // ���ǰѺ���mythreadͨ��packaged_task��װ������
    std::thread t1(std::ref(mypt), 114514);
    // std::thread t1(mypt, 1);
    // �߳�ֱ�ӿ�ʼִ�У��ڶ������� ��Ϊ�߳���ں����Ĳ�����
    t1.join();
    // ���ǿ��Ե�������ȴ��߳�ִ����ϣ������������̫�У������

    std::future<int> result = mypt.get_future();
    // std::future��Ҳ�İ���ˣ���������ﺬ���߳���ں���
    std::shared_future<int> result_s(std::move(result));
    bool ifcanget = result.valid();
    // std::shared_future<int> result_s(result.share());
    // ִ����Ϻ�result_s����ֵ����result�����
    ifcanget = result.valid();

    // std::shared_future<int> result_s(mypt.get_future()); // ͨ��get_future����ֱֵ�ӹ�����һ��share_future����
    auto mythreadresult = result_s.get();
    mythreadresult = result_s.get();

    // std::thread t2(mythread2, std::ref(result));
    std::thread t2(mythread2, std::ref(result_s));
    t2.join();
    // ���߳�ִ�����

    cout << "I Love China!" << endl;
    return 0;
}

// int g_mycout = 0; // ����һ��ȫ�ֱ���
std::atomic<int> g_mycout(0); // ���Ƿ�װ��һ������Ϊint�Ķ���ֵ�������ǿ��������һ��int���ͱ���һ�����������g_mycout
int g_mycout_1 = 0;
std::mutex g_my_mutex; // ������
std::list<int> g_mylist;

void mythread03(int sign) // �߳���ں���
{
    for (int i = 0; i < 100; i++)
    {
        // g_my_mutex.lock();
        // 7����ʵ����2000��εļ����ͽ�����
        // g_mycout++;
        // ...
        // ...
        // g_my_mutex.unlock();

        g_mycout++;
        // ��Ӧ�Ĳ�����ԭ�Ӳ��������ᱻ��ϣ�
		cout << "sign: " << sign << " g_mycout: " << g_mycout << endl;
    }

    return;
}

void mythread03_1(int sign)
{
	for (int i = 0; i < 100; i++)
	{
		std::lock_guard<std::mutex> myguard(g_my_mutex);
		// g_mycout_1++;

		// ��θ����״����̳߳�ͻ�Ĵ���
		if (g_mylist.size() > 0)
		{
			// std::lock_guard<std::mutex> myguard(g_my_mutex);  // error �������ﻹ�ǻ��̳߳�ͻ
			cout << "sign: " << sign << "g_mylist fisrt: " << g_mylist.front() << endl;
			g_mylist.pop_front();
		}

		cout << "sign: " << sign << " g_mycout_1: " << g_mycout_1 << endl;
	}
}


// int main()
int main03()
{
    // ����ԭ�Ӳ���std::automic
    // ��3.1��ԭ�Ӳ���������������
    // �����������̱߳���� �����������ݣ������������������ݣ�����
    // �������̣߳���һ���������в���������̶߳��ñ���ֵ����һ���߳������������дֵ��
    // int atomvalue = 5��
    // // ���߳�A
    // int tmpvalue = atomvalue;
    // �������atomvalue������Ƕ���߳�֮�乲��ı�����
    // // д�߳�B
    // atomvalue = 6;
    // ������Ļ���

    // ��ҿ��԰�ԭ�Ӳ�������һ�֣�����Ҫ�õ������������������������Ķ��̲߳�����̷�ʽ
    // ԭ�Ӳ������ڶ��߳��� ���ᱻ��ϵ� ����ִ��Ƭ�Σ�ԭ�Ӳ������Ȼ�����Ч���ϸ�ʤһ�
    // �������ļ���һ�������һ������Σ����д��룩����ԭ�Ӳ�����Ե�һ����һ��������������һ������Σ�

    // ԭ�Ӳ�����һ�㶼��ָ�����ɷָ�Ĳ�������Ҳ����˵���ֲ���״̬Ҫô����ɵģ�Ҫô��û��ɵģ������ܳ��ְ����״̬��
    // std::atomic������ԭ�Ӳ�����std::automic�Ǹ���ģ�塣��ʵstd::atomic���������������װĳ�����͵�ֵ�ģ�
    // ��3.2��������std::atomic

	// g_mylist.front();  // error
	// g_mylist.pop_front();
	for (int i = 0; i < 100; i++)
	{
		g_mylist.push_back(i);	
	}

    std::thread myobj1(mythread03, 114514);
    std::thread myobj2(mythread03, 1919);

	std::thread myobj3(mythread03_1, 810);
	std::thread myobj4(mythread03_1, 910);
	std::thread myobj5(mythread03_1, 1010);
	std::thread myobj6(mythread03_1, 1110);
	std::thread myobj7(mythread03_1, 1210);
    
	myobj1.join();
    myobj2.join();

	myobj3.join();
	myobj4.join();
	myobj5.join();
	myobj6.join();
	myobj7.join();

    cout << "Both threads finished, final result of g_mycout is: " << g_mycout << endl;

    return 0;
}

std::atomic<bool> g_ifend(0); // �߳��˳���ǣ�������ԭ�Ӳ�������ֹ����д���ף�

void mythread04()
{
    std::chrono::milliseconds dura(1000);
    // 1����
    while (g_ifend == false)
    {
        // ϵͳûҪ���߳��˳������Ա��߳̿��Ը��Լ���ɵ�����
        cout << "thread id = " << std::this_thread::get_id() << " running..." << endl;
        std::this_thread::sleep_for(dura);
    }
    cout << "thread id = " << std::this_thread::get_id() << " finished running..." << endl;
    return;
}

int main()
// int main04()
{
    // ��3.3����ʦ���ĵã�һ�����ڼ�������ͳ�ƣ��ۼƷ��ͳ�ȥ�˶��ٸ����ݰ����ۼƽ��ܵ��˶�����ݰ�����

    std::thread myobj1(mythread04);
    std::thread myobj2(mythread04);
    std::chrono::milliseconds dura(5000);
    // 5����
    std::this_thread::sleep_for(dura);
    g_ifend = true; //�߳��첽
    // ��ԭ�Ӷ����д���������߳��������н����
    myobj1.join();
    myobj2.join();
    cout << "Program finished, exiting" << endl;

    return 0;
}