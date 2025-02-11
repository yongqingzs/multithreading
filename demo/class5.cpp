// 第五节.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
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
	//把收到的消息（玩家命令）入到一个队列的线程
	void inMsgRecvQueue()
	{
		my_mutex.lock();
		for (int i = 0; i < 100000; ++i)
		{
			cout << "inMsgRecvQueue(), in: " << i << endl;

			// {
			// 	std::lock_guard<std::mutex> sbguard(my_mutex);
			// 	msgRecvQueue.push_back(i);
			// 	//假设这个数字i就是我收到的命令，我直接弄到消息队列里边来；
			// }

			// my_mutex.lock();
			msgRecvQueue.push_back(i);
			// my_mutex.unlock();
			//......
			//其他处理代码；
		}
		my_mutex.unlock();
	}

	bool outMsgLULProc(int &command)
	{
		std::lock_guard<std::mutex> sbguard(my_mutex);
		//sbguard是老师随便起的对象名
		//lock_guard构造函数里执行了mutext::lock()；
		//lock_guard析构函数里执行了mutext::unlock()；
		//my_mutex.lock();
		if (!msgRecvQueue.empty())
		{
			//消息不为空
			command = msgRecvQueue.front();
			//返回第一个元素，但不检查元素是否存在；
			msgRecvQueue.pop_front();
			//移除第一个元素，但不返回；
			//my_mutex.unlock();
			return true;
		}
		//my_mutex.unlock();
		return false;
	}

	//把数据从消息队列中取出的线程：
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue(), out: " << command << endl;
				//可以考虑进行命令（数据）处理
				//.....
			}
			else
			{
				//消息队列为空
				cout << "outMsgEecvQueue(), but empty!" << i << endl;
			}
		}
		cout << endl;
	}

	bool add_data()
	{
		std::lock_guard<std::mutex> sbguard(my_mutex);
		if (!msgRecvQueue.empty())
		{
			// 获取第一个元素并加 100
			auto tmp = msgRecvQueue.front();
			msgRecvQueue.front() += 100;
			cout << "first data: " << tmp << ", add first data: " << msgRecvQueue.front() << endl;
			msgRecvQueue.pop_front();
			return true;
		}
		return false;
	}

	void add_data_for()
	{
		// std::lock_guard<std::mutex> sbguard(my_mutex);
		for (int i = 0; i < 100; i++)
		{
			add_data();
		}
	}

	bool sub_data()
    {
        std::lock_guard<std::mutex> sbguard(my_mutex);
        if (!msgRecvQueue.empty())
        {
            // 获取第一个元素并减 100
            auto tmp = msgRecvQueue.front();
            msgRecvQueue.front() -= 100;
            cout << "first data: " << tmp << ", sub first data: " << msgRecvQueue.front() << endl;
            msgRecvQueue.pop_front();
            return true;
        }
        return false;
    }

    void sub_data_for()
    {
        // std::lock_guard<std::mutex> sbguard(my_mutex);
        for (int i = 0; i < 100; i++)
        {
            sub_data();
        }
    }

	bool read_data()
	{
		// std::lock_guard<std::mutex> sbguard(my_mutex);
		if (!msgRecvQueue.empty())
		{
			// 获取第一个元素并加 100
			cout << "first data read: " << msgRecvQueue.front() << endl;
			return true;
		}
		return false;
	}

	void read_data_for()
	{
		// std::lock_guard<std::mutex> sbguard(my_mutex);
		for (int i = 0; i < 100; i++)
		{
			read_data();
		}
	}
	

public:
	std::list<int> msgRecvQueue;

private:
	//容器，专门用于代表玩家给咱们发送过来的命令。
	std::mutex my_mutex;
	//创建一个互斥量（一把锁头）
	std::mutex my_mutex2;
};

