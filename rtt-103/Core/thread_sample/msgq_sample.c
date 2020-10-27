#include "rtthread.h"

#define THREAD_PRIORITY 25
#define THREAD_TIMESLICE 5

static struct rt_messagequeue mq;

static rt_uint8_t msg_pool[2048];

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;
/* 线程 1 入口函数 */
static void thread1_entry(void *parameter)
{
    char buf = 0;
    rt_uint8_t cnt = 0;
    while (1)
    {
        /* 从消息队列中接收消息 */
        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: recv msg from msg queue, the content:%c\n", buf);
            if (cnt == 19)
            {
                break;
            }
        }
        /* 延时 50ms */
        cnt++;
        rt_thread_mdelay(50);
    }
    rt_kprintf("thread1: detach mq \n");
    rt_mq_detach(&mq);
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

static void thread2_entry(void *parameter)
{
    int result;
    char buf = 'A';
    rt_uint8_t cnt = 0;
    while (1)
    {
        if (cnt == 8)
        {
            result = rt_mq_urgent(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_urgent ERR\n");
            }
            else
            {
                rt_kprintf("thread2: send urgent message - %c\n", buf);
            }
        }
        else if (cnt >= 20)
        {
            rt_kprintf("message queque stop send, thread2 quit\n");
            break;
        }
        else
        {
            result = rt_mq_send(&mq, &buf, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_send ERR\n");
            }
            rt_kprintf("thread2: send message - %c\n", buf);
        }
        buf++;
        cnt++;
        rt_thread_mdelay(5);
    }
}
/* 消息队列示例的初始化 */ int msgq_sample(void)
{
    rt_err_t result;
    /* 初始化消息队列 */ result = rt_mq_init(&mq,
                                             "mqt",
                                             &msg_pool[0],
                                             1, sizeof(msg_pool),
                                             RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }
    rt_thread_init(&thread1, "thread1", thread1_entry, RT_NULL, &thread1_stack[0], sizeof(thread1_stack), THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread1);
    rt_thread_init(&thread2, "thread2", thread2_entry, RT_NULL, &thread2_stack[0], sizeof(thread2_stack), THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread2);
    return 0;
}
/* 导出到 msh 命令列表中 */ MSH_CMD_EXPORT(msgq_sample, msgq sample);