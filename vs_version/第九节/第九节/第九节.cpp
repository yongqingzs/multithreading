// �ھŽ�.cpp : Defines the entry point for the console application.
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

int mythread() //�߳���ں���
{
	cout << "mythread() start" << " threadid = " << std::this_thread::get_id() << endl;	//��ӡ���߳�id
	std::chrono::milliseconds dura(5000);	//��һ��5���ʱ��
	std::this_thread::sleep_for(dura);	//��Ϣ��һ��ʱ��
	cout << "mythread() end" << " threadid = " << std::this_thread::get_id() << endl;	//��ӡ���߳�id
	return 5;
}

int main01()
{
	//һ��std::async��std::future������̨������ֵ
	//ϣ���̷߳���һ�������
	//std::async �Ǹ�����ģ�壬��������һ���첽������������һ���첽����֮��������һ��std::future����std::future�Ǹ���ģ�塣
	//ʲô�С�����һ���첽���񡱣������Զ�����һ���̲߳���ʼִ�ж�Ӧ���߳���ں�����������һ��std::future����
	//���std::future������߾ͺ����߳���ں��������صĽ�����̷߳��صĽ���������ǿ���ͨ������future����ĳ�Ա����get()����ȡ�����
	//��future������������˼������Ҳ�ƺ�std::future�ṩ��һ�ַ����첽��������Ļ��ƣ�����˵�����������û�а취�����õ��������õĽ�����
	//���߳�ִ����ϵ�ʱ������ܹ��õ�����ˣ����Դ�Ҿ���ô��⣺���future��������ᱣ��һ��ֵ���ڽ�����ĳ��ʱ��

	//���г���ͨ��std::future�����get()��Ա�����ȴ��߳�ִ�н��������ؽ����
	//���get()������ţ�����õ������ķ���ֵ �Ĳ����ݣ����õ�ֵ�ҾͿ�������ȴ���ֵ��

	cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
	std::future<int> result = std::async(mythread);
	//�����̲߳���ʼִ�У��󶨹�ϵ�����̲�����������
	cout << "continue...!" << endl;
	int def;
	def = 0;
	cout << result.get() << endl;
	//��������ȴ�mythread()ִ����ϣ��õ����
	//cout << result.get() << endl;
	//get()ֻ�ܵ���һ�Σ����ܵ��ö�Σ�

	//result.wait();
	//�ȴ��̷߳��أ����������ؽ����
	cout << "I Love China!" << endl;

	return 0;
}




class A
{
public:
	int mythread(int mypar)	//�߳���ں���
	{
		cout << mypar << endl;
		cout << "mythrea() start" << " threadid = " << std::this_thread::get_id() << endl;
		//��ӡ���߳�id
		std::chrono::milliseconds dura(5000);
		//��һ��5���ʱ��
		std::this_thread::sleep_for(dura);
		//��Ϣ��һ��ʱ��
		cout << "mythread() end" << " threadid = " << std::this_thread::get_id() << endl;
		//��ӡ���߳�id
		return 5;
	}
};

