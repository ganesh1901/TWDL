cmd_/root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_dio.o := gcc -Wp,-MD,/root/acextreme/drivers/acex/prj/../src/core/dio/.ddc_udl_dio.o.d  -nostdinc -isystem /usr/lib/gcc/i686-redhat-linux/4.4.7/include -Iinclude  -I/usr/src/kernels/2.6.32-504.el6.i686/include/uapi -I/usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include -Iarch/include/generated -Iinclude -include /usr/src/kernels/2.6.32-504.el6.i686/include/linux/kconfig.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -m32 -msoft-float -mregparm=3 -freg-struct-return -mpreferred-stack-boundary=2 -march=i686 -mtune=generic -Wa,-mtune=generic32 -ffreestanding -fstack-protector -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_AVX=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -Wframe-larger-than=1024 -Wno-unused-but-set-variable -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-dwarf2-cfi-asm -fconserve-stack -I/root/acextreme/drivers/acex/prj/../src -I/root/acextreme/drivers/acex/prj/../src/bus/pci -I/root/acextreme/drivers/acex/prj/../src/os/bus/pci -Dacexpci -Wall -DDDC_DBG_PRINT_ENABLED=0  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ddc_udl_dio)"  -D"KBUILD_MODNAME=KBUILD_STR(acexpci)" -D"DEBUG_HASH=32" -D"DEBUG_HASH2=37" -c -o /root/acextreme/drivers/acex/prj/../src/core/dio/.tmp_ddc_udl_dio.o /root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_dio.c

