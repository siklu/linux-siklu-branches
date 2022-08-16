#!/bin/bash -eu

PATH=/opt/arm/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin:$PATH
export INSTALL_MOD_PATH=$(pwd)/modules
export ARCH=arm
export CROSS_COMPILE=arm-none-linux-gnueabihf-

make siklu_PCB277_V2_defconfig
#make menuconfig
#make savedefconfig
make clean
make zImage
make modules
make imx6ull-14x14-siklu.dtb
make modules_install
