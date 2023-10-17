/**
 *write to the operating system by myself
 *
 
 *
 * the main function of this part is to load the second boot program
 *created by Imp on 2023-10-05
 */
	
  
  	.code16					//16位代码，用于实模式下运行
 	.text
	.global _start
	.extern loader_entry //二级引导程序入口

_start:
	jmp loader_entry


	.code32 //32位代码,用于保护模式下运行
	.text
	.global protect_mode_entry
	.extern load_kernel
protect_mode_entry:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	jmp $8, $load_kernel