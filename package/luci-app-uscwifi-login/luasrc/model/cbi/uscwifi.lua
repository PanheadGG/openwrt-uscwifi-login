local uci = luci.model.uci.cursor()
local fs = require "nixio.fs"

local m = Map("uscwifi", "USC校园网登录设置")
local s = m:section(TypedSection, "main", "账号配置")
s.anonymous = true

s:option(Value, "username", "用户名")
s:option(Value, "password", "密码").password = true

-- 操作按钮
local btn_section = m:section(SimpleSection)
btn_section.title = "操作"
btn_section.description = [[
<a href="]] .. luci.dispatcher.build_url("admin/services/uscwifi/action_login") .. [[" class="cbi-button cbi-button-apply">手动登录</a>
<a href="]] .. luci.dispatcher.build_url("admin/services/uscwifi/action_status") .. [[" class="cbi-button cbi-button-reload">刷新状态</a>
]]

--状态展示
local stat_raw = fs.readfile("/var/run/uscwifi.status") or '{"msg":"未查询"}'
local s2 = m:section(SimpleSection)
s2.title = "在线状态"
s2.description = '<pre style="white-space:pre-wrap;">' .. stat_raw .. "</pre>"

return m