deps_/root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_dio.o := \
  /root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_dio.c \
  /usr/src/kernels/2.6.32-504.el6.i686/include/linux/kconfig.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  /root/acextreme/drivers/acex/prj/../src/os/include/ddc_os_types.h \
  include/linux/version.h \
  /root/acextreme/drivers/acex/prj/../src/include/ddc_ioctl.h \
    $(wildcard include/config/id.h) \
    $(wildcard include/config/set.h) \
    $(wildcard include/config/clr.h) \
    $(wildcard include/config/update.h) \
  /root/acextreme/drivers/acex/prj/../src/os/include/ddc_os_ioctl.h \
  /root/acextreme/drivers/acex/prj/../src/include/ddc_error_list.h \
  /root/acextreme/drivers/acex/prj/../src/include/ddc_arinc_error_list.h \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/ddc_udl_private.h \
    $(wildcard include/config/state.h) \
  /root/acextreme/drivers/acex/prj/../src/os/include/ddc_os_types_private.h \
  include/linux/spinlock.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/typecheck.h \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/types.h \
  include/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/posix_types_32.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/bitops.h \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/x86/cmov.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/alternative.h \
    $(wildcard include/config/paravirt.h) \
  include/linux/stringify.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/asm.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/cpufeature.h \
    $(wildcard include/config/x86/invlpg.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/x86/cmpxchg64.h) \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/x86/p6/nop.h) \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/swab.h \
    $(wildcard include/config/x86/bswap.h) \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/minix.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/page.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/page_types.h \
  include/linux/const.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/page_32_types.h \
    $(wildcard include/config/highmem4g.h) \
    $(wildcard include/config/highmem64g.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/4kstacks.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/page_32.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/x86/3dnow.h) \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /usr/lib/gcc/i686-redhat-linux/4.4.7/include/stdarg.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/string.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/string_32.h \
    $(wildcard include/config/kmemcheck.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/asm-generic/getorder.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/processor.h \
    $(wildcard include/config/x86/vsmp.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/m386.h) \
    $(wildcard include/config/m486.h) \
    $(wildcard include/config/x86/debugctlmsr.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/vm86.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/ptrace.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/ptrace-abi.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/segment.h \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/math_emu.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/sigcontext.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/current.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/percpu.h \
    $(wildcard include/config/x86/64/smp.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/linkage.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/alignment/16.h) \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_debug.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/div64.h \
  include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/system.h \
    $(wildcard include/config/ia32/emulation.h) \
    $(wildcard include/config/x86/32/lazy/gs.h) \
    $(wildcard include/config/x86/ppro/fence.h) \
    $(wildcard include/config/x86/oostore.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/cmpxchg.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/cmpxchg_32.h \
    $(wildcard include/config/x86/cmpxchg.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/irqflags.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/paravirt.h \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/paravirt/spinlocks.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/compat/vdso.h) \
    $(wildcard include/config/proc/fs.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pgtable_32_types.h \
    $(wildcard include/config/highmem.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pgtable-3level_types.h \
  include/asm-generic/pgtable-nopud.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/paravirt_types.h \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/paravirt/debug.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/desc_defs.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  include/asm-generic/kmap_types.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/msr.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/msr-index.h \
  include/linux/ioctl.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/cpumask.h \
  include/linux/personality.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/cache.h \
    $(wildcard include/config/x86/l1/cache/shift.h) \
  include/linux/math64.h \
  include/linux/err.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/ftrace.h \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
    $(wildcard include/config/ftrace/syscalls.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/atomic.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/atomic_32.h \
  include/asm-generic/atomic-long.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/spinlock.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/rwlock.h \
  include/linux/spinlock_api_smp.h \
  include/linux/vmalloc.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/have/legacy/per/cpu/area.h) \
  /root/acextreme/drivers/acex/prj/../src/os/bus/pci/ddc_udl_os_bus_private.h \
  include/linux/pci.h \
    $(wildcard include/config/pci/iov.h) \
    $(wildcard include/config/pcieaspm.h) \
    $(wildcard include/config/pci/msi.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/pci/legacy.h) \
    $(wildcard include/config/pcieaer.h) \
    $(wildcard include/config/pcie/ecrc.h) \
    $(wildcard include/config/ht/irq.h) \
    $(wildcard include/config/pci/domains.h) \
    $(wildcard include/config/pci/mmconfig.h) \
    $(wildcard include/config/hotplug/pci.h) \
  include/linux/pci_regs.h \
  include/linux/mod_devicetable.h \
  include/linux/ioport.h \
  include/linux/errno.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
    $(wildcard include/config/sysfs.h) \
  include/linux/kref.h \
  include/linux/wait.h \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
  include/linux/klist.h \
  include/linux/module.h \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
  include/linux/stat.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/seqlock.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/bounds.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/have/default/no/spin/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/rwsem.h \
  include/linux/srcu.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/sparsemem.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/smp.h \
    $(wildcard include/config/x86/io/apic.h) \
    $(wildcard include/config/x86/32/smp.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/mpspec.h \
    $(wildcard include/config/x86/numaq.h) \
    $(wildcard include/config/mca.h) \
    $(wildcard include/config/eisa.h) \
    $(wildcard include/config/x86/mpparse.h) \
    $(wildcard include/config/acpi.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/mpspec_def.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/x86_init.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/bootparam.h \
  include/linux/screen_info.h \
  include/linux/apm_bios.h \
  include/linux/edd.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/e820.h \
    $(wildcard include/config/efi.h) \
    $(wildcard include/config/intel/txt.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/memtest.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/ist.h \
  include/video/edid.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/apic.h \
    $(wildcard include/config/x86/x2apic.h) \
  include/linux/delay.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/delay.h \
  include/linux/pm.h \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/ppc/pseries.h) \
  include/linux/workqueue.h \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/timex.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/tsc.h \
    $(wildcard include/config/x86/tsc.h) \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/completion.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/apicdef.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/fixmap.h \
    $(wildcard include/config/provide/ohci1394/dma/init.h) \
    $(wildcard include/config/x86/visws/apic.h) \
    $(wildcard include/config/x86/f00f/bug.h) \
    $(wildcard include/config/x86/cyclone/timer.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/acpi.h \
    $(wildcard include/config/acpi/numa.h) \
  include/acpi/pdc_intel.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/numa.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/numa_32.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/mmu.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/idle.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/io_apic.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/irq_vectors.h \
    $(wildcard include/config/sparse/irq.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/topology.h \
    $(wildcard include/config/x86/ht.h) \
    $(wildcard include/config/x86/64/acpi/numa.h) \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
  include/linux/sysctl.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/elf.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/user.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/user_32.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/auxvec.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/vdso.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/desc.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/ldt.h \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  include/linux/prio_tree.h \
  include/linux/rbtree.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
  include/linux/rcutree.h \
    $(wildcard include/config/no/hz.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/local.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/debug/kmemleak.h) \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slab_def.h \
    $(wildcard include/config/kmemtrace.h) \
  include/linux/kmalloc_sizes.h \
  include/linux/pfn.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/module.h \
    $(wildcard include/config/m586.h) \
    $(wildcard include/config/m586tsc.h) \
    $(wildcard include/config/m586mmx.h) \
    $(wildcard include/config/mcore2.h) \
    $(wildcard include/config/matom.h) \
    $(wildcard include/config/m686.h) \
    $(wildcard include/config/mpentiumii.h) \
    $(wildcard include/config/mpentiumiii.h) \
    $(wildcard include/config/mpentiumm.h) \
    $(wildcard include/config/mpentium4.h) \
    $(wildcard include/config/mk6.h) \
    $(wildcard include/config/mk8.h) \
    $(wildcard include/config/x86/elan.h) \
    $(wildcard include/config/mcrusoe.h) \
    $(wildcard include/config/mefficeon.h) \
    $(wildcard include/config/mwinchipc6.h) \
    $(wildcard include/config/mwinchip3d.h) \
    $(wildcard include/config/mcyrixiii.h) \
    $(wildcard include/config/mviac3/2.h) \
    $(wildcard include/config/mviac7.h) \
    $(wildcard include/config/mgeodegx1.h) \
    $(wildcard include/config/mgeode/lx.h) \
  include/asm-generic/module.h \
  include/trace/events/module.h \
  include/trace/define_trace.h \
  include/linux/semaphore.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/device.h \
    $(wildcard include/config/dmar.h) \
  include/linux/pm_wakeup.h \
    $(wildcard include/config/pm.h) \
  include/linux/io.h \
    $(wildcard include/config/has/ioport.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/io.h \
    $(wildcard include/config/mtrr.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/io_32.h \
  include/asm-generic/iomap.h \
  include/linux/irqreturn.h \
  include/linux/pci_ids.h \
  include/linux/dmapool.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/scatterlist.h \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pci.h \
  include/linux/mm.h \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/bit_spinlock.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pgtable.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pgtable_32.h \
    $(wildcard include/config/highpte.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pgtable_32_types.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/mm_track.h \
    $(wildcard include/config/track/dirty/pages.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/pgtable-3level.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/s390.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/asm-generic/pci-dma-compat.h \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/bug.h \
  include/linux/scatterlist.h \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/dma-mapping.h \
    $(wildcard include/config/isa.h) \
  include/linux/kmemcheck.h \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  /usr/src/kernels/2.6.32-504.el6.i686/arch/x86/include/asm/swiotlb.h \
    $(wildcard include/config/swiotlb.h) \
  include/linux/swiotlb.h \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  include/asm-generic/dma-mapping-common.h \
  include/linux/pci-dma.h \
  include/asm-generic/pci.h \
  /root/acextreme/drivers/acex/prj/../src/include/ddc_types.h \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/ddc_udl_um_private.h \
    $(wildcard include/config/pulse.h) \
    $(wildcard include/config/mask/irig/input/range.h) \
    $(wildcard include/config/mask/int/tt/cnt/en.h) \
    $(wildcard include/config/mask/ext/tt/cnt/en.h) \
    $(wildcard include/config/mask/ext/tt/ld/pul/en.h) \
    $(wildcard include/config/mask/ext/tt/clk/en.h) \
    $(wildcard include/config/mask/clk/20/out/en.h) \
    $(wildcard include/config/pul/mask/ext/io/reset.h) \
    $(wildcard include/config/pul/mask/irigb/reset.h) \
    $(wildcard include/config/pul/mask/429/reset.h) \
    $(wildcard include/config/pul/mask/1553/1/reset.h) \
    $(wildcard include/config/pul/mask/1553/0/reset.h) \
    $(wildcard include/config/pulse/mask/bd/reset.h) \
    $(wildcard include/config/pul/mask/mstr/reset.h) \
    $(wildcard include/config/pul/mask/test/int.h) \
    $(wildcard include/config/addr/shift.h) \
    $(wildcard include/config/module/en.h) \
    $(wildcard include/config/mrt/mode/en.h) \
    $(wildcard include/config/srt/address/source.h) \
    $(wildcard include/config/srt/address.h) \
    $(wildcard include/config/srt/address/parity.h) \
    $(wildcard include/config/srt/address/latch/trk.h) \
    $(wildcard include/config/latch/srt/address.h) \
    $(wildcard include/config/mask/mode/code/reset.h) \
    $(wildcard include/config/mask/brodcast/disabled.h) \
    $(wildcard include/config/mask/rt/configured.h) \
    $(wildcard include/config/gbl/dstk/en.h) \
    $(wildcard include/config/int/status/queue/en.h) \
    $(wildcard include/config/mask/rt/cmd/stk/sz.h) \
    $(wildcard include/config/ba.h) \
    $(wildcard include/config/rt/fail/flag/wrap/en.h) \
    $(wildcard include/config/clr/serv/request.h) \
    $(wildcard include/config/rt/halt.h) \
    $(wildcard include/config/enh/tt/sync.h) \
    $(wildcard include/config/ld/tt/on/mode/sync.h) \
    $(wildcard include/config/clr/tt/on/mode/sync.h) \
    $(wildcard include/config/dont/post/on/cmd/stk.h) \
    $(wildcard include/config/brdcst/dis.h) \
    $(wildcard include/config/isq/dis/valid/msg.h) \
    $(wildcard include/config/isq/dis/invalid/msg.h) \
    $(wildcard include/config/ext/bitwd/if/busy.h) \
    $(wildcard include/config/inhib/bitwd/if/busy.h) \
    $(wildcard include/config/mode/cmds/override/busy.h) \
    $(wildcard include/config/alt/status/wd/en.h) \
    $(wildcard include/config/1553a/error/resp/en.h) \
    $(wildcard include/config/1553a/mc/en.h) \
    $(wildcard include/config/gap/chk/en.h) \
    $(wildcard include/config/busy/rx/tfr/dis.h) \
    $(wildcard include/config/ill/rx/tfr/dis.h) \
    $(wildcard include/config/busy/lkup/tbl/en.h) \
    $(wildcard include/config/ill/dis.h) \
    $(wildcard include/config/rx/db/en.h) \
    $(wildcard include/config/no/resp/timeout/mask.h) \
    $(wildcard include/config/no/resp/timeout/shift.h) \
    $(wildcard include/config/hpq/enable.h) \
    $(wildcard include/config/lpq/enable.h) \
    $(wildcard include/config/resp/timeout/mask.h) \
    $(wildcard include/config/resp/timeout/shift.h) \
    $(wildcard include/config/rw.h) \
    $(wildcard include/config/mode/mask.h) \
    $(wildcard include/config/mode/mtie/enable.h) \
    $(wildcard include/config/mti/block/status/enable.h) \
    $(wildcard include/config/mti/selective/monitor/disable.h) \
    $(wildcard include/config/cha/disable.h) \
    $(wildcard include/config/chb/disable.h) \
    $(wildcard include/config/gap/check/enable.h) \
    $(wildcard include/config/broadcast/disable.h) \
    $(wildcard include/config/1553a/mcodes/enable.h) \
    $(wildcard include/config/override/mc/tr/err/enable.h) \
    $(wildcard include/config/busy/bit/nodata/valid/format/enable.h) \
    $(wildcard include/config/eom/ttag/enable.h) \
    $(wildcard include/config/bwe/swap/enable.h) \
    $(wildcard include/config/bus/switch/eom/disable.h) \
    $(wildcard include/config/mode/mtir/enable.h) \
    $(wildcard include/config/trigger/start/enable.h) \
    $(wildcard include/config/trigger/stop/enable.h) \
    $(wildcard include/config/no/resp/timeout/bit/align.h) \
    $(wildcard include/config/no/resp/timeout/default.h) \
    $(wildcard include/config/reserved/07.h) \
    $(wildcard include/config/reserved/06.h) \
    $(wildcard include/config/reserved/05.h) \
    $(wildcard include/config/reserved/04.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/mask.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/256/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/512/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/1024/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/2048/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/4096/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/8192/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/16384/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/32768/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/65536/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/131072/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/262144/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/524288/dw.h) \
    $(wildcard include/config/mt/cmd/data/stack/size/1048576/dw.h) \
    $(wildcard include/config/reg/a/ba.h) \
    $(wildcard include/config/reg/b/ba.h) \
    $(wildcard include/config/reg/c/ba.h) \
    $(wildcard include/config/int/enable.h) \
    $(wildcard include/config/ch1/interrupt/trigger.h) \
    $(wildcard include/config/ch2/interrupt/trigger.h) \
    $(wildcard include/config/interrupt/trigger.h) \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/ddc_udl_dma_private.h \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/ddc_udl_queueop_private.h \
  /root/acextreme/drivers/acex/prj/../src/core/irig/ddc_udl_irigb_private.h \
  /root/acextreme/drivers/acex/prj/../src/core/can/ddc_udl_can_private.h \
    $(wildcard include/config/option/mem/offset.h) \
    $(wildcard include/config/option/run/state/offset.h) \
    $(wildcard include/config/option/speed/offset.h) \
    $(wildcard include/config/option/msg/cnt/int/offset.h) \
    $(wildcard include/config/option/timer/int/offset.h) \
    $(wildcard include/config/option/tx/debug/header.h) \
    $(wildcard include/config/option/tx/debug/id.h) \
    $(wildcard include/config/option/tx/debug/length.h) \
    $(wildcard include/config/option/tx/debug/words/1/4.h) \
    $(wildcard include/config/option/tx/debug/words/5/8.h) \
    $(wildcard include/config/option/monitor/state.h) \
    $(wildcard include/config/option/loop/back/offset.h) \
    $(wildcard include/config/option/rx/filter/offset.h) \
  /root/acextreme/drivers/acex/prj/../src/core/can/ddc_udl_can.h \
    $(wildcard include/config/pending.h) \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/ddc_udl_sdk.h \
  /root/acextreme/drivers/acex/prj/../src/core/1553/ddc_udl_1553_common_private.h \
  /root/acextreme/drivers/acex/prj/../src/core/serial_io/ddc_udl_serial_io_private.h \
  /root/acextreme/drivers/acex/prj/../src/core/serial_io/ddc_udl_serial_io.h \
  /root/acextreme/drivers/acex/prj/../src/core/arinc429/ddc_udl_arinc429.h \
    $(wildcard include/config/interrupt/conditions.h) \
    $(wildcard include/config/timestamp.h) \
    $(wildcard include/config/repeater.h) \
    $(wildcard include/config//output/type.h) \
  /root/acextreme/drivers/acex/prj/../src/core/arinc429/ddc_udl_arinc429_private.h \
  /root/acextreme/drivers/acex/prj/../src/core/arinc717/ddc_udl_arinc717_private.h \
  /root/acextreme/drivers/acex/prj/../src/core/arinc717/ddc_udl_arinc717.h \
  /root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_diott_private.h \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/ddc_udl_bd_private.h \
    $(wildcard include/config/type.h) \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/debug/ddc_udl_debug_private.h \
  /root/acextreme/drivers/acex/prj/../src/driver_sdk/debug/ddc_udl_debug.h \
    $(wildcard include/config/ctrl.h) \
  /root/acextreme/drivers/acex/prj/../src/os/debug/ddc_udl_os_debug_private.h \

/root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_dio.o: $(deps_/root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_dio.o)

$(deps_/root/acextreme/drivers/acex/prj/../src/core/dio/ddc_udl_dio.o):
