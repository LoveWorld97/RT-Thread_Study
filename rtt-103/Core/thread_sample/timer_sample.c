#include "rtthread.h"

/* 定时器的控制块 */
static rt_time_t timer1;
static rt_time_t timer2;
static int cnt = 0;

/*周期定时器 timer1 的超时函数，timer1 定时时间到会执行次函数，10 次之后停止定时器 timer1。*/
static void timeout1(void *parameter)
{
    rt_kprintf("periodic timer is timeout %d\n", cnt);
    /* 运行第 10 次，停止周期定时器 */
    if (cnt++ >= 9)
    {
        rt_timer_stop(timer1);
        rt_kprintf("periodic timer was stopped! \n");
    }
}
static void timeout2(void *parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}

int timer_sample(void)
{
    /* 创建定时器 1 周期定时器 */
    timer1 = rt_timer_create("timer1",
                             timeout1,
                             RT_NULL,
                             10,
                             RT_TIMER_FLAG_PERIODIC);
    if (timer1 != RT_NULL)
    {
        rt_timer_start(timer1);
    }

    /* 创建定时器 2 周期定时器 */
    timer2 = rt_timer_create("timer2",
                             timeout2,
                             RT_NULL,
                             30,
                             RT_TIMER_FLAG_ONE_SHOT);
    if (timer2 != RT_NULL)
    {
        rt_timer_start(timer2);
    }

}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(timer_sample, timer sample);