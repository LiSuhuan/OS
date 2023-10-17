#include "init.h"
#include "comm/boot_info.h"

static int arr[100];
int a = 10;
char * str = "hello world";

void kernel_init (boot_info_t* boot_info) {
    str[1] = 'H';
}