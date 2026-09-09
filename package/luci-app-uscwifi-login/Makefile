include $(TOPDIR)/rules.mk

PKG_NAME:=luci-app-uscwifi-login
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_MAINTAINER:=PanheadGG
PKG_LICENSE:=MIT
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/luci-app-uscwifi-login
  SECTION:=luci
  CATEGORY:=LuCI
  SUBMENU:=3. Applications
  TITLE:=USC WiFi Login WebUI
  DEPENDS:=+luci-base +luci-lua-runtime +uscwifi-logind
  PKGARCH:=all
endef

define Package/luci-app-uscwifi-login/description
 LuCI UI for uscwifi-login
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./luasrc/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/luci-app-uscwifi-login/install
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci
	cp -pR $(PKG_BUILD_DIR)/* $(1)/usr/lib/lua/luci/
endef

define Package/luci-app-uscwifi-login/postinst
#!/bin/sh
[ -n "$${IPKG_INSTROOT}" ] || {
	rm -f /tmp/luci-indexcache.*
	rm -rf /tmp/luci-modulecache/
	/etc/init.d/rpcd reload 2>/dev/null
	exit 0
}
endef

$(eval $(call BuildPackage,luci-app-uscwifi-login))
