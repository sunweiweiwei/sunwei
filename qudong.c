#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//1-线程的头文件
#include <linux/sched.h>   //wake_up_process()
#include <linux/kthread.h> //kthread_create(),kthread_run()
#include <linux/err.h>     //IS_ERR(),PTR_ERR()
//#include <linux/timer.h>   //定时器timer
//A-定时器的头文件
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
//(1)-信号量
#include <linux/semaphore.h>
//(a)-字符设备节点
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
//(b)-宏定义
#define CHRDEV_MAJOR 240   //主设备号
#define CHRDEV_MAION 0     //次设备号
#define CHRDEV_COUNT 1     //次设备号个数
#define CHRDEV_NAME "chrdev"  //字符设备节点名字
#define THREAD1 1     //线程编号1
#define THREAD2 2     //线程编号2
#define START 1       //开始
#define PAUSE 0x20    //暂停
#define CONTINUE 3    //继续
#define STOP 4        //停止
#define STARTALL 5       //全部开始
#define PAUSEALL 6       //全部暂停
#define CONTINUEALL 7    //全部继续
#define STOPALL 8        //全部停止

struct chrdev_cdev{
	struct cdev chrdevcdev;
	int major;
	dev_t dev;
	struct class *dev_class;
};
struct chrdev_cdev *pdev;

//(2)-定义信号量
struct semaphore sem1; 
struct semaphore sem2; 

// 延时函数
void my_delay(unsigned int s)
{
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout( msecs_to_jiffies(s) );
	//unsigned long end_time = time + s;	
	//while (time < end_time ) {  //等待时间
	//	schedule(); // 释放CPU 
	//}
	//cpu_relax();
}
	
//2-线程结构体指针  
static struct task_struct *my_task = NULL; 
static struct task_struct *my_task2 = NULL; 
static int my_kthread(void *arg) //3-线程函数
{
	int n = 0;
	while(!kthread_should_stop())
	{
		down_interruptible(&sem1);  //(4)-获得信号量1
		set_current_state(TASK_INTERRUPTIBLE);
		n = (n >= 100)?1:n+1;
		printk("Thread1 : %d\n",n);
	    up(&sem1);  //(5)-释放信号量1 
		my_delay(1000); 
	}
	return 0;
}
static int my_kthread2(void *arg) //3-线程函数
{
	int n = 0;
	while(!kthread_should_stop())
	{
		down_interruptible(&sem2);  //(4)-获得信号量2
		set_current_state(TASK_INTERRUPTIBLE);
		n = (n >= 100)?1:n+1;
		printk("Thread2 : %d\n",n);
	    up(&sem2);  //(5)-释放信号量2
		my_delay(2000); 
	}
	return 0;
}
//自定义创建线程并运行的函数
int thread_run(int n)
{
	int err;
	if(n == 1){//4-创建线程
		my_task = kthread_create(my_kthread, NULL, "Thread1");
		if(IS_ERR(my_task)){
			printk("Unable to start kernel thread.\n");
			err = PTR_ERR(my_task);
			return err;
		}
		printk("create Thread1\n");
		wake_up_process(my_task); //5-启动线程
	}
	else if(n == 2){//4-创建线程
		my_task2 = kthread_create(my_kthread2, NULL, "Thread2");
		if(IS_ERR(my_task2)){
			printk("Unable to start kernel thread.\n");
			err = PTR_ERR(my_task2);
			return err;
		}
		printk("create Thread2\n");
		wake_up_process(my_task2); //5-启动线程
	}
	return 0;
}
//(c) - 打开函数
int chrdev_open(struct inode *inode, struct file *filp)
{
	printk("%s\r\n", __func__);

	return 0;
}
//(c) - 操作函数
long chrdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	printk("cmd %d %ld\n",cmd, arg);
	switch (cmd) {
	case START:
		if(arg == THREAD1) 
		{	if(!my_task) ret = thread_run(1); }
		else 
		{	if(!my_task2) ret = thread_run(2); }
		break;
	case PAUSE:
		if(arg == THREAD1) 
			sema_init(&sem1, 0);
		else
			sema_init(&sem2, 0);
		break;
	case CONTINUE:
		if(arg == THREAD1) 
		{ up(&sem1); }
		else 
		{ up(&sem2); }
		break;
	case STOP:
		if(arg == THREAD1 && my_task) 
		{		
			printk("Cancel thread1.\n");
			kthread_stop(my_task);   //6-关闭线程
			my_task=NULL;
		}
		else if(arg == THREAD2 && my_task2)
		{
			printk("Cancel thread2.\n");
			kthread_stop(my_task2);
			my_task2=NULL;
		}
		break;
	case STARTALL :  
		if(!my_task) ret =thread_run(1);
		if(!my_task2) ret = thread_run(2);
		break;
	case PAUSEALL :
		sema_init(&sem1, 0);  
		sema_init(&sem2, 0);  
		break;
	case CONTINUEALL :
		up(&sem1);  //(5)-释放信号量1
		up(&sem2);  //(5)-释放信号量2
		break;
	case STOPALL :
		if(my_task){ 
			printk("Cancel thread1.\n");
			kthread_stop(my_task);   //6-关闭线程
			my_task=NULL;
		}
		if(my_task2){ 
			printk("Cancel thread2.\n");
			kthread_stop(my_task2);   //6-关闭线程
			my_task2=NULL;
		}
		break;
	default:
		return -EINVAL;
	}
	return ret; /* 0表示成功 */
}
//(c)-关闭函数
int chrdev_release(struct inode *inode, struct file *filp)
{
	printk("%s\r\n", __func__);
//	up(&sem1);  //(5)-释放信号量1
// 	up(&sem2);  //(5)-释放信号量2
//	if(my_task){ 
//		kthread_stop(my_task);   //6-关闭线程
//		my_task=NULL;
//	}
//	if(my_task2){ 
//		kthread_stop(my_task2);   //6-关闭线程
//		my_task2=NULL;
//	}
	return 0;
}