// int main()
int main01()
{
	//保护共享数据，操作时，某个线程 用代码把共享数据锁住、操作数据、解锁，
	//其他想操作共享数据的线程必须等待解锁，锁定住，操作，解锁。
	//“互斥量”
	//一：互斥量（mutex）的基本概念
	//互斥量是个类对象。理解成一把锁，多个线程尝试用lock()成员函数赖加锁这把锁头，只有一个线程能锁定成功（成功的标志是lock()函数返回）
	//如果没锁成功，那么流程卡在lock()这里不断的尝试去锁这把锁头；解锁unlock()
	//互斥量使用要小心，保护数据不多也不少，少了，没达到保护效果，多了，影响效率

	//二：互斥量的用法
	//（2.1）lock()，unlock()
	//步骤：先lock()，操作共享数据，unlock()；
	//lock()和unlock()要成对使用，有lock()必然要有unlock()，每调用一次lock()，必然应该调用一次unlock()；
	//不应该也不允许调用1次lock()却调用2次unlock()，也不允许调用2次lock()却调用1次unlock()，这些非对称数量的调用都会导致代码不稳定甚至崩溃。
	//有lock，忘记unlock的问题，非常难排查；
	//为了防止大家忘记unlock()，引入了一个叫std::lock_guard的类模板：你忘记unlock不要紧，我替你unlock()；
	//学习过智能指针(unique_ptr<>)：你忘记释放内存不要紧，我给你释放；	保姆；

	//（2.2）std::lock_guard类模板：直接取代lock()和unlock()；；也就是说，你用了lock_gruard之后，再不能使用lock()和unlock()

	// (2.3) lock锁的不是数据本身，而是执行数据操作的代码块。意味着和需要被锁的数据相关的代码块没有全部被锁，并且其被执行，还是会出现
	// 数据崩溃的错误

	// (2.4) lock锁for循环，会导致另一个join线程直到最后也没有被执行

	// (2.5) 必须得用同一把锁去锁
	A myobja;
	std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);
	//第二个参数 引用，才能保证线程里 用的是同一个对象
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();


	return 0;
}

int main()
// int main05()
{
	A myobja;
	for (int i = 0; i < 100; ++i)
		myobja.msgRecvQueue.push_back(i + 5);
	std::thread myAddData(&A::add_data_for, &myobja);
	std::thread mySubData(&A::sub_data_for, &myobja);
	std::thread myReadData(&A::read_data_for, &myobja);  // 单纯read不用互斥锁

	myAddData.join();
	mySubData.join();
	myReadData.join();
	return 0;
}


class AA
{
public:
	//把收到的消息（玩家命令）入到一个队列的线程
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100; ++i)
		{
			cout << "inMsgRecvQueue(), in: " << i << endl;
			
			std::lock_guard<std::mutex> sbguard1(my_mutex1);
			std::lock_guard<std::mutex> sbguard2(my_mutex2);
			//my_mutex1.lock();
			//实际工程这两个锁头不一定挨着，可能他们需要保护不同的数据共享块；
			////。。。。；//
			//my_mutex2.lock();
			msgRecvQueue.push_back(i);
			//假设这个数字i就是我收到的命令，我直接弄到消息队列里边来；
			//my_mutex2.unlock();
			//my_mutex1.unlock();
			//......
			//其他处理代码；
		}
	}

	bool outMsgLULProc(int &command)
	{
		std::lock_guard<std::mutex> sbguard1(my_mutex1);
		//sbguard是老师随便起的对象名
		//lock_guard构造函数里执行了mutext::lock()；
		//lock_guard析构函数里执行了mutext::unlock()；3
		std::lock_guard<std::mutex> sbguard2(my_mutex2);

		//my_mutex2.lock();
		//my_mutex1.lock();
		//my_mutex1.lock();
		//my_mutex2.lock();
		if (!msgRecvQueue.empty())
		{
			//消息不为空
			command = msgRecvQueue.front();
			//返回第一个元素，但不检查元素是否存在；
			msgRecvQueue.pop_front();
			//移除第一个元素，但不返回；
			//my_mutex1.unlock();
			//my_mutex2.unlock();
			return true;
		}
		//my_mutex1.unlock();
		//my_mutex2.unlock();
		return false;
	}

	//把数据从消息队列中取出的线程：
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue(), out: " << command << endl;
				//可以考虑进行命令（数据）处理
				//.....
			}
			else
			{
				//消息队列为空
				cout << "outMsgEecvQueue(), but empty!" << i << endl;
			}
		}
		cout << endl;
	}

private:
	std::list<int> msgRecvQueue;
	//容器，专门用于代表玩家给咱们发送过来的命令。
	std::mutex my_mutex1;
	//创建一个互斥量（一把锁头）
	std::mutex my_mutex2;
	//创建一个互斥量
};