int main02()
{
	//����ͨ��������std::async()����һ���������ò���������std::lasunch���ͣ�ö�����ͣ������ﵽһЩ�����Ŀ�ģ�
	//a��std::launch::deferred����ʾ�߳���ں������ñ��ӳٵ�std::future��wait()����get()��������ʱ��ִ��
	//�����wait()����get()û�б����ã���ô�߳� ��ִ����ûִ�С�ʵ���ϣ��̸߳�����û������
	//std::launch::deferred���ӳٵ��ã�����û�д������̣߳��������߳��е��õ� �߳���ں�����
	//b��std::launch::async���ڵ���async������ʱ��Ϳ�ʼ�������̣߳�
	//async()������Ĭ���õľ���std::launch::async���


	A a;
	int tmppar = 12;
	cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
	//std::future<int> result = std::async(mythread, tmppar);
	//�����̲߳���ʼִ�У��󶨹�ϵ�����̲�����������
	//std::future<int> result = std::async(&A::mythread, &a, tmppar);
	//�ڶ��������Ƕ������ã����ܱ�֤�߳����õ���ͬһ������
	//std::future<int> result = std::async(std::launch::deferred, &A::mythread, &a, tmppar);
	//�ڶ��������Ƕ������ã����ܱ�֤�߳����õ���ͬһ������
	//std::future<int> result = std::async(std::launch::async, &A::mythread, &a, tmppar);
	std::future<int> result = std::async(std::launch::deferred | std::launch::async, &A::mythread, &a, tmppar);

	cout << "continue...!" << endl;
	cout << "continue...!" << endl;
	cout << "continue...!" << endl;
	cout << "continue...!" << endl;
	cout << "continue...!" << endl;
	cout << "continue...!" << endl;
	int def;
	def = 0;
	cout << result.get() << endl;
	//��������ȴ�mythread()ִ����ϣ��õ����
	//cout << result.get() << endl;
	//get()ֻ�ܵ���һ�Σ����ܵ��ö�Σ�
	//result.wait();
	//�ȴ��̷߳��أ����������ؽ����
	cout << "I Love China!" << endl;

	return 0;
}




int mythread1(int mypar) //�߳���ں���
{
	cout << "mythrea() start" << " threadid = " << std::this_thread::get_id() << endl;	//��ӡ���߳�id
	std::chrono::milliseconds dura(5000);	//��һ��5���ʱ��
	std::this_thread::sleep_for(dura);	//��Ϣ��һ��ʱ��
	cout << "mythread() end" << " threadid = " << std::this_thread::get_id() << endl;	//��ӡ���߳�id
	return 5;
}

int main03()
{
	//����std::packaged_task��������񣬰������װ����
	//�Ǹ���ģ�壬����ģ������� ���ֿɵ��ö���ͨ��std::packaged_task ���Ѹ��ֿɵ��ö����װ���������㽫����Ϊ�߳���ں��������ã�

	cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
	std::packaged_task<int(int)> mypt(mythread1);
	//���ǰѺ���mythreadͨ��packaged_task��װ����
	std::thread t1(std::ref(mypt), 1);
	//�߳�ֱ�ӿ�ʼִ�У��ڶ������� ��Ϊ�߳���ں����Ĳ�����
	t1.join();
	//�ȴ��߳�ִ����ϡ�
	std::future<int> result = mypt.get_future();
	//std::future������������߳���ں����ķ��ؽ��������result����mythread���صĽ����
	cout << result.get() << endl;
	cout << "I Love China!" << endl;

	return 0;
}




int main04()
{
	//packaged_task��װ�����Ŀɵ��ö��󻹿���ֱ�ӵ��ã����Դ�����Ƕ�������packaged_task����Ҳ�� һ���ɵ��ö���

	cout << "main" << " threadid = " << std::this_thread::get_id() << endl;

	std::packaged_task<int(int)> mypt([](int mypar) {
		cout << mypar << endl;
		cout << "mythrea() start" << " threadid = " << std::this_thread::get_id() << endl;
		//��ӡ���߳�id
		std::chrono::milliseconds dura(5000);
		//��һ��5���ʱ��
		std::this_thread::sleep_for(dura);
		//��Ϣ��һ��ʱ��
		cout << "mythread() end" << " threadid = " << std::this_thread::get_id() << endl;
		//��ӡ���߳�id
		return 5;
	});

	//std::thread t1(sed::ref(mypt), 1);
	//�߳�ֱ�ӿ�ʼִ�У��ڶ������� ��Ϊ�߳���ں����Ĳ�����
	//t1.join();
	//�ȴ��߳�ִ����ϡ�
	//std::future<int> result = mypt.get_future();
	//std::future������������߳���ں����ķ��ؽ��������result����mythread���صĽ����
	//cout << result.get() << endl;
	//cout << "I Love China!" << endl;

	mypt(105);
	//ֱ�ӵ��ã��൱�ں������ã�
	std::future<int> result = mypt.get_future();
	cout << result.get() << endl;

	return 0;
}




