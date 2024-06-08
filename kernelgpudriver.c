#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include "address_map_arm.h"
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Icaro");

//CONSTANTES
#define DATA_A  0x80
#define DATA_B  0x70
#define START 0xc0
#define WR_FULL 0xb0

#define DRIVER_NAME "gpu123"

static unsigned char data[8]; 

void * LW_virtual; // Lightweight bridge base address
volatile int *DATA_A_PTR, *DATA_B_PTR, *START_PTR, *WR_FULL_PTR; // virtual addresses



static struct drvled_data{
    dev_t devnum;
    struct cdev cdev;
} drvled_data;




int major_number;

void print_binary(unsigned int num) {
    int i;
    printk(KERN_INFO "Valor em binário: ");
    for (i = sizeof(num) * 8 - 1; i >= 0; i--) {
        printk(KERN_CONT "%d", (num >> i) & 1);
        if (i % 4 == 0) printk(KERN_CONT " "); // Adiciona um espaço a cada 4 bits para facilitar a leitura
    }
    printk(KERN_CONT "\n");
}



int device_open(struct inode *inode,struct file *filp){
        printk(KERN_INFO "aberto");
        return 0;
}






int ret;

int t;
ssize_t device_write(struct file* filp, const unsigned char *bufSourceData, size_t bufCount, loff_t* curOffset){
    int ret;
    unsigned int concat, concat1;
    printk(KERN_INFO "escrevendo");
    ret = copy_from_user(data, bufSourceData, bufCount);
    if (ret != 0) {
        printk(KERN_ERR "Failed to copy data from user space\n");
        return -EFAULT; // Error code for bad address
    }

    int i;
    printk(KERN_INFO "Valores em data:\n");
    for (i = 0; i < 8; i++) {
        printk(KERN_INFO "%u ", data[i]);
    }
    printk(KERN_INFO "\n");
    printk(KERN_INFO "\n");


    while(*WR_FULL_PTR){} // mexemos aqui ó

    printk(KERN_INFO "oioi");

    concat = data[7] << 24 | data[6] << 16 | data[5] << 8 | data[4];//c>
    concat1 = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];//>

    print_binary(concat1);
    print_binary(concat);

    *START_PTR = 0;
    *DATA_A_PTR = concat1;
    *DATA_B_PTR = concat;
    *START_PTR = 1;

    return bufCount;
}



int device_close(struct inode *inode, struct file *filp){
       // up(&virtual_device.sem);
        return 0;


}


ssize_t device_read(struct file* filp,char *bufDestination, size_t bufCount, loff_t* curOffset){
        int ret;
        printk(KERN_INFO "LENDO DISPOTIVO");


        ret=copy_to_user(bufDestination,data + *curOffset,bufCount);
        if (ret != 0) {
                printk(KERN_ERR "Failed to copy data para o espaco de usuarioe\n");
                return -EFAULT; // Error code for bad address
    }
        printk(KERN_INFO "lendo");
        return bufCount;
}


static const struct file_operations fops = {
        .owner=THIS_MODULE,
        .open = device_open,
        .release =device_close,
        .write = device_write,
        .read = device_read,
};
   

static int __init init_kernelgpudriver(void) {


    int result;




//     uint16_t buffer1, buffer2;
    result = alloc_chrdev_region(&drvled_data.devnum, 0, 1, DRIVER_NAME);
    if (result) {
    pr_err("%s: Failed to allocate device number!\n", DRIVER_NAME);
    return result;
    }
   // major_number = MAJOR(&drvled_data.devnum);
   // printk(KERN_INFO "major number %d",major_number);
    cdev_init(&drvled_data.cdev, &fops);


    result = cdev_add(&drvled_data.cdev, drvled_data.devnum, 1);
    if (result) {
    pr_err("%s: Char device registration failed!\n", DRIVER_NAME);
    unregister_chrdev_region(drvled_data.devnum, 1);
    return result;
    }


    //drvled_setled(LED_OFF);
   
    LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    
    DATA_A_PTR = (unsigned int *) (LW_virtual + DATA_A);
    DATA_B_PTR = (unsigned int *) (LW_virtual + DATA_B);  
    START_PTR = (unsigned int *) (LW_virtual + START);  
    WR_FULL_PTR = (unsigned int *) (LW_virtual + WR_FULL);  


    pr_info("%s: initialized.\n", DRIVER_NAME);




   return 0;
}


static void __exit exit_kernelgpudriver(void) {
   *START_PTR = 0;
    iounmap (LW_virtual);
    cdev_del(&drvled_data.cdev);
    unregister_chrdev_region(drvled_data.devnum, 1);
    pr_info("%s: exiting.\n", DRIVER_NAME);
}


module_init(init_kernelgpudriver);
module_exit(exit_kernelgpudriver);




