# 物理内存分布图
    +---------------------+-------------------------------------------+
    | Address             | details                                   |
    +---------------------+-------------------------------------------+
    | 0x00~0x400          | int vectors                               |
    +---------------------+-------------------------------------------+
    | 0x400~0x500         | ROM BIOS parameter area                   |
    +---------------------+-------------------------------------------+
    | 0x1000~0x1100       | ards                                      |
    +---------------------+-------------------------------------------+
    | 0x7c00~0x7e00       | boot.bin                                  |
    +---------------------+-------------------------------------------+
    | 0x10000~0x42000     | kernel.elf                                |
    +---------------------+-------------------------------------------+
    | 0x42000~0x90000     | normal file                               |
    +---------------------+-------------------------------------------+
    | 0x90000~0x94000     | loader.bin                                |
    +---------------------+-------------------------------------------+
    | 0x9e000~0x9f000     | kernel statck                             |
    +---------------------+-------------------------------------------+
    | 0x9fc00~0xa0000     | extended BIOS data area (EBDA)            |
    +---------------------+-------------------------------------------+
    | 0xa0000~0xc0000     | display adapter reserved                  |
    +---------------------+-------------------------------------------+
    | 0xc0000~0xe0000     | Reserved for ROM expansion                |
    +---------------------+-------------------------------------------+
    | 0xe0000~0xf0000     | Expansion of system ROM                   |
    +---------------------+-------------------------------------------+
    | 0xf0000~0x100000    | System ROM                                |
    +---------------------+-------------------------------------------+
    | 0x100000~0x200000   |kernel                                     |
    +---------------------+-------------------------------------------+
    | 0x200000~0x200800   | GDT                                       |
    +---------------------+-------------------------------------------+
    | 0x200800~0x201000   | IDT                                       |
    +---------------------+-------------------------------------------+
    | 0x201000~0x202000   | KERNEL PAGE DIR TABLE                     |
    +---------------------+-------------------------------------------+
    | 0x202000~0x203000   | KERNEL PAGE TABLE                         |
    +---------------------+-------------------------------------------+
    | 0x210000~0x220000   | phy memory pool bitmap                    |
    +---------------------+-------------------------------------------+
    | 0x220000~0x230000   | vir memory pool bitmap                    |
    +---------------------+-------------------------------------------+
    | 0x230000~0x240000   | kernel vir addr bitmap                    |
    +---------------------+-------------------------------------------+
    | 0x400000~0xfffff000 | avaliable memory                          |
    +---------------------+-------------------------------------------+