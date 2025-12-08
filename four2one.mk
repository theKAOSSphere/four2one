######################################
# Buildroot package fragment for Four2One
######################################

FOUR2ONE_SITE_METHOD = local
FOUR2ONE_SITE = $($(PKG)_PKGDIR)/
FOUR2ONE_DEPENDENCIES =
FOUR2ONE_BUNDLES = kaoss-four2one.lv2

# version (bump to force rebuild if needed)
FOUR2ONE_VERSION = 1

# toolchain PATH trimmed to avoid inheriting Windows host entries with spaces
FOUR2ONE_TOOLCHAIN_PATH = $(HOST_DIR)/bin:$(HOST_DIR)/sbin:$(HOST_DIR)/usr/bin:$(HOST_DIR)/usr/sbin:/usr/bin:/bin

# Use standard variables for cross-compilation, explicitly passed to override environment if needed
FOUR2ONE_TARGET_MAKE = PATH="$(FOUR2ONE_TOOLCHAIN_PATH)" \
	CC="$(TARGET_CC)" CXX="$(TARGET_CXX)" AR="$(TARGET_AR)" LD="$(TARGET_LD)" \
	PKG_CONFIG="$(TARGET_PKG_CONFIG)" \
	CFLAGS="$(TARGET_CFLAGS)" CXXFLAGS="$(TARGET_CXXFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	$(MAKE) -C $(@D)

# This tells the builder to run make inside the 'source' directory
define FOUR2ONE_BUILD_CMDS
    $(FOUR2ONE_TARGET_MAKE) -C $(@D)/source
endef

define FOUR2ONE_INSTALL_TARGET_CMDS
	$(FOUR2ONE_TARGET_MAKE) -C $(@D)/source install DESTDIR=$(TARGET_DIR)
endef

$(eval $(generic-package))
