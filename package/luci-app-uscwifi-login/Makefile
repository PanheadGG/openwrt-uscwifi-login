include $(TOPDIR)/rules.mk

PKG_NAME:=luci-app-uscwifi-login
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_MAINTAINER:=PanheadGG
PKG_LICENSE:=MIT

include $(INCLUDE_DIR)/package.mk

define Package/luci-app-uscwifi-login
  SECTION:=luci
  CATEGORY:=LuCI
  SUBMENU:=3. Applications
  TITLE:=USC WiFi Login WebUI
  DEPENDS:=+uscwifi-logind
endef

define Package/luci-app-uscwifi-login/description
 LuCI UI for uscwifi-login
endef

define Build/Compile
endef

define Package/luci-app-uscwifi-login/install
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci
	cp -r ./luasrc/* $(1)/usr/lib/lua/luci/
endef

$(eval $(call BuildPackage,luci-app-uscwifi-login))
