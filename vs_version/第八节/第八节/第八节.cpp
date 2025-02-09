// �ڰ˽�.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>

using namespace std;

class A
{
public:
	//���յ�����Ϣ���������뵽һ�����е��߳�
	void inMsgRecvQueue()	//unlock()
	{
		for (int i = 0; i < 100000; ++i)
		{
			std::unique_lock<std::mutex> sbguard1(my_mutex1);
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;
			msgRecvQueue.push_back(i);
			//�����������i�������յ��������ֱ��Ū����Ϣ�����������

			//����outMsgRecvQueue()���ڴ���һ��������Ҫһ��ʱ�䣬������������wait()����ȴ��㻽�ѣ���ô��ʱ���notify_one()�������Ҳ���ûЧ����
			//my_cond.notify_one();
			//���ǳ��԰�wait()���̻߳��ѣ�ִ�������У���ôoutMsgRecvQueue()��ߵ�wait�ͻᱻ����
			//����֮�����������о���
			my_cond.notify_all();
			//......
			//����������룻
		}
		return;
	}

	//bool outMsgLULProc(int &command)
	//{
	//  //˫��������˫�ؼ��
	//  if (!msgRecvQueue.empty())
	//  {
	//  std::unique_lock<std::mutex> sbguard1(my_mutex1); 

	//  if (!msgRecvQueue.empty())
	//  {
	//  //��Ϣ��Ϊ��
	//  command = msgRecvQueue.front(); //���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
	//  msgRecvQWueue.pop_front(); //�Ƴ���һ��Ԫ�أ��������أ�
	//  return true;
	//  }
	//  }
	//  return false;
	//}

	//�����ݴ���Ϣ������ȡ�����̣߳�
	void outMsgRecvQueue()
	{
		int command = 0;

		//for (int i = 0; i < 100000; ++i)
		//{
		//  bool result = outMsgLULProc(command);
		//  if (result == true)
		//  {
		//  cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << command << endl;
		//  //���Կ��ǽ���������ݣ�����
		//  //.....
		//  }
		//  else
		//  {
		//  //sleep(100)
		//  //��Ϣ����Ϊ��
		//  cout << "outMsgEecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ��" << i << endl;
		//  }
		//}
		//cout << endl;

		while (true)
		{
			std::unique_lock<std::mutex> sbguard1(my_mutex1);

			//wait()������һ������
			//����ڶ�������lambda���ʽ����ֵ��true����wait()ֱ�ӷ��أ�
			//����ڶ�������lambda���ʽ����ֵ��false����ôwait()�������������������������У�
			//�Ƕ�����ʲôʱ��Ϊֹ�أ�����������ĳ���̵߳���notify_one()��Ա����Ϊֹ��
			//���wait()û�еڶ�������	my_cond.wait(sbguard1)����ô�͸��ڶ�������lambda���ʽ����falseЧ��һ��
				//wait()�������������������������У�����������ĳ���̵߳���notify_one()��Ա����Ϊֹ��
				//�������߳���notify_one()����wait��ԭ����˯��/��������״̬���Ѻ�wait�Ϳ�ʼ�ָ��ɻ��ˣ��ָ���wait��ʲô�
				//a��wait()���ϵĳ������»�ȡ���������������ȡ��������ô���̾Ϳ���wait������Ż�ȡ�������ȡ�����������ڼ�����������ôwait�ͼ���ִ��b��
				//b��
				//b.1�����wait�еڶ�������(lambda)�����ж����lambda���ʽ�����lambda���ʽΪfalse����ôwait�ֶԻ�����������Ȼ��������������ȴ��ٴα�notify_one����
				//b.2�����lambda���ʽΪtrue����wait���أ���������������ʱ�����������ţ���
				//b.3�����waitû�еڶ�����������wait���أ�������������
				my_cond.wait(sbguard1, [this] {	//һ��lambda����һ���ɵ��ö��󣨺�����
				if (!msgRecvQueue.empty())
					return true;
				return false;
			});

			//����ֻҪ���ߵ������������������һ�������ŵġ�
			//һ����д������...
			command = msgRecvQueue.front();	//���ص�һ��Ԫ�أ��������Ԫ���Ƿ���ڣ�
			msgRecvQueue.pop_front();	//�Ƴ���һ��Ԫ�أ��������أ�
			cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��: " << command << "threadid = " << std::this_thread::get_id() << endl;
			sbguard1.unlock();	//��Ϊunique_lock������ԣ��������ǿ�����ʱ��unlock������������ס̫��ʱ��

			//ִ��һЩ�����Ķ�����������ҳ鿨���鿨��Ҫ100����Ĵ���ʱ�䣻
			//...
			//ִ��100����
			//

		}  //end while
	}

private:
	std::list<int> msgRecvQueue;	//������ר�����ڴ�����Ҹ����Ƿ��͹��������
	std::mutex my_mutex1;	//����һ����������һ����ͷ��
	std::condition_variable my_cond;	//����һ��������������
};

int main()
{
	//һ����������std::condition_variable��wait()��notify_one()��ֻ��֪ͨһ��outMsgRecvQueue�߳�
	//�߳�A���ȴ�һ����������
	//�߳�B��ר������Ϣ����������Ϣ�����ݣ�
	//std::condition_variableʵ������һ���࣬��һ����������ص�һ���࣬˵���˾��ǵȴ�һ��������ɡ�
	//�������Ҫ�ͻ���������Ϲ������õ�ʱ������Ҫ���������Ķ���


	//����������������˼��


	//����notify_all()


	A myobja;
	std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);	//�ڶ������� ���ã����ܱ�֤�߳��� �õ���ͬһ������
	std::thread myOutnMsgObj2(&A::outMsgRecvQueue, &myobja);
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

	myInMsgObj.join();
	myOutnMsgObj.join();
	myOutnMsgObj2.join();

	return 0;
}
