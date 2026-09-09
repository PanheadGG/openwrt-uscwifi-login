#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uci.h>
#include "cJSON.h"

#define STATUS_FILE "/var/run/uscwifi.status"
#define COOKIE_FILE "/tmp/uscwifi_cookie.txt"
static char g_username[64] = "";
static char g_password[64] = "";

//读取UCI
static int read_uci_cfg(void)
{
    struct uci_context *ctx = uci_alloc_context();
    struct uci_ptr ptr;
    char buf[128];

    if (!ctx) return -1;
    uci_set_confdir(ctx, "/etc/config");

#define GET_UCI(cfg,sec,opt,out,sz) do{ \
    snprintf(buf,sizeof(buf),"%s.%s.%s",cfg,sec,opt);\
    if (!uci_lookup_ptr(ctx, &ptr, buf, true) && ptr.o && ptr.o->type == UCI_TYPE_STRING && ptr.o->v.string) {\
        strncpy(out, ptr.o->v.string, sz-1);\
        out[sz-1] = 0;\
    }\
}while(0)

    GET_UCI("uscwifi","main","username", g_username, sizeof(g_username));
    GET_UCI("uscwifi","main","password", g_password, sizeof(g_password));
    uci_free_context(ctx);
    return 0;
}

//执行命令获取输出
static int exec_cmd(const char* cmd, char* out, size_t outsz)
{
    FILE* fp = popen(cmd, "r");
    if(!fp) return -1;
    size_t idx = 0;
    int ch;
    memset(out,0,outsz);
    while ((ch=fgetc(fp)) != EOF && idx < outsz-1)
    {
        out[idx++] = (char)ch;
    }
    pclose(fp);
    return 0;
}

//仅查询状态
static int query_status()
{
    char cmd_buf[1024];
    char resp[4096];
    snprintf(cmd_buf, sizeof(cmd_buf),
        "uclient-fetch -b %s -O- http://210.43.112.9/api/account/status",
        COOKIE_FILE);
    exec_cmd(cmd_buf, resp, sizeof(resp));

    cJSON *status_root = cJSON_Parse(resp);
    char out_json[2048];
    if(status_root)
    {
        cJSON *code = cJSON_GetObjectItemCaseSensitive(status_root, "code");
        cJSON *msg = cJSON_GetObjectItemCaseSensitive(status_root, "msg");
        cJSON *online = cJSON_GetObjectItemCaseSensitive(status_root, "online");
        cJSON *userip = NULL;
        cJSON *token = cJSON_GetObjectItemCaseSensitive(status_root, "token");
        if(online) userip = cJSON_GetObjectItemCaseSensitive(online, "UserIpv4");

        snprintf(out_json, sizeof(out_json),
            "{\"ok\":%s,\"code\":%d,\"msg\":\"%s\",\"UserIpv4\":\"%s\",\"token\":\"%.30s...\"}",
            (code && code->valuedouble == 0) ? "true" : "false",
            code ? (int)code->valuedouble : -1,
            cJSON_IsString(msg) ? msg->valuestring : "",
            cJSON_IsString(userip) ? userip->valuestring : "",
            cJSON_IsString(token) ? token->valuestring : ""
        );
        cJSON_Delete(status_root);
    }else{
        snprintf(out_json, sizeof(out_json), "{\"ok\":false,\"raw\":\"%.300s\"}", resp);
    }
    FILE *fp = fopen(STATUS_FILE,"w");
    if(fp){fputs(out_json,fp);fclose(fp);}
    return 0;
}

//完整登录流程
static int do_login_flow()
{
    char cmd_buf[1024];
    char resp[4096];
    char csrf[128];

    if(strlen(g_username)==0 || strlen(g_password)==0)
    {
        FILE *fp = fopen(STATUS_FILE,"w");
        if(fp){fputs("{\"ok\":false,\"msg\":\"账号或密码未填写\"}",fp);fclose(fp);}
        return -1;
    }

    // Step1 获取csrf + 保存cookie
    snprintf(cmd_buf, sizeof(cmd_buf),
        "uclient-fetch -c %s -O- http://210.43.112.9/api/csrf-token",
        COOKIE_FILE);
    exec_cmd(cmd_buf, resp, sizeof(resp));
    cJSON *csrf_root = cJSON_Parse(resp);
    if(!csrf_root)
    {
        FILE *fp = fopen(STATUS_FILE,"w");
        if(fp){fputs("{\"ok\":false,\"msg\":\"获取csrf失败\"}",fp);fclose(fp);}
        return -2;
    }
    cJSON *csrf_item = cJSON_GetObjectItemCaseSensitive(csrf_root,"csrf_token");
    if(cJSON_IsString(csrf_item)) strncpy(csrf, csrf_item->valuestring, sizeof(csrf)-1);
    cJSON_Delete(csrf_root);

    // Step2 POST login form-data
    snprintf(cmd_buf, sizeof(cmd_buf),
        "uclient-fetch -b %s -H \"X-CSRF-Token: %s\" -F \"username=%s\" -F \"password=%s\" http://210.43.112.9/api/account/login",
        COOKIE_FILE, csrf, g_username, g_password);
    exec_cmd(cmd_buf, resp, sizeof(resp));

    // Step3 查询状态更新页面
    query_status();
    return 0;
}

int main(int argc, char **argv)
{
    read_uci_cfg();
    if(argc >=2 && strcmp(argv[1],"login") == 0)
    {
        do_login_flow();
    }else if(argc >=2 && strcmp(argv[1],"status") ==0)
    {
        query_status();
    }else{
        FILE *fp = fopen(STATUS_FILE,"w");
        if(fp){fputs("{\"ok\":false,\"msg\":\"参数错误，使用 login / status\"}",fp);fclose(fp);}
    }
    return 0;
}
