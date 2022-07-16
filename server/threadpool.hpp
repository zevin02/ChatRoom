#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <unistd.h>
#include <pthread.h>
// #cludine"Task.hpp"
// using namespace ns_task;
namespace ns_threadpool
{
    const int g_num = 5;
    template <class T>
    class ThreadPool //线程池
    {

    private:
        int num_;                  //一个线程池里面有多少个任务
        std::queue<T> task_queue_; //任务队列,临界资源
        pthread_mutex_t mtx_;
        pthread_cond_t cond_;
        static ThreadPool<T> *ins; //静态成员在所有的对象里面只有一个静态成员，必须要通过静态变量来获取对象
        //保存内存的可见性
    private:
        //单例的话，就不能让构造函数暴露在外面，否则，只有有构造函数，就能初始化
        //构造函数必须得实现，当时必须得私有
        ThreadPool(int num = g_num) : num_(num)
        {
            pthread_mutex_init(&mtx_, nullptr);
            pthread_cond_init(&cond_, nullptr);
        }

        ThreadPool(const ThreadPool &tp) = delete;
        // c++11的新特性
        //静止编译器生成拷贝构造，
        //=delete就是禁止调用这个函数，在私有里面

        ThreadPool operator=(const ThreadPool &tp) = delete;
        //把赋值运算符也禁止掉，这也就可以避免创建多个对象

    public:
        static ThreadPool<T> *GetInstance() //这个必须是使用静态的，非静态函数都是有对象的，静态函数才是没对象的
        {

            static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //使用静态的初始化

            if (ins == nullptr)//双判定，减少锁的争用，提高单例获取的效率，
            //假如说有的线程进来发现不为空，就可以直接走了，如果同时为nullptr的化，那么再把他们放进来争抢锁资源、

            {

                pthread_mutex_lock(&lock); //争抢锁的过程就是一个串行化的过程，成本很高

                //当前的单例对象还没有被创建
                if (ins == nullptr)
                //假如是在多线程的情况下，那么多个线程执行的时候，都是nullptr，都创建了对象，那么就出现了线程安全
                {
                    //就创建它
                    ins = new ThreadPool<T>(); //创建一个，使用构造函数
                    //创建出来了一个单例之后，就直接给他初始化一个池就行了
                    ins->InitThreadPool();
                    std::cout << "首次加载对象" << std::endl;
                }
                pthread_mutex_unlock(&lock);
            }
            return ins;
        }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&cond_);
        }

        //在类中，要让

        static void *Rountine(void *args)
        //也不能访问类里面非static成员
        {

            pthread_detach(pthread_self()); //实现线程分离就不要再去join等待了
            ThreadPool<T> *tp = (ThreadPool<T> *)args;
            while (true)
            {
                //从任务队列里面去拿一个任务
                //执行任务，要先把这个任务队列锁主

                //每个线程他跟放任务的线程一样，都是竞争式的去拿一个任务
                tp->Lock();
                //先检测任务队列是否有一个任务
                while (tp->IsEmpty())
                {
                    //检测到任务队列为空
                    //此时线程就挂起等待
                    tp->Wait();
                }
                //该任务队列里面一定有任务了
                T t;
                tp->PopTask(&t);
                //任务就拿到了
                tp->UnLock();
                t.Run(); //可能有多个线程在处理任务,

                sleep(1);
            }
        }
        void InitThreadPool()
        {
            //初始化一批线程，
            //这样就不要每次用都要去开辟线程了
            pthread_t tid; //一次创建一批线程
            for (int i = 0; i < num_; i++)
            {
                pthread_create(&tid, nullptr, Rountine, (void *)this);
                //在类中不能执行线程的方法，因为他都有隐藏的this指针
                //所以我们需要使用静态的函数，就没有了this指针
            }
        }
        void PopTask(T *out)
        {
            *out = task_queue_.front();
            task_queue_.pop();
        }
        void Wait()
        {
            pthread_cond_wait(&cond_, &mtx_);
        }
        bool IsEmpty()
        {
            return task_queue_.empty();
        }
        void Lock()
        {
            pthread_mutex_lock(&mtx_);
        }
        void UnLock()
        {
            pthread_mutex_unlock(&mtx_);
        }
        void Wakeup()
        {
            pthread_cond_signal(&cond_);
        }
        void PushTask(const T &in)
        {
            //塞任务，就相当于一个生产者，生产者之间要进行互斥访问
            Lock();
            task_queue_.push(in);
            UnLock();
            Wakeup();
        }

        //万一任务队列里面一个任务都没有的话，那么线程池里面的每一个线程就要处于休眠状态，挂起等待
    };
    template <class T>
    //静态成员变量的初始化必须要在类外面初始化
    ThreadPool<T> *ThreadPool<T>::ins = nullptr; //将threadpool里面的ins进行初始化，返回值是指针，给它初始化为空，说明没有被创建出来

}