#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include "types.h"
#define BOOT_RAM_REGION_MAX 10
typedef struct _boot_info_t{
    struct{
        uint32_t start;  //ram区域的起始地址
        uint32_t size;  //ram区域的大小
    }ram_region_cfg[BOOT_RAM_REGION_MAX];

    int ram_region_count;   //指示ram_region_cfg中有多少个有效的ram区域
}boot_info_t; 



#define SECTOR_SIZE 512
#define SYS_KERNEL_LOAD_ADDR (1024 * 1024)        //kernel地址

#endif