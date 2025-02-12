// 第十节.cpp : Defines the entry point for the console application.
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
    cout << "mythread() start" << " threadid = " << std::this_thread::get_id() << endl; // 新线程id
    std::chrono::milliseconds dura(1000); // 订一个5秒的时间
    std::this_thread::sleep_for(dura); // 休息了一定时长
    cout << "mythread() end" << " threadid = " << std::this_thread::get_id() << endl;
    return 5;
}

// int main()
int main01()
{
    // 一：std::future的其他成员函数，get()函数转移数据

    cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
    std::future<int> result = std::async(mythread);
    // 线程并不会卡在这里
    // std::future<int> result = std::async(std::launch::deferred, mythread);
    cout << "continue...!" << endl;
    // cout << result.get() << endl;
    // 卡在这里等待线程执行完，
    // 但是这种get因为一些内部特殊操作，不能get多次，只能get一次
    // 枚举类型：
    // wait_for（等待一定的时间）
    std::future_status status = result.wait_for(std::chrono::seconds(2));
    // 等待1秒
    if (status == std::future_status::timeout)
        // 超时：我想等待你1秒钟，希望你返回，你没有返回，那么status = timeout
    {
        // 表示线程还没执行完；
        cout << "Timeout, thread has not finished yet" << endl;
    }
    else if (status == std::future_status::ready)
    {
        // 表示线程成功返回
        cout << "Thread finished successfully, returning" << endl;
        cout << result.get() << endl;
    }
    else if (status == std::future_status::deferred)
    {
        // 如果async的第一个参数被设置为std::launch::deferred，则本条件成立
        cout << "Thread execution deferred" << endl;
        cout << result.get() << endl;
    }

    cout << "I Love China!" << endl;
    return 0;
}

int mythread1(int mypar)
{
    cout << "mythread1() start " << mypar << " threadid = " << std::this_thread::get_id() << endl; // 新线程id
    std::chrono::milliseconds dura(1000); // 订一个5秒的时间
    std::this_thread::sleep_for(dura); // 休息了一定时长
    cout << "mythread1() end " << mypar << " threadid = " << std::this_thread::get_id() << endl;
    return 5;
}

// void mythread2(std::future<int> &tmpf)  // 注意参数
void mythread2(std::shared_future<int> &tmpf)
{
    cout << "mythread2() start" << " threadid = " << std::this_thread::get_id() << endl;
    auto result = tmpf.get();
    // 获取值，只能get一次否则会报异常
    // result = tmpf.get();
    // 为什么第二次get这个future我们会得到一个异常；主要是因为get函数的设计，是一个移动语义
    cout << "mythread2 result: " << result << endl;
    return;
}

// int main()
int main02()
{
    // 二：std::shared_future：也是个类模板，get()函数复制数据

    cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
    std::packaged_task<int(int)> mypt(mythread1);
    // 我们把函数mythread通过packaged_task包装起来；
    std::thread t1(std::ref(mypt), 114514);
    // std::thread t1(mypt, 1);
    // 线程直接开始执行，第二个参数 作为线程入口函数的参数；
    t1.join();
    // 我们可以调用这个等待线程执行完毕，不调用这个不太行，会崩溃

    std::future<int> result = mypt.get_future();
    // std::future大家不陌生了，这个对象里含有线程入口函数
    std::shared_future<int> result_s(std::move(result));
    bool ifcanget = result.valid();
    // std::shared_future<int> result_s(result.share());
    // 执行完毕后result_s里有值，而result里空了
    ifcanget = result.valid();

    // std::shared_future<int> result_s(mypt.get_future()); // 通过get_future返回值直接构造了一个share_future对象。
    auto mythreadresult = result_s.get();
    mythreadresult = result_s.get();

    // std::thread t2(mythread2, std::ref(result));
    std::thread t2(mythread2, std::ref(result_s));
    t2.join();
    // 等线程执行完毕

    cout << "I Love China!" << endl;
    return 0;
}

// int g_mycout = 0; // 定义一个全局变量
std::atomic<int> g_mycout(0); // 我们封装了一个类型为int的对象（值）；我们可以像操作一个int类型变量一样来操作这个g_mycout
int g_mycout_1 = 0;
std::mutex g_my_mutex; // 互斥量
std::list<int> g_mylist;

void mythread03(int sign) // 线程入口函数
{
    for (int i = 0; i < 100; i++)
    {
        // g_my_mutex.lock();
        // 7秒钟实现了2000万次的加锁和解锁；
        // g_mycout++;
        // ...
        // ...
        // g_my_mutex.unlock();

        g_mycout++;
        // 对应的操作是原子操作（不会被打断）
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

		// 这段更容易触发线程冲突的错误
		if (g_mylist.size() > 0)
		{
			// std::lock_guard<std::mutex> myguard(g_my_mutex);  // error 放在这里还是会线程冲突
			cout << "sign: " << sign << "g_mylist fisrt: " << g_mylist.front() << endl;
			g_mylist.pop_front();
		}

		cout << "sign: " << sign << " g_mycout_1: " << g_mycout_1 << endl;
	}
}


// int main()
int main03()
{
    // 三：原子操作std::automic
    // （3.1）原子操作概念引出范例
    // 互斥量：多线程编程中 保护共享数据：先锁，操作共享数据，开锁
    // 有两个线程，对一个变量进行操作，这个线程读该变量值，另一个线程往这个变量中写值。
    // int atomvalue = 5；
    // // 读线程A
    // int tmpvalue = atomvalue;
    // 这里这个atomvalue代表的是多个线程之间共享的变量；
    // // 写线程B
    // atomvalue = 6;
    // 汇编代码的话；

    // 大家可以把原子操作理解成一种：不需要用到互斥量加锁（无锁）技术的多线程并发编程方式
    // 原子操作：在多线程中 不会被打断的 程序执行片段；原子操作，比互斥量效率上更胜一筹。
    // 互斥量的加锁一般是针对一个代码段（几行代码），而原子操作针对的一般是一个变量，而不是一个代码段；

    // 原子操作：一般都是指“不可分割的操作”；也就是说这种操作状态要么是完成的，要么是没完成的，不可能出现半完成状态；
    // std::atomic来代表原子操作，std::automic是个类模板。其实std::atomic这个东西是用来封装某个类型的值的；
    // （3.2）基本的std::atomic

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

std::atomic<bool> g_ifend(0); // 线程退出标记，这里是原子操作，防止读和写乱套；

void mythread04()
{
    std::chrono::milliseconds dura(1000);
    // 1秒钟
    while (g_ifend == false)
    {
        // 系统没要求线程退出，所以本线程可以干自己想干的事情
        cout << "thread id = " << std::this_thread::get_id() << " running..." << endl;
        std::this_thread::sleep_for(dura);
    }
    cout << "thread id = " << std::this_thread::get_id() << " finished running..." << endl;
    return;
}

int main()
// int main04()
{
    // （3.3）老师的心得，一般用于计数或者统计（累计发送出去了多少个数据包，累计接受到了多个数据包；）

    std::thread myobj1(mythread04);
    std::thread myobj2(mythread04);
    std::chrono::milliseconds dura(5000);
    // 5秒钟
    std::this_thread::sleep_for(dura);
    g_ifend = true; //线程异步
    // 对原子对象的写操作，让线程自行运行结果；
    myobj1.join();
    myobj2.join();
    cout << "Program finished, exiting" << endl;

    return 0;
}