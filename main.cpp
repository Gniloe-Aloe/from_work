#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "timer.h"
#include <future>


std::mutex mtx;
std::mutex m1, m2;
std::recursive_mutex rec_mtx;
std::condition_variable cond_var;
bool _var = false;

int dowork(int& a) {
    std::unique_lock<std::mutex> u_lock(mtx, std::defer_lock);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    u_lock.lock();
    std::cout << std::this_thread::get_id() << " dowork start" << std::endl;

    for (int i = 0; i < 8; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << std::this_thread::get_id() << " work. iteration: " << i << std::endl;
    }
    a *= 2;
    u_lock.unlock();
    //thread_local int x = 10;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << std::this_thread::get_id() << " dowork stop" << std::endl;
    return a;
}

void print_sqr(char& symbhol) {

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::lock_guard<std::mutex> guard(mtx);

    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 10; ++i) {
            std::cout << symbhol;
        }
        std::cout << std::endl;
    }
    guard.~lock_guard();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int fibo(unsigned number) {
    rec_mtx.lock();
    if (number <= 1) {
        rec_mtx.unlock();
        return 1;
    }
    rec_mtx.unlock();
    return fibo(number - 1) + fibo(number - 2);
}

void thread_wait() {
    std::unique_lock<std::mutex> u_lock(mtx);
    cond_var.wait(u_lock);
    std::cout << "thread_wait fulfilled!" << std::endl;
}

void thread_yeld() {
    std::cout << "Enter: " << std::endl;
    char inp;
    std::cin >> inp;
    cond_var.notify_all();
}

void lock_test1(std::vector<int>& vec) { 
    std::lock(m1, m2);
    std::cout << "lock_test1" << std::endl;
    m1.unlock();
    m2.unlock();
}

void lock_test2(std::vector<int>& vec) {
    std::lock(m1, m2);
    std::cout << "lock_test2" << std::endl;
    m1.unlock();
    m2.unlock();
}

void test_promise(std::promise<int>& value) {
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    value.set_value(10);
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void mtx_test1() {
    mtx.lock();
    std::cout << "message!" << std::endl;
    mtx.unlock();
}

void mtx_test2() {
    mtx.lock();
    std::this_thread::sleep_for(std::chrono::seconds(100));
    mtx.unlock();
}


int main()
{
    //выводит в консоль время вополнения процесса при вызове диструктора
    timer t;

    std::cout << "id: " << std::this_thread::get_id() << " main start" << std::endl;

    //void (*pdowork)() = dowork;
    
    //std::reference_wrapper<int> r_value(value);
    //std::thread th([&value, &result]() {result = dowork(value); });
    //std::thread th1([&value1]() {print_sqr(std::ref(value1)); });
    //std::thread th2([&value2]() {print_sqr(std::ref(value2)); });
    std::vector<int> vec{ 1, 2, 3 };

    //std::promise<int> prom;
    //std::future<int> fut = prom.get_future();

    std::thread th1(mtx_test1);
    std::thread th2(mtx_test2);

    th1.join();
    th2.join();
    std::cout << "id: " << std::this_thread::get_id() << " main stop" << std::endl;
    return 0;
}
