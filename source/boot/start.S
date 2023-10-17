/**
 *write to the operating system by myself
 *
 * 系统引导部分，启动时由硬件加载运行，然后完成对二级引导程序loader的加载
 * 该部分程序存储于磁盘的第1个扇区，在计算机启动时将会由BIOS加载到0x7c00处
 * 之后，将由BIOS跳转至0x7c00处开始运行
 *
 *created by Imp on 2023-10-05
 */
	#include "boot.h"

  	// 16位代码，务必加上
  	.code16
 	.text
	.global _start
	.extern boot_entry // 二级引导程序入口

_start:
	mov $0, %ax			
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs				

	mov $_start, %esp  

	mov $0xe, %ah  // 0xe是显示字符的BIOS中断号
	mov $'L', %al
	int $0x10  
	
	mov $'O', %al
	int $0x10 

	mov $'S', %al
	int $0x10 

read_loader:
	mov $0x8000, %bx    // 0x8000是loader的加载地址
	mov $0x2, %ah       // 0x2是读磁盘BIOS中断号
	mov $0x2, %cx     
	mov $64,  %al		// 读取64个扇区
	mov $0x0080, %dx
	int $0x13			
	jc read_loader		// 读取失败则重试

	jmp boot_entry		//实现汇编语言到C语言的跳转
	jmp .
	
	// 引导结束段
	.section boot_end, "ax"
boot_sig: .byte 0x55,0xaa
