SECTIONS {
    . = 0x100000;

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodate)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

}