#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_SITE_LENGTH 50
#define MAX_PROXYTYPE_LENGTH 10

// 定义配置和代理列表文件的名称
char CONFIG_FILE[] = "config.txt";
char PROXY_LIST_FILE[] = "proxylist.txt";

// 用于存储配置的结构体
struct Config {
    int timeout;
    char proxytype[MAX_PROXYTYPE_LENGTH];
};

void createConfig();
void checkProxies();
void setColor(int color);

int main(int argc, char* argv[]) {
    system("chcp 65001 > nul");

    if (argc < 2) {
        printf("使用方法: %s <命令>\n命令:\n start - 生成配置文件\n check - 检查代理\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "start") == 0) {
        createConfig();
    }
    else if (strcmp(argv[1], "check") == 0) {
        checkProxies();
    }
    else {
        printf("未知命令: %s\n", argv[1]);
    }

    return 0;
}

// 创建配置文件和代理列表文件
void createConfig() {
    FILE* configFile;
    FILE* proxyListFile;

    // 创建config.txt
    fopen_s(&configFile, CONFIG_FILE, "w");
    if (configFile == NULL) {
        perror("无法创建配置文件");
        exit(1);
    }
    fprintf(configFile, "timeout = 5000ms\n");
    fprintf(configFile, "proxytype = http\n");
    fclose(configFile);

    // 创建proxylist.txt
    fopen_s(&proxyListFile, PROXY_LIST_FILE, "w");
    if (proxyListFile == NULL) {
        perror("无法创建代理列表文件");
        exit(1);
    }
    fprintf(proxyListFile, "www.google.com\n");
    fclose(proxyListFile);

    printf("配置文件和代理列表文件已创建。\n");
}

// 检查代理列表中的代理
void checkProxies() {
    system("chcp 936 > nul");

    struct Config config = {};
    FILE* configFile;
    FILE* proxyListFile;
    char proxy[MAX_SITE_LENGTH];
    char command[100];
    int result;

    // 读取配置文件
    
    if (fopen_s(&configFile, CONFIG_FILE, "r") != 0) {
        perror("无法打开配置文件");
        exit(1);
    }
    fscanf_s(configFile, "timeout = %dms\n", &config.timeout);
    fscanf_s(configFile, "proxytype = %s\n", &config.proxytype, MAX_PROXYTYPE_LENGTH);
    fclose(configFile);

    // 打开代理列表文件
    if (fopen_s(&proxyListFile, PROXY_LIST_FILE, "r") != 0) {
        perror("无法打开代理列表文件");
        exit(1);
    }

    // 逐行读取代理并检查
    while (fgets(proxy, MAX_SITE_LENGTH, proxyListFile) != NULL) {
        // 构建ping命令
        snprintf(command, sizeof(command), "ping -n 1 -w %d %s", config.timeout, proxy);
        // 执行ping命令
        result = system(command);
        // 根据结果设置颜色
        switch (result) {
        case 0:
            setColor(2); // 绿色
            break;
        case 1:
            setColor(14); // 黄色
            break;
        case 2:
            setColor(6); // 橙色
            break;
        default:
            setColor(4); // 红色
        }
        printf("%s\n", proxy);
        setColor(7); // 重置为默认颜色
    }

    fclose(proxyListFile);
}

// 设置控制台文本颜色
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}