#
#
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=moter_pwm
PKG_RELEASE:=1

define KernelPackage/moter_pwm
  SUBMENU:=Other modules
  TITLE:=moter_pwm
  FILES:=$(PKG_BUILD_DIR)/moter_pwm.ko
  KCONFIG:=
endef

define KernelPackage/moter_pwm/description
  This is a 4 channels drivers
endef

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)"

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
	$(MAKE_OPTS) modules
endef

$(eval $(call KernelPackage,moter_pwm))
