#----
#File:		makefile
#Auther: 	Jason Hu, Yu Zhu，Suote127
#Time: 		2019/6/1
#copyright:	(C) 2018-2020 by Book OS developers. All rights reserved.
#----

#----tools----
#tool dir

PHONY		:=
all:

# tools
NASM 		= nasm
CC			= gcc
LD 			= ld
DD 			= dd
OBJDUMP		= objdump
# virtual machine
QEMU 		= qemu-system-i386
BOCHS 		= bochs
VBOX_MANAGE = VBoxManage

# debugger
BOCHSDBG 	= bochsdbg

# development dir
DEVELOPMENT_DIR	= ../developments/

# bochs development dir
BOCHS_DIR = $(DEVELOPMENT_DIR)bochs/

# bochsrc for windows
BXRC_WIN 		= $(BOCHS_DIR)bochsrc.win
# bochsrc for linux
BXRC_LINUX 		= $(BOCHS_DIR)bochsrc.linux

# virtual box 虚拟机名称
VBOX_NAME	= "Xbook"

#----img----
#img dir
IMG_DIR = $(DEVELOPMENT_DIR)image/

FLAPPY_IMG 	= $(IMG_DIR)a.img
HDA_IMG		= $(IMG_DIR)c.img
HDB_IMG		= $(IMG_DIR)d.img
HDC_IMG		= $(IMG_DIR)e.img
HDD_IMG		= $(IMG_DIR)f.img

#----flags----
#out a map file: -Map kernel.map
#i386 link flags
LDFLAGS		= -m elf_i386

#system disk
LOADER_OFF = 2		
LOADER_CNTS = 8
 
KERNEL_OFF = 10

# assume 512 kb 
KERNEL_CNTS = 1024	

#----dir----
#arch
ARCH = arch/x86

ARCH_BOOT_DIR = $(ARCH)/boot/
ARCH_KERNEL_DIR = $(ARCH)/kernel/

#----bin and elf----
#boot temp file
BOOT_BIN =  $(ARCH_BOOT_DIR)boot.bin
LOADER_BIN =  $(ARCH_BOOT_DIR)loader.bin

TOPDIR	:= $(CURDIR)

# System environment variable.
ifeq ($(OS),Windows_NT)
	HOSTOS		:= windows
else
	ifeq ($(shell uname),Darwin)
		HOSTOS		:= macos
	else
		ifeq ($(shell uname),Linux)
			HOSTOS		:= linux
		else
			HOSTOS		:= unix-like
		endif
	endif
endif

#objs
SRCDIRS	:= $(ARCH)/ driver/ external/ init/ kernel/ lib/

# 库文件目录
LIBDIRS	:= lib/

INCDIRS	:= include/ $(ARCH)/include/

OBJS	:= $(patsubst %/, %/built-in.o, $(SRCDIRS))
OBJS_LIB	:= $(patsubst %/, %/built-in.o, $(LIBDIRS))

INCLUDE_PATH	:= $(patsubst %, -I $(TOPDIR)/%, $(INCDIRS))

OPTFLAGS		:=
ifeq ("$(origin G)", "command line")
ifeq ($(G),on)
OPTFLAGS		:= -O0 -g -ggdb
endif
endif

CFLAGS	:=  -fno-builtin -nostdinc -Wall -Wunused -fno-PIE -m32 -fno-stack-protector $(OPTFLAGS) $(INCLUDE_PATH) -c
NASM_FLAGS	:= $(INCLUDE_PATH)

#kernel file	
KERNEL_ELF =  kernel.elf
KERNEL_ELF_DISASM =  kernel.elf.disasm

PHONY	+= all run compile link disk bochs bochsdbg qemu qemudbg clean dis
PHONY	+= $(SRCDIRS)

export NASM NASM_FLAGS CC LD CFLAGS LDFLAGS TOPDIR

#----cmd----
all: disk
run: qemu

#compile file
$(OBJS) : $(SRCDIRS) ;

$(SRCDIRS) :
ifneq ($(strip $(MAKECMDGOALS)),clean)
	$(MAKE) -C $@ -f $(TOPDIR)/Makefile.build
else
	$(MAKE) -C $@ -f $(TOPDIR)/Makefile.build clean
endif

$(KERNEL_ELF): $(OBJS)
	$(LD) $(LDFLAGS)  -e _start -Ttext 0x80100000 -o $(KERNEL_ELF) $(OBJS)

dis: $(KERNEL_ELF)
	$(OBJDUMP) -M intel -D $(KERNEL_ELF) > $(KERNEL_ELF_DISASM)

$(LIBC_A): $(OBJS_LIB) 
	$(AR) $(ARFLAGS) $(LIBC_A) $(OBJS_LIB)

#write file into disk
disk: $(KERNEL_ELF)
	$(DD) if=$(BOOT_BIN) of=$(FLAPPY_IMG) bs=512 count=1 conv=notrunc
	$(DD) if=$(LOADER_BIN) of=$(FLAPPY_IMG) bs=512 seek=$(LOADER_OFF) count=$(LOADER_CNTS) conv=notrunc
	$(DD) if=$(KERNEL_ELF) of=$(FLAPPY_IMG) bs=512 seek=$(KERNEL_OFF) count=$(KERNEL_CNTS) conv=notrunc

#run kernel in vm

bochs: disk
ifeq ($(HOSTOS),windows)
	$(BOCHS) -q -f $(BXRC_WIN)
else
	$(BOCHS) -q -f $(BXRC_LINUX)
endif

bochsdbg: disk
ifeq ($(HOSTOS),windows)
	$(BOCHSDBG) -q -f $(BXRC_WIN)
else
	$(BOCHSDBG) -q -f $(BXRC_LINUX)
endif

#-hda $(HDA_IMG) -hdb $(HDB_IMG)
# 网卡配置: 
#	-net nic,vlan=0,model=rtl8139,macaddr=12:34:56:78:9a:be
# 网络模式：
#	1.User mode network(Slirp) :User网络
#		-net user
#	2.Tap/tun network : Tap网络
#		-net tap
# 		-net tap,vlan=0,ifname=tap0
#	example: -net nic,model=rtl8139 -net tap,ifname=tap0,script=no,downscript=no 
		
# 音频配置：
# 	a.使用蜂鸣器：-soundhw pcspk
#	b.使用声霸卡：-soundhw sb16
# 控制台串口调试： -serial stdio 
QEMU_ARGUMENT = -m 256M \
		-name "XBook Development Platform for x86" \
		-fda $(FLAPPY_IMG) -hda $(HDA_IMG) -hdb $(HDB_IMG) -boot a \
		-serial stdio
		
qemu: disk
	$(QEMU) $(QEMU_ARGUMENT)

# 调试配置：-S -gdb tcp::10001,ipv4
qemudbg: disk
	$(QEMU) -S -gdb tcp::10001,ipv4 $(QEMU_ARGUMENT)

# virtual box
vbox: disk
	$(VBOX_MANAGE) startvm $(VBOX_NAME) --type gui 

# clean temporary files
clean: $(SRCDIRS)
	$(RM) $(KERNEL_ELF) $(KERNEL_ELF_DISASM)

.PHONY: $(PHONY)
