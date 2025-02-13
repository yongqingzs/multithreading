#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// ���������ֵ
int sharedNumber = 0;
// ���ڱ���������Դ�Ļ�����
std::mutex numberMutex;
// ���������������̼߳��ͬ��
std::condition_variable cv;
// ����������¼����ɼӷ��������߳�����
int threadsCompleted = 0;
// �ܹ���Ҫ��ɼӷ��������߳�����
const int totalThreadsToComplete = 2;

// �����ֵ���̺߳���
void incrementNumber() {
    for (int i = 0; i < 1000; ++i) {
        // ����������������Դ
        std::lock_guard<std::mutex> lock(numberMutex);
        sharedNumber++;
    }

    // ������׼���޸Ĺ��������̼߳�������֪ͨ��������
    std::unique_lock<std::mutex> lock(numberMutex);
    threadsCompleted++;
    std::cout << "Thread " << std::this_thread::get_id() << " completed increment operation, threads completed: " << threadsCompleted << std::endl;

    // ����ɼ�ֵ�������߳������ﵽ����ʱ��֪ͨ�ȴ����߳�
    if (threadsCompleted == totalThreadsToComplete) {
        cv.notify_one(); // ����һ���ȴ����������� cv �ϵ��߳� (���߳� 3)
    }
    // unique_lock �ڴ����������ʱ�Զ��ͷ���
}

// ���������ֵ���̺߳���
void outputNumber() {
    // ������׼���ȴ�����������֪ͨ
    std::unique_lock<std::mutex> lock(numberMutex);
    std::cout << "Thread " << std::this_thread::get_id() << " waiting for other threads to complete..." << std::endl;

    // �ȴ�����������֪ͨ��ֻ�е� threadsCompleted == totalThreadsToComplete ʱ�Żᱻ����
    cv.wait(lock, []{ return threadsCompleted == totalThreadsToComplete; });

    // ��������ʱ��˵��������ֵ�߳�����ɣ����԰�ȫ�ط��� sharedNumber
    std::cout << "Thread " << std::this_thread::get_id() << " woken up, outputting final value: " << sharedNumber << std::endl;
    // unique_lock �ڴ����������ʱ�Զ��ͷ���
}

int main() {
    std::cout << "Main thread " << std::this_thread::get_id() << " started" << std::endl;

    // �������������ֵ���߳�
    std::thread thread1(incrementNumber);
    std::thread thread2(incrementNumber);

    // �������������ֵ���߳�
    std::thread thread3(outputNumber);

    // �ȴ������߳����
    thread1.join();
    thread2.join();
    thread3.join();

    std::cout << "Main thread " << std::this_thread::get_id() << " finished" << std::endl;
    return 0;
}