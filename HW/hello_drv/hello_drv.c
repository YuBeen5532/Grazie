

#include <linux/init.h> // module_init, module_exit 매크로 정의
#include <linux/kernel.h> // printk 함수 정의
#include <linux/module.h>// 모듈 관련 자료 구조 및 매크로 정의
#include <linux/fs.h> // register_chrdev 함수
#include <asm/uaccess.h> // copy_to_user함수

#define HELLO_MAJOR_NUM 290
#define HELLO_NAME "Hello"	
#define max 2000
static char array[max] = {'a','b','c','d','e','f','g','8','9'};

static int hello_open(struct inode *node, struct file *pfile)
{
	printk("hello_open enter\n");
	return 0;
}

static ssize_t hello_read(struct file *pfile, char __user *pBuff, size_t size, loff_t *filepos)
{
	printk("hello_read enter\n");
		copy_to_user( pBuff, array, max );
	return 0;
}

static ssize_t hello_write(struct file *pfile, const char __user *pBuff, size_t size, loff_t *filepos) {
	printk("hello_write enter\n");
		copy_from_user ( array , pBuff , max );
	return 0;
}

static long hello_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{   int cnt = 0; int k = 0;
    if (_IOC_TYPE (cmd) != 0x55) {printk("Wrong Magic#!\n"); return -1;}
    switch(_IOC_NR (cmd))
    {
        case 99: printk("IOCLT Size : %d\n",_IOC_SIZE (cmd));
                copy_from_user(array, arg, _IOC_SIZE(cmd));
                break;
        case 98: printk("IOCLT Size : %d\n",_IOC_SIZE (cmd));
                copy_from_user(&cnt, arg, _IOC_SIZE(cmd));
                for (k=0; k<cnt; k++) printk ("%d ", array[k]); 
                printk ("\n"); break;
        default: break;
    }
}

static int hello_release(struct inode *node, struct file *pfile)
{
    printk("hello_release enter\n");
    return 0;
}

static const struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .open = hello_open,
    .read = hello_read,
    .write = hello_write,
    .release = hello_release,
    .unlocked_ioctl = hello_ioctl,
};



int flagRegisterSuccess = 0;

int __init init_hello(void)
{
    int ret;
    printk("Hello world.\n");
    ret = register_chrdev(HELLO_MAJOR_NUM, HELLO_NAME, &hello_fops);
    if ( ret < 0)
    {
        printk("Hello registr_chrdev fail\n");
        return ret; }
        flagRegisterSuccess = 1;
        return 0;
    }

void __exit exit_hello(void)
{
    if ( flagRegisterSuccess)
    {
        flagRegisterSuccess = 0;
        unregister_chrdev(HELLO_MAJOR_NUM, HELLO_NAME);
    }
    printk("Good-bye~ \n");
}

module_init(init_hello); // 모듈 로딩시(insmod) 해당 함수 호출
module_exit(exit_hello); // 모듈 제거시(rmmod) 해당 함수 호출
MODULE_LICENSE("GPL");
