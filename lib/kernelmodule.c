#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include "address_map_arm.h"
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>


//CONSTANTES
#define DATA_A  0x80
#define DATA_B  0x70
#define START 0xc0

#define DRIVER_NAME "gpu123"

void * LW_virtual; // Lightweight bridge base address
volatile int *DATA_A_PTR, *DATA_B_PTR, *START_PTR; // virtual addresses

struct gpu_amada{
	unsigned char data[4];
} gpu_amada;

static struct drvled_data{
	dev_t devnum;
	struct cdev cdev;
} drvled_data;


int major_number;

static const struct file_operations fops = {
        .owner=THIS_MODULE,
        .open = device_open,
        .release =device_close,
        .write = device_write,
        .read = device_read
};
	


int device_open(struct inode *inode,struct file *filp){


       // if(down_interruptible(&virtual_device.sem) != 0){

//                printk(KERN_ALERT "nao foi possivel trancar durante a bertura");
  //              return -1;
    //    }
        printk(KERN_INFO "aberto");
        return 0;
}



int ret;

ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset){
        printk(KERN_INFO "lendo");
        ret = copy_to_user(bufStoreData,gpu_amada.data,bufCount);
        return ret;
}


ssize_t device_write(struct file* filp, const char bufSourceData, size_t bufCount, loff_t* curOffset){
        unsigned char buffer1[2];
        unsigned char buffer2[2];
        unsigned char data[4];

        ret=copy_from_user(&data, bufSourceData, bufCount);
	
        LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
        // Set virtual address pointer to I/O port
        DATA_A_PTR = (unsigned int *) (LW_virtual + DATA_A);
        DATA_B_PTR = (unsigned int *) (LW_virtual + DATA_B);   
        START_PTR = (unsigned int *) (LW_virtual + START);   
	
        *buffer1 = (data[0] << 8) | data[1];
        *buffer2 = (data[2] << 8) | data[3];
	    *START_PTR = 0;
        *DATA_A_PTR = *buffer1;
        *DATA_B_PTR = *buffer2;
        *START_PTR = 1;
	
	    return ret;
}


int device_close(struct inode *inode, struct file *filp){
       // up(&virtual_device.sem);
        return 0;

}





static int __init init_kernelteste(void) {

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
    

    pr_info("%s: initialized.\n", DRIVER_NAME);


   return 0;
}

 
static void __exit exit_kernelteste(void) {
   *START_PTR = 0;
    iounmap (LW_virtual);
    cdev_del(&drvled_data.cdev);
    unregister_chrdev_region(drvled_data.devnum, 1);
    pr_info("%s: exiting.\n", DRIVER_NAME);
}

module_init(init_kernelteste);
module_exit(exit_kernelteste);