#ifndef LOADER_H
#define LOADER_H

#include "comm/types.h"
#include "comm/boot_info.h"
#include "comm/cpu_instr.h"

void protect_mode_entry (void);
 

typedef struct SMAP_entry {
    uint32_t BaseL;   //基地址低32位
    uint32_t BaseH;   //基地址高32位
    uint32_t LengthL; //长度低32位
    uint32_t LengthH; //长度高32位
    uint32_t Type;    //内存类型,值为1表示可用内存
    uint32_t ACPI;    //extended，bit0=0时表示此条目应当被忽略   
    
} __attribute__((packed)) SMAP_entry_t;   //packed表示取消对齐


extern boot_info_t boot_info;

#endif