vector<std::packaged_task<int(int)>> mytasks;  //����
int main05()
{
	cout << "main" << " threadid = " << std::this_thread::get_id() << endl;

	std::packaged_task<int(int)> mypt([](int mypar) {
		cout << mypar << endl;
		cout << "mythrea() start" << " threadid = " << std::this_thread::get_id() << endl;
		//��ӡ���߳�id
		std::chrono::milliseconds dura(5000);
		//��һ��5���ʱ��
		std::this_thread::sleep_for(dura);
		//��Ϣ��һ��ʱ��
		cout << "mythread() end" << " threadid = " << std::this_thread::get_id() << endl;
		//��ӡ���߳�id
		return 5;
	});

	mytasks.push_back(std::move(mypt));
	//���������������ƶ����壬���ȥ֮��mypt��Ϊ��
	//...
	std::packaged_task<int(int)> mypt2;
	auto iter = mytasks.begin();
	mypt2 = std::move(*iter);
	//�ƶ�����
	mytasks.erase(iter);
	//ɾ����һ��Ԫ�أ��������Ѿ�ʧЧ�ˣ����Ժ������벻������ʹ��iter��
	mypt2(123);
	std::future<int> result = mypt2.get_future();
	cout << result.get() << endl;

	return 0;
}



void mythread2(std::promise<int> &tmpp, int calc)	//���ע���һ������
{
	//��һϵ�и��ӵĲ���
	calc++;
	calc *= 10;
	//���������У���������������5���ӣ�

	std::chrono::milliseconds dura(5000);
	//��һ��5���ʱ��
	std::this_thread::sleep_for(dura);
	//��Ϣ��һ��ʱ��

	//���ڼ���������
	int result = calc;
	//������
	tmpp.set_value(result);
	//����ұ��浽��tmpp��������У�
	return;
}

void mythread3(std::future<int> &tmpf)
{
	auto result = tmpf.get();
	cout << "mythread2 result" << result << endl;
	return;
}

int main()
{
	//����std::promise����ģ��
	//�����ܹ���ĳ���߳��и�����ֵ��Ȼ�����ǿ����������߳��У������ȡֵ�����ã�
	//�ܽ᣺ͨ��promise����һ��ֵ���ڽ���ĳ��ʱ������ͨ����һ��future�󶨵����promise�����õ�����󶨵�ֵ��
	std::promise<int> myprom;
	//����һ��std::promise����myprom�������ֵ����Ϊint��
	std::thread t1(mythread2, std::ref(myprom), 180);
	t1.join();

	//��ȡ���ֵ
	std::future<int> ful = myprom.get_future();
	//promise��future�󶨣����ڻ�ȡ�̷߳���ֵ
	//auto result = ful.get();
	//getֻ�ܵ���һ�Σ����ܵ��ö��
	//cout << "result = " << result << endl;

	std::thread t2(mythread3, std::ref(ful));
	t2.join();
	//��mythread2�߳�ִ�����

	cout << "I Love China!" << endl;

	//�ģ�С�᣺������ô�ã�ʲôʱ���ã�
	//����ѧϰ��Щ������Ŀ�ģ�������Ҫ�����Ƕ����������Լ���ʵ�ʿ����С�
	//�෴����������ܹ������ٵĶ����ܹ�д��һ���ȶ�����Ч�ض��̳߳��򣬸�ֵ�����ͣ�
	//����Ϊ�˳ɳ�������ҪҪ�Ķ�һЩ����д�Ĵ��룬�Ӷ�����ʵ���Լ�����Ļ��ۣ����ǵļ����ͻ���һ������ȵ�������
	//��ʦ��Ը�⽫ѧϰ��Щ���ݵ����ɽ���Ϊ��Ϊ���ǽ����ܹ���������������ʦд�Ĵ������·��

	return 0;
}