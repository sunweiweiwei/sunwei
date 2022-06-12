#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define CHRDEV_NAME "/dev/chrdev"  //字符设备节点
#define THREAD1 1     //线程编号1
#define THREAD2 2     //线程编号2
#define START 1       //开始
#define PAUSE 0x20       //暂停
#define CONTINUE 3    //继续
#define STOP 4        //停止 
#define STARTALL 5       //全部开始
#define PAUSEALL 6       //全部暂停
#define CONTINUEALL 7    //全部继续
#define STOPALL 8       //全部停止

int main(int argc, char *argv[])
{
	int c;
	int fd;
	fd = open(CHRDEV_NAME, O_RDONLY);   //打开文件
	if (fd < 0) {
		perror(argv[1]);
		return -1;
	}
	while(1)
	{
		printf("0 : exit\n1 : startall\n2 : pauseall\n3 : continueall\n4 : stopall\n");
		printf("5 : start thread1\n6 : pause thread1 \n7 : continue thread1 \n8 : stop thread1\n"); 
		printf("9 : start thread2\n10 : pause thread2 \n11 : continue thread2 \n12 : stop thread2\n"); 
		printf("please input : ");
		scanf("%d", &c);
		if(c == 0) break;
		switch(c) {
		case 5 : 
			printf("START 1\n");
			ioctl(fd, START, THREAD1); 
			break;
		case 9:
			printf("START 2\n");
			ioctl(fd, START, THREAD2);
			break;
		case 6:
			printf("PAUSE 1\n");
			ioctl(fd, PAUSE, THREAD1);
			break;
		case 10:
			printf("PAUSE 2\n");
			ioctl(fd, PAUSE, THREAD2);
			break;
		case 7:
			printf("CONTINUE 1 \n");
			ioctl(fd, CONTINUE, THREAD1);
			break;
		case 11:
			printf("CONTINUE 2 \n");
			ioctl(fd, CONTINUE, THREAD2);
			break;
		case 8:
			printf("STOP 1\n");
			ioctl(fd, STOP, THREAD1);
			break;
		case 12:
			printf("STOP 2\n");
			ioctl(fd, STOP, THREAD2);
			break;
		case 1:
			printf("START ALL \n");
			ioctl(fd, STARTALL, THREAD1);
			break;
		case 2:
			printf("PAUSE ALL\n");
			ioctl(fd, PAUSEALL, THREAD1);
			break;
		case 3:
			printf("CONTINUEALL \n");
			ioctl(fd, CONTINUEALL, THREAD1);
			break;
		case 4:
			printf("STOPALL \n");
			ioctl(fd, STOPALL, THREAD1);
			break;
		default : break;
		}
	}
	close(fd); // 关闭文件
	return 0;
}

