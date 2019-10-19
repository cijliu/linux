cp arch/arm/configs/hi3516ev200_full_defconfig  .config
make ARCH=arm CROSS_COMPILE=arm-himix100-linux- uImage
