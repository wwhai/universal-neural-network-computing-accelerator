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

#ifndef CLIENT_H
#define CLIENT_H

#include <zmq.h>
#include <pthread.h>
#include "api.pb-c.h"

#define HEARTBEAT_INTERVAL 5000 // 心跳间隔，单位：毫秒
#define HEARTBEAT_TIMEOUT 10000 // 心跳超时时间，单位：毫秒

typedef enum
{
    UNAUTHORIZED,
    AUTH_SUCCESS,
    AUTH_FAILED,
    CONNECTED,
    DISCONNECTED,
    ERROR,
    EXIT,
} Status;

typedef struct
{
    void *zmq_context;
    void *zmq_socket;
    char uuid[32];
    uint32_t timeout;
    pthread_t heartbeat_thread;
    int heartbeat_running;
} UNNCAClient;

UNNCAClient *init_client(const char *endpoint, const char *identity);
void *heartbeat_thread_func(void *arg);
void start_heartbeat_thread(UNNCAClient *client);
void send_detect_request(void *socket);
void send_auth_request(void *socket, const char *uuid);
void send_ping_request(void *socket);
void send_accelerator_info_request(void *socket);
UNNCA__RpcResponse *receive_response(void *socket);
void free_response(UNNCA__RpcResponse *response);
void parse_auth_response(UNNCA__AuthResponse *response);
void parse_detect_response(UNNCA__DetectResponse *response);
void parse_ping_response(UNNCA__PingResponse *response);
void parse_accelerator_info_response(UNNCA__AcceleratorInfoResponse *response);
void process_response(UNNCA__RpcResponse *response);

#endif // CLIENT_H