#!/bin/bash -eu

PATH=/opt/arm/gcc-linaro-7.5.0-2019.12-i686_arm-linux-gnueabihf/bin:$PATH
export INSTALL_MOD_PATH=$(pwd)/modules
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

#make savedefconfig
#make menuconfig

make siklu_PCB277_V2_defconfig
make clean
make zImage
make modules
make imx6ull-14x14-siklu.dtb
make modules_install
