This makefile allows for programming and debugging the STM32L476xxx Nucleo-64
MCU/PCB with the files in this project. Make sure all files are in the same
directory when using this makefile (or use autotools or something).

Command uses:
all -> compile and link all files into a .elf file called final.elf 
semi -> compile and link all files into a .elf file called final_sh.elf (can be used for semihosting)
clean -> remove all object files, map files, and .elf files
load -> setup a connection to the mcu using OpenOCD (runs as a server)
client -> runs a gdb executable from the GNU Arm toolchain to allow for connection to the OpenOCD server (ran by the previous load command)
