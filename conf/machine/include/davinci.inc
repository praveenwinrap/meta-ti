require conf/machine/include/arm/armv5/tune-arm926ejs.inc

# Increase this everytime you change something in the kernel
MACHINE_KERNEL_PR = "r54"

KERNEL_IMAGETYPE = "zImage"

PREFERRED_PROVIDER_virtual/kernel ?= "linux-ti-staging"
PREFERRED_PROVIDER_virtual/bootloader = "u-boot-ti-staging"
PREFERRED_PROVIDER_u-boot = "u-boot-ti-staging"

UBOOT_MACHINE = "davinci_dvevm_config"

UBOOT_ENTRYPOINT = "0x80008000"
UBOOT_LOADADDRESS = "0x80008000"

EXTRA_IMAGEDEPENDS += "u-boot"

SERIAL_CONSOLES ?= "115200;ttyS0"
EXTRA_IMAGECMD:jffs2 = "--pad --little-endian --eraseblock=0x20000 -n"

MACHINE_FEATURES = "kernel26 serial ethernet usbhost usbgadget mmc alsa"
