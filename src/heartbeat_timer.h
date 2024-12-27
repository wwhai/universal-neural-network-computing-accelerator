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

#ifndef HEARTBEAT_TIMER_H
#define HEARTBEAT_TIMER_H

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> // 新增：添加线程相关头文件

#define HEARTBEAT_INTERVAL 5 // 心跳间隔，单位：秒
#define MAX_CLIENTS 10       // 最大客户端数量

// 定义一个结构体来存储客户端信息
typedef struct
{
    char *uuid;
    time_t last_heartbeat_time;
} ClientInfo;

// 初始化客户端信息
void init_clients(ClientInfo **clients, int *client_count);
// 添加客户端信息
void add_client(ClientInfo *clients, int *client_count, char *uuid);
// 更新客户端心跳时间
void update_client_heartbeat(ClientInfo *clients, int client_count, char *uuid);
// 检查客户端心跳
void check_client_heartbeats(ClientInfo *clients, int client_count);
// 启动心跳计时器线程
void start_heartbeat_timer();
// 停止心跳计时器线程
void stop_heartbeat_timer();

#endif // HEARTBEAT_TIMER_H