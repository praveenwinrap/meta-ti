DESCRIPTION = "MFP Linux utils for TI ARM/DSP processors"
SECTION = "devel"
LICENSE = "GPLv2 & BSD"

BASE_SRC_URI = "http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/linuxutils/${PV}/exports"
SRC_URI = "${BASE_SRC_URI}/linuxutils_${PV}.tar.gz "

LIC_FILES_CHKSUM = "file://gpl_2.0.txt;md5=1298794fc2eed3df92a89f3c9d317440"

S = "${WORKDIR}/linuxutils_${PV}"

require recipes-ti/includes/ti-paths.inc
require recipes-ti/includes/ti-staging.inc

PR = "${MACHINE_KERNEL_PR}"

#This is a kernel module, don't set PR directly
MACHINE_KERNEL_PR:append = "f"

inherit module

do_compile() {
    do_make_scripts
    unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS

    case "${SOC_FAMILY}" in
    "dm365")
            modules="cmem edma irq";;
    "omap3")
            modules="cmem sdma";;
    "dm355")
            modules="cmem edma irq";;
    *)
            modules="cmem"
    esac

    for module in $modules ; do
    cd ${S}/packages/ti/sdo/linuxutils/$module
    make \
      LINUXKERNEL_INSTALL_DIR="${STAGING_KERNEL_DIR}" \
      MVTOOL_PREFIX="${TARGET_PREFIX}" \
      UCTOOL_PREFIX="${TARGET_PREFIX}" \
      clean debug release
    done
}

do_install () {
    install -d ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/dsp
    # Install CMEM
    for module in $(find ${S}/packages/ti/sdo/linuxutils/ -name "*.ko") ; do
        install -m 0755 $module ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/dsp
    done
    cd ${S}/packages/ti/sdo/linuxutils/cmem/apps
    make \
      LINUXKERNEL_INSTALL_DIR="${STAGING_KERNEL_DIR}" \
      MVTOOL_PREFIX="${TARGET_PREFIX}" \
      UCTOOL_PREFIX="${TARGET_PREFIX}" \
      EXEC_DIR="${D}${installdir}/ti-linuxutils-app/cmem-app" \
      install
    # Install EDMA
    if [ "${SOC_FAMILY}" = "dm365" ] ; then
        cd ${S}/packages/ti/sdo/linuxutils/edma/apps
        make \
         LINUXKERNEL_INSTALL_DIR="${STAGING_KERNEL_DIR}" \
         MVTOOL_PREFIX="${TARGET_PREFIX}" \
         UCTOOL_PREFIX="${TARGET_PREFIX}" \
         EXEC_DIR="${D}${installdir}/ti-linuxutils-app/edma-app" \
         install
    fi

    # stage tree - other packages may need this
    install -d ${D}${LINUXUTILS_INSTALL_DIR_RECIPE}
    cp -pPrf ${S}/* ${D}${LINUXUTILS_INSTALL_DIR_RECIPE}
}




PACKAGES =+ " ti-cmem-module \
              ti-sdma-module \
              ti-edma-module \
              ti-irq-module \
              ti-linuxutils-app"

FILES:ti-cmem-module = "/lib/modules/${KERNEL_VERSION}/kernel/drivers/dsp/cmem*.ko"
FILES:ti-sdma-module = "/lib/modules/${KERNEL_VERSION}/kernel/drivers/dsp/sdma*.ko"
FILES:ti-edma-module = "/lib/modules/${KERNEL_VERSION}/kernel/drivers/dsp/edma*.ko"
FILES:ti-irq-module = "/lib/modules/${KERNEL_VERSION}/kernel/drivers/dsp/irq*.ko"

FILES:ti-linuxutils-app = "${installdir}/ti-linuxutils-app/*/*"

INSANE_SKIP:ti-linuxutils-app = "True"
INHIBIT_PACKAGE_STRIP = "1"
