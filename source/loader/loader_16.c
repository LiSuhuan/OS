__asm__(".code16gcc");   //告诉编译器，下面的代码要用16位的汇编指令集编译


#include "loader.h"

boot_info_t boot_info;   //定义一个boot_info_t类型的变量boot_info

/**
 * @brief 打印字符串
 * 
 * @param msg 传入信息
 * @return int 是否成功
 */
static int show_msg (const char* msg) {
    char c;

    //内联汇编，调用BIOS的中断例程，显示字符
    while((c=*msg++)!='\0'){
       __asm__ __volatile__(
            "mov $0x0e, %%ah\n\t"
            "mov %[ch], %%al\n\t"
            "int $0x10"
            :          //输出部分
            :[ch]"r"(c)   //将c的值放入寄存器a中
       );
    }


}

/// @brief  检测内存
static void detect_memory(void) {
    SMAP_entry_t smap_entry;
    uint32_t signature = 0,bytes = 0,contID = 0;
    SMAP_entry_t * entry = &smap_entry;
    show_msg("Detecting memory...:\n");

    boot_info.ram_region_count = 0;
    for (int i=0; i<BOOT_RAM_REGION_MAX; i++) {
       

     __asm__ __volatile__("int  $0x15"
        : "=a"(signature),"=c"(bytes),"=b"(contID)
        : "a"(0xE820),"b"(contID),"c"(24),"d"(0x534D4150),"D"(entry)
     ); 

     if(signature != 0x534D4150){
         show_msg("Detecting memory failed!\r\n");
         return;
     }


     if(bytes>20 && (entry->ACPI & 0x0001) == 0){
        continue;
     }

    if(entry->Type == 1){
        boot_info.ram_region_cfg[boot_info.ram_region_count].start = entry->BaseL;   
        boot_info.ram_region_cfg[boot_info.ram_region_count].size = entry->LengthL;
        boot_info.ram_region_count++;

    }
    
    if(contID == 0){
        break;
    }        
}
show_msg("Detecting memory done!\r\n");

}

uint16_t gdt_table[][4] = {
    {0, 0, 0, 0},
    {0xffff, 0x0000, 0x9a00, 0x00cf}, 
    {0xffff, 0x0000, 0x9200, 0x00cf}, 
};


/// @brief  进入保护模式
/// @param  
static  void enter_protect_mode (void) {
    //1、关闭中断
    cli();  

    //2、设置A20，读取92端口的值，将其第2位置设为1
    uint8_t v = inb(0x92);  
    outb(0x92, v | 0x2);   

    //3、设置GDT表
    lgdt((uint32_t)gdt_table, sizeof(gdt_table));  

    //4、开启保护模式的使能位，设置cr0寄存器的第0位PE为1
    uint32_t cr0 = read_cr0();  
    write_cr0(cr0 | 0x1);     

    /// @brief  5、远跳转到32为的loader程序，并清空原来的16位指令流水线
    far_jump((1<<3), (uint32_t)protect_mode_entry);  

}

/// @brief  加载内核

void loader_entry (void){
    show_msg("\nello, OS kernel!\n");
    
    //1、检测内存
    detect_memory();

    //2、进入保护模式
    enter_protect_mode();
    
    for(;;){}
}