// int main()
int main02()
{
	//三：死锁
	//张三：站在北京 等李四，不挪窝；
	//李四：站在深圳 等张三，不挪窝；
	//C++中：
	//比如我有两把锁（死锁这个问题 是由至少两个锁头就是两个互斥量才能产生）；金锁（JinLock），银锁（YinLock）
	//两个线程A、B
	//(1)线程A执行的时候，这个线程先锁 金锁，把金锁lock()成功了，然后它正要去lock银锁。。。
	//出现了上下文切换
	//(2)线程B执行了，这个线程先锁 银锁，因为银锁还没有被锁，所以银锁会lock()成功，线程B要去lock金锁。。。。。
	//此时此刻，死锁就产生了；
	//(3)线程A因为拿不到银锁头，流程走不下去（所有后边代码有解锁金锁头的但是流程走不下去，所以金锁头解不开）
	//(4)线程B因为拿不到金锁头，流程走不下去（所有后边代码有解锁银锁头的但是流程走不下去，所以银锁头解不开）
	//大家都晾在这里，你等我，我等你；

	//（3.1）死锁演示
	//（3.2）死锁的一般解决方案
	//只要保证这两个互斥量上锁的顺序一致就不会死锁。

	AA myobja;
	std::thread myOutnMsgObj(&AA::outMsgRecvQueue, &myobja);
	//第二个参数 引用，才能保证线程里 用的是同一个对象
	std::thread myInMsgObj(&AA::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();

	return 0;
}




class AAA
{
public:
	//把收到的消息（玩家命令）入到一个队列的线程
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 100; ++i)
		{
			cout << "inMsgRecvQueue(), in: " << i << endl;

			std::lock(my_mutex1, my_mutex2);
			//相当于每个互斥量都调用了lock()；

			std::lock_guard<std::mutex> sbguard1(my_mutex1, std::adopt_lock);
			std::lock_guard<std::mutex> sbguard2(my_mutex2, std::adopt_lock);

			msgRecvQueue.push_back(i);
			//假设这个数字i就是我收到的命令，我直接弄到消息队列里边来；
			//my_mutex2.unlock();
			//my_mutex1.unlock();
			//......
			//其他处理代码；
		}
	}

	bool outMsgLULProc(int &command)
	{
		std::lock(my_mutex1, my_mutex2);

		std::lock_guard<std::mutex> sbguard1(my_mutex1, std::adopt_lock);
		std::lock_guard<std::mutex> sbguard2(my_mutex2, std::adopt_lock);

		if (!msgRecvQueue.empty())
		{
			//消息不为空
			command = msgRecvQueue.front();
			//返回第一个元素，但不检查元素是否存在；
			msgRecvQueue.pop_front();
			//移除第一个元素，但不返回；
			//my_mutex1.unlock();
			//my_mutex2.unlock();
			return true;
		}
		//my_mutex1.unlock();
		//my_mutex2.unlock();
		return false;
	}

	//把数据从消息队列中取出的线程：
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 100; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true)
			{
				cout << "outMsgRecvQueue(), out: " << command << endl;
				//可以考虑进行命令（数据）处理
				//.....
			}
			else
			{
				//消息队列为空
				cout << "outMsgEecvQueue(), but empty!" << i << endl;
			}
		}
		cout << endl;
	}

private:
	std::list<int> msgRecvQueue;
	//容器，专门用于代表玩家给咱们发送过来的命令。
	std::mutex my_mutex1;
	//创建一个互斥量（一把锁头）
	std::mutex my_mutex2;
	//创建一个互斥量
};

// int main()
int main03()
{
	//（3.3）std::lock()函数模板：用来处理多个互斥量的时候才出场
	//能力：一次锁住两个或者两个以上的互斥量（至少两个，多了不限，1个不行）；
	//它不存在这种因为在多个线程中 因为锁的顺序问题导致死锁的风险问题；
	//std::lock()：如果互斥量中有一个没锁住，它就在那里等着，等所有互斥量都锁住，它才能往下走（返回）；
	//要么两个互斥量都锁住，要么两个互斥量都没锁住。如果只锁了一个，另外一个没锁成功，则它立即把已经锁住的解锁。

	//（3.4）std::lock_guard的std::adopt_lock参数
	//std::adopt_lock是个结构体对象，起一个标记作用：作用就是表示这个互斥量已经lock()，
	//不需要再std::lock_guard<std::mutex>构造函数里面对mutex对象进行再次lock()了；
	//总结：std::lock()一次锁定多个互斥量；谨慎使用（建议一个一个锁）；

	AAA myobja;
	std::thread myOutnMsgObj(&AAA::outMsgRecvQueue, &myobja);
	//第二个参数 引用，才能保证线程里 用的是同一个对象
	std::thread myInMsgObj(&AAA::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();

	return 0;
}
