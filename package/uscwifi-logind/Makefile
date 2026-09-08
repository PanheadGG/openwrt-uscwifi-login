include $(TOPDIR)/rules.mk

PKG_NAME:=uscwifi-logind
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/uscwifi-logind
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=USC WiFi Login Tool
  DEPENDS:=+libuci +uclient-fetch
endef

define Package/uscwifi-logind/description
 USC campus wifi login tool, manual trigger only.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(TARGET_LDFLAGS) \
	-o $(PKG_BUILD_DIR)/uscwifi-logind \
	$(PKG_BUILD_DIR)/main.c $(PKG_BUILD_DIR)/cJSON.c \
	-luci
endef

define Package/uscwifi-logind/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/uscwifi-logind $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/uscwifi.config $(1)/etc/config/uscwifi

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/uscwifi.init $(1)/etc/init.d/uscwifi
endef

$(eval $(call BuildPackage,uscwifi-logind))