/* (d) 字符设备对应操作 */
const struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = chrdev_open,
	.unlocked_ioctl = chrdev_ioctl,
	.release = chrdev_release,
};

static int __init number_init(void)
{
	printk("%s:\n", __func__);
	/* (e)-0. 注册设备编号 */
	if (register_chrdev_region(MKDEV(CHRDEV_MAJOR, CHRDEV_MAION), CHRDEV_COUNT, CHRDEV_NAME) < 0) {
		printk("register charactor device number failure\n");
		return -EBUSY;
	}
	/* (e)-1. 分配字符设备 */
	pdev = kmalloc(sizeof(*pdev), GFP_KERNEL);
	if (pdev == NULL) {
		printk("allocate led structure failure\n");
		return -ENOMEM;
	}
	pdev->dev = MKDEV(CHRDEV_MAJOR, CHRDEV_MAION);

	/* 2. 初始化字符设备 */
	cdev_init(&pdev->chrdevcdev, &fops);
	pdev->chrdevcdev.owner = THIS_MODULE;

	/* 3. 添加字符设备，主设备号300，次设备号0*/
	cdev_add(&pdev->chrdevcdev, pdev->dev, CHRDEV_COUNT);

	//(3) 初始化信号量  初始值1
	sema_init(&sem1, 1);
	sema_init(&sem2, 1);

	//if(!my_task) thread_run(1, &my_task);
	//if(!my_task2) thread_run(2, &my_task2);
	return 0;
}
static void __exit number_stop(void)
{
	printk("%s:\n", __func__);
	//(f)-注销字符设备
	cdev_del(&pdev->chrdevcdev);
	kfree(pdev);
	unregister_chrdev_region(pdev->dev, CHRDEV_COUNT);

	if(my_task)
	{
		printk(KERN_ALERT "Cancel this kernel thread1.\n");
		kthread_stop(my_task);   //6-关闭线程
		printk(KERN_ALERT "Canceled.\n");
	}
	if(my_task)
	{
		printk(KERN_ALERT "Cancel this kernel thread2.\n");
		kthread_stop(my_task2);   //6-关闭线程
		printk(KERN_ALERT "Canceled.\n");
	}
	printk("Goodbyte, Kernel!\n");
}
module_init(number_init);
module_exit(number_stop);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yuhang");
