
#include "loader.h"
/// @brief 以LBA模式读取磁盘，在此采用的是LBA48模式。
/// @param sector 读取的扇区号
/// @param sector_count 读取的扇区数量
/// @param buf 数据存放的地址




static void read_disk (uint32_t sector, uint16_t sector_count, uint8_t *buf) {

    //1、设置以LBA模式进行读取，即将磁盘看作一片连续的扇区
    outb(0x1F6, 0xE0);   //将寄存器第6位置设为1进入LBA模式，，第四位置0指定驱动器号为主盘

    //2、初始化各个端口寄存器的高8位
    outb(0x1F2, (uint8_t)(sector_count >> 8));    // 读取扇区数的高8位
                                                //6字节的LBA值，先初始化第456个字节
                                                //暂时只用到哦了4个字节的LBA值，所以第56个字节置0即可
    outb(0x1F3, (uint8_t)(sector_count >> 24));  //LBA4
    outb(0x1F4, 0);                             //LBA5
    outb(0x1F5, 0);                             //LBA6

    
    //3、初始化各个端口寄存器的低八位
    outb(0x1F2, (uint8_t)sector_count );        //读取扇区数的低八位
    outb(0x1F3, (uint8_t)sector);               //LBA1
    outb(0x1F4, (uint8_t)sector >> 8);          //LBA2
    outb(0x1F5, (uint8_t)sector >> 16);         //LBA3

    //4、将读取扇区命令（0x24）发送到端口 0x1F7
    outb(0x1F7, 0x24);

    //5、读取状态端口寄存器，判断是否可读，若可以则读取，否则阻塞等待

    uint16_t* data_buf = (uint16_t*)buf;     //数据缓冲区，每次读取16位数据
    while (sector_count--) {                
        while((inb(0x1F7) & 0x88) != 0x8){
            
        } //取出状态寄存器3位和7七位
                                            //若 ！= 0x8即DRQ位为0,即非就绪状态
                                            //或者BSY（7位）为1,即忙碌状态，都不可取

        for(int i=0; i<SECTOR_SIZE / 2; ++i){
            *(data_buf++) = inw(0x1F0);     //从数据寄存器读取16位数据，即 两个字节
        }
       
      
    }

}



void  load_kernel (void) {
    read_disk(100, 500, (uint8_t *)SYS_KERNEL_LOAD_ADDR);            //读取磁盘
    ((void (*)(boot_info_t *))SYS_KERNEL_LOAD_ADDR)(&boot_info);                        //跳转到kernel入口
    for(;;);
}