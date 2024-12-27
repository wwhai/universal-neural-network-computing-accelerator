// Copyright (C) 2024 wwhai
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "heartbeat_timer.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static int heartbeat_timer_running = 0;
static pthread_t heartbeat_timer_thread;
static ClientInfo *clients;
static int client_count;

// 心跳计时器线程函数
void *heartbeat_timer_thread_func(void *arg)
{
    while (heartbeat_timer_running)
    {
        check_client_heartbeats(clients, client_count);
        sleep(HEARTBEAT_INTERVAL);
    }
    return NULL;
}

// 初始化客户端信息
void init_clients(ClientInfo **clients, int *client_count)
{
    *clients = (ClientInfo *)malloc(sizeof(ClientInfo) * MAX_CLIENTS);
    *client_count = 0;
}

// 添加客户端信息
void add_client(ClientInfo *clients, int *client_count, char *uuid)
{
    if (*client_count < MAX_CLIENTS)
    {
        clients[*client_count].uuid = strdup(uuid);
        clients[*client_count].last_heartbeat_time = time(NULL);
        (*client_count)++;
    }
    else
    {
        printf("Maximum number of clients reached\n");
    }
}

// 更新客户端心跳时间
void update_client_heartbeat(ClientInfo *clients, int client_count, char *uuid)
{
    for (int i = 0; i < client_count; i++)
    {
        if (strcmp(clients[i].uuid, uuid) == 0)
        {
            clients[i].last_heartbeat_time = time(NULL);
            break;
        }
    }
}

// 检查客户端心跳
void check_client_heartbeats(ClientInfo *clients, int client_count)
{
    time_t current_time = time(NULL);
    for (int i = 0; i < client_count; i++)
    {
        if (current_time - clients[i].last_heartbeat_time > HEARTBEAT_INTERVAL)
        {
            printf("Client %s is offline\n", clients[i].uuid);
            // 可以在这里处理离线客户端的逻辑
        }
    }
}

// 启动心跳计时器线程
void start_heartbeat_timer()
{
    init_clients(&clients, &client_count);
    if (!heartbeat_timer_running)
    {
        heartbeat_timer_running = 1;
        if (pthread_create(&heartbeat_timer_thread, NULL, heartbeat_timer_thread_func, NULL) != 0)
        {
            perror("Failed to start heartbeat timer thread");
            heartbeat_timer_running = 0;
        }
    }
}

// 停止心跳计时器线程
void stop_heartbeat_timer()
{
    if (heartbeat_timer_running)
    {
        heartbeat_timer_running = 0;
        if (pthread_join(heartbeat_timer_thread, NULL) != 0)
        {
            perror("Failed to stop heartbeat timer thread");
        }
    }
    stop_heartbeat_timer();
}
// int main() {
//     ClientInfo *clients;
//     int client_count;
//     // 初始化客户端信息
//     init_clients(&clients, &client_count);
//     // 添加客户端
//     add_client(clients, &client_count, "client1");
//     add_client(clients, &client_count, "client2");
//     // 启动心跳计时器线程
//     start_heartbeat_timer(clients, client_count);
//     // 模拟程序运行
//     for (int i = 0; i < 20; i++) {
//         // 假设在处理消息的过程中可以获取到客户端的 UUID
//         update_client_heartbeat(clients, client_count, "client1");
//         update_client_heartbeat(clients, client_count, "client2");
//         sleep(2);
//         printf("Heartbeat sent from client1 and client2\n");
//     }
//     // 停止心跳计时器线程
//     stop_heartbeat_timer();
//     // 释放客户端信息
//     for (int i = 0; i < client_count; i++) {
//         free(clients[i].uuid);
//     }
//     free(clients);
//     return 0;
// }