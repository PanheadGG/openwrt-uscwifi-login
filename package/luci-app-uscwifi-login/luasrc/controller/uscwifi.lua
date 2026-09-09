module("luci.controller.uscwifi", package.seeall)
function index()
    entry({"admin", "services", "uscwifi"}, cbi("uscwifi"), _("USC WiFi Login"), 65)
    entry({"admin", "services", "uscwifi", "action_login"}, call("action_login"))
    entry({"admin", "services", "uscwifi", "action_status"}, call("action_status"))
end

function action_login()
    luci.util.exec("/usr/bin/uscwifi-logind login")
    luci.http.redirect(luci.dispatcher.build_url("admin/services/uscwifi"))
end

function action_status()
    luci.util.exec("/usr/bin/uscwifi-logind status")
    luci.http.redirect(luci.dispatcher.build_url("admin/services/uscwifi"))
end
