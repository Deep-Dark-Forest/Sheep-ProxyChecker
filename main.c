#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 定义配置和代理列表文件的名称
#define CONFIG_FILE "config.txt"
#define PROXY_LIST_FILE "proxylist.txt"

// 用于存储配置的结构体
struct Config {
    int timeout;
    char testsite[50];
    char proxytype[10];
};

// 函数声明
void createConfig();
void checkProxies();
void setColor(int color);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("使用方法: %s <命令>\n命令:\n start - 生成配置文件\n check - 检查代理\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "start") == 0) {
        createConfig();
    } else if (strcmp(argv[1], "check") == 0) {
        checkProxies();
    } else {
        printf("未知命令: %s\n", argv[1]);
    }

    return 0;
}

// 创建配置文件和代理列表文件
void createConfig() {
    FILE *configFile;
    FILE *proxyListFile;

    // 创建config.txt
    configFile = fopen(CONFIG_FILE, "w");
    if (configFile == NULL) {
        perror("无法创建配置文件");
        exit(1);
    }
    fprintf(configFile, "timeout = 5000ms\n");
    fprintf(configFile, "testsite = https://www.google.com\n");
    fprintf(configFile, "proxytype = http\n");
    fclose(configFile);

    // 创建proxylist.txt
    proxyListFile = fopen(PROXY_LIST_FILE, "w");
    if (proxyListFile == NULL) {
        perror("无法创建代理列表文件");
        exit(1);
    }
    fclose(proxyListFile);

    printf("配置文件和代理列表文件已创建。\n");
}

// 检查代理列表中的代理
void checkProxies() {
    struct Config config;
    FILE *configFile;
    FILE *proxyListFile;
    char proxy[50];
    char command[100];
    int result;

    // 读取配置文件
    configFile = fopen(CONFIG_FILE, "r");
    if (configFile == NULL) {
        perror("无法打开配置文件");
        exit(1);
    }
    fscanf(configFile, "timeout = %dms\n", &config.timeout);
    fscanf(configFile, "testsite = %s\n", config.testsite);
    fscanf(configFile, "proxytype = %s\n", config.proxytype);
    fclose(configFile);

    // 打开代理列表文件
    proxyListFile = fopen(PROXY_LIST_FILE, "r");
    if (proxyListFile == NULL) {
        perror("无法打开代理列表文件");
        exit(1);
    }

    // 逐行读取代理并检查
    while (fgets(proxy, 50, proxyListFile) != NULL) {
        // 构建ping命令
        snprintf(command, sizeof(command), "ping -n 1 -w %d %s", config.timeout, config.testsite);
        // 执行ping命令
        result = system(command);
        // 根据结果设置颜色
        if (result == 0) {
            setColor(2); // 绿色
        } else if (result == 1) {
            setColor(14); // 黄色
        } else if (result == 2) {
            setColor(6); // 橙色
        } else {
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
