#ifndef CPU_INSTR_H
#define CPU_INSTR_H

#include "types.h"
//从端口寄存器读取八位数据 
static inline uint8_t inb(uint16_t port)
{
    uint8_t rv;

    // inb al,ad
    __asm__ __volatile__("inb %[p], %[v]" : [v] "=a"(rv) : [p] "d"(port));
    return rv;
}

//从端口寄存器读取16位数据
static inline uint16_t inw(uint16_t port)
{
    uint16_t rv;

    // inb al,ad
    __asm__ __volatile__("in %[p], %[v]" : [v] "=a"(rv) : [p] "d"(port));
    return rv;
}
// 这个函数是用来写端口的，第一个参数是端口号，第二个参数是要写入的数据
static inline void outb(uint16_t port, uint8_t data)
{
    // outb al,dx
    __asm__ __volatile__("outb %[v], %[p]" : : [v] "a"(data), [p] "d"(port));
}

/**
 *  @brief 关闭中断
 * 
 */
static inline void cli(void)
{
    __asm__ __volatile__("cli");
}

/**
@brief 开中断
*/
static inline void sti(void)
{
    __asm__ __volatile__("sti");
}


static inline void lgdt(uint32_t start, uint32_t size)
{
    struct
    {
        uint16_t limit;             //GDT在内存中的界限值
        uint16_t start15_0;         //GDT地址的低16位
        uint16_t start31_16;        //GDT地址的高16位
    }gdt;
    gdt.start31_16 = (start >> 16) & 0xffff;     
    gdt.start15_0 = start & 0xffff;   
    gdt.limit = size - 1;
    
    __asm__ __volatile__("lgdt %[g]" : : [g] "m" (gdt));
}

/// @brief 读取cr0寄存器的值
static inline uint32_t read_cr0 (void) {
    uint32_t cr0;

    __asm__ __volatile__ ("mov %%cr0, %[v]" : [v] "=r"(cr0));
    return cr0;
}

/// @brief 写cr0寄存器的值

static inline void write_cr0 (uint32_t data) {
    __asm__ __volatile__ ("mov %[v], %%cr0" : : [v]"r"(data));
}

/// @brief 远跳转
static inline void far_jump (uint32_t selector, uint32_t offset) {
    uint32_t addr[] = {offset, selector};
    __asm__ __volatile__ ("ljmpl *(%[a])" : : [a] "r" (addr));
}

#endif