cmd_/root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug.o := gcc -Wp,-MD,/root/acextreme/drivers/acex/prj/../src/os/debug/.ddc_udl_os_debug.o.d  -nostdinc -isystem /usr/lib/gcc/i686-redhat-linux/4.4.7/include -Iinclude  -I/usr/src/kernels/2.6.32-504.el6.i686/include/uapi -I/usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include -Iarch/include/generated -Iinclude -include /usr/src/kernels/2.6.32-504.el6.i686/include/linux/kconfig.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -m32 -msoft-float -mregparm=3 -freg-struct-return -mpreferred-stack-boundary=2 -march=i686 -mtune=generic -Wa,-mtune=generic32 -ffreestanding -fstack-protector -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_AVX=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -Wframe-larger-than=1024 -Wno-unused-but-set-variable -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-dwarf2-cfi-asm -fconserve-stack -I/root/acextreme/drivers/acex/prj/../src -I/root/acextreme/drivers/acex/prj/../src/bus/pci -I/root/acextreme/drivers/acex/prj/../src/os/bus/pci -Dacexpci -Wall -DDDC_DBG_PRINT_ENABLED=0  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ddc_udl_os_debug)"  -D"KBUILD_MODNAME=KBUILD_STR(acexpci)" -D"DEBUG_HASH=4" -D"DEBUG_HASH2=30" -c -o /root/acextreme/drivers/acex/prj/../src/os/debug/.tmp_ddc_udl_os_debug.o /root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug.c

deps_/root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug.o := \
  /root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug.c \
  /usr/src/kernels/2.6.32-504.el6.i686/include/linux/kconfig.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  /root/acextreme/drivers/acex/prj/../src/os/include/ddc_os_types.h \
  include/linux/version.h \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/debug/ddc_udl_debug_private.h \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/debug/ddc_udl_debug.h \
    $(wildcard include/config/ctrl.h) \
  /root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug_private.h \

/root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug.o: $(deps_/root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug.o)

$(deps_/root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug.o):
