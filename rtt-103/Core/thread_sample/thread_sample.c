#include "rtthread.h"
#define THREAD_PRIORITY 25
#define THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 5

static rt_thread_t tid1 = RT_NULL;

/*线程1的入口函数*/
static void thread1_entry(void * parameter)
{
    rt_uint32_t count = 0;
    while (1)
    {
        /*线程1采用低优先级运行，一直打印级数*/
        rt_kprintf("thread1 count:%d\n", count++);
        rt_thread_mdelay(500);
    }
    
}
/*线程2线程栈、控制块、入口函数定义*/
ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

/*线程2入口函数*/
static void thread2_entry(void *param)
{
    rt_uint32_t count = 0;
    /*线程2拥有高优先级，以抢占线程1而获得执行*/
    for (count = 0; count < 10; count++)
    {
        /*线程2打印计数值*/
        rt_kprintf("thread2 count:%d\n", count);

    }
    rt_kprintf("thread2 exit\n");
    /*线程2运行结束后也将自动被系统脱离*/
}

/*例程代码，其中创建了线程 thread1，初始化了线程 thread2，并将函数使用 MSH_CMD_EXPORT 导出命令。*/
int thread_sample(void)
{
    /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/
    tid1 = rt_thread_create("thread1",
                            thread1_entry,
                            RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY,
                            THREAD_TIMESLICE);
                            if (tid1 != RT_NULL)
                            {
                                rt_thread_startup(tid1);
                            }
                            /* 初始化线程 2，名称是 thread2，入口是 thread2_entry */
                            rt_thread_init(&thread2,
                                           "thread2",
                                           thread2_entry,
                                           RT_NULL,
                                           &thread2_stack[0],
                                           sizeof(thread2_stack),
                                           THREAD_PRIORITY - 1,
                                           THREAD_TIMESLICE);
                            rt_thread_startup(&thread2);
                            return 0;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_sample, thread sample);
