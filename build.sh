#!/bin/bash

PATH=/opt/arm/gcc-linaro-7.5.0-2019.12-i686_arm-linux-gnueabihf/bin:$PATH
export INSTALL_MOD_PATH=$(pwd)/modules
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

make menuconfig
make clean
make zImage
make modules
make imx6ull-14x14-siklu.dtb
make modules_install
