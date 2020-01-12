cmd_drivers/net/wireless/esp8089/esp_ext.o := arm-himix100-linux-gcc -Wp,-MD,drivers/net/wireless/esp8089/.esp_ext.o.d  -nostdinc -isystem /opt/hisi-linux/x86-arm/arm-himix100-linux/host_bin/../lib/gcc/arm-linux-uclibceabi/6.3.0/include -I./arch/arm/include -I./arch/arm/include/generated/uapi -I./arch/arm/include/generated  -I./include -I./arch/arm/include/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hibvt/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-PIE -fno-dwarf2-cfi-asm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -fno-ipa-sra -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -marm -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -fno-delete-null-pointer-checks -Wno-frame-address -O2 --param=allow-store-data-races=0 -DCC_HAVE_ASM_GOTO -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -Wno-unused-const-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types  -DMODULE  -DKBUILD_BASENAME='"esp_ext"'  -DKBUILD_MODNAME='"esp8089"' -c -o drivers/net/wireless/esp8089/esp_ext.o drivers/net/wireless/esp8089/esp_ext.c

source_drivers/net/wireless/esp8089/esp_ext.o := drivers/net/wireless/esp8089/esp_ext.c

deps_drivers/net/wireless/esp8089/esp_ext.o := \

drivers/net/wireless/esp8089/esp_ext.o: $(deps_drivers/net/wireless/esp8089/esp_ext.o)

$(deps_drivers/net/wireless/esp8089/esp_ext.o):
