#include "rtthread.h"

#define THREAD_STACK_SIZE 512 //栈空间
#define THREAD_PRIORITY 20     //优先级
#define THREAD_TIMESLICE 10    //时间片

/*两个线程公共的入口函数，线程 thread1 和 thread2 采用同一个入口函数，但是变量分别存在不同的 堆空间*/
static void thread_entry(void *parameter)
{
    rt_uint32_t value;
    rt_uint32_t count = 0;
    value = (rt_uint32_t)parameter;

    while (1)
    {
        if (0 == (count % 5))
        {
            rt_kprintf("thread:%d is runnning, thread %d count = %d\n", value, value, count);
            if (count > 200)
            {
                return;
            }
        }
        count++;
    }
}
/*线程时间片的示例函数，示例函数首先创建并启动了线程 thread1，
然后创建并启动了线程 thread2。
并将函数使用 MSH_CMD_EXPORT 导出命令。*/
int timeslice_sample(void)
{
    rt_thread_t tid = RT_NULL;
    /*创建线程1*/
    tid = rt_thread_create("thread1",
                           thread_entry,
                           (void *)1,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY,
                           THREAD_TIMESLICE);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    /*创建线程2*/
    tid = rt_thread_create("thread2",
                           thread_entry,
                           (void *)2,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY,
                           THREAD_TIMESLICE - 5);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    return 0;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(timeslice_sample, timeslice sample)