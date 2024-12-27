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

#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
#include "clog.h"
void *heartbeat_thread_func(void *arg)
{
    UNNCAClient *client = (UNNCAClient *)arg;
    while (client->heartbeat_running)
    {
        // 当Timeout时间内没有收到心跳响应，则认为连接已断开
        if (client->heartbeat_running == 0)
        {
            log_error("Connection timed out.");
            break;
        }
        send_ping_request(client->zmq_socket);
        usleep(HEARTBEAT_INTERVAL * 1000);
    }
    client->heartbeat_running = 0;
    return NULL;
}

void start_heartbeat_thread(UNNCAClient *client)
{
    client->heartbeat_running = 1;
    pthread_create(&client->heartbeat_thread, NULL, heartbeat_thread_func, client);
}

// 初始化 ZMQ 上下文和客户端 socket
UNNCAClient *init_client(const char *endpoint, const char *identity)
{
    UNNCAClient *client = (UNNCAClient *)malloc(sizeof(UNNCAClient));
    client->zmq_context = zmq_ctx_new();
    client->zmq_socket = zmq_socket(client->zmq_context, ZMQ_DEALER);
    memcpy(client->uuid, identity, strlen(identity));
    client->timeout = 5000;
    client->heartbeat_running = 0;
    client->heartbeat_thread = 0;
    zmq_setsockopt(client->zmq_socket, ZMQ_IDENTITY, identity, strlen(identity));
    zmq_connect(client->zmq_socket, endpoint);
    zmq_connect(client->zmq_socket, endpoint);
    log_info("Client connected to %s with identity: %s", endpoint, identity);
    return client;
}

void send_detect_request(void *socket)
{
    UNNCA__DetectRequest detect_request = UNNCA__DETECT_REQUEST__INIT;
    detect_request.width = 1920;
    detect_request.height = 1080;
    detect_request.data.data = (uint8_t *)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    UNNCA__RpcRequest rpc_request = UNNCA__RPC_REQUEST__INIT;
    rpc_request.detect_request = &detect_request;
    rpc_request.request_case = UNNCA__RPC_REQUEST__REQUEST_DETECT_REQUEST;
    size_t rpc_request_size = unnca__rpc_request__get_packed_size(&rpc_request);
    uint8_t *request_buffer = (uint8_t *)malloc(rpc_request_size);
    unnca__rpc_request__pack(&rpc_request, request_buffer);
    dump_hex_string((const uint8_t *)request_buffer, rpc_request_size);
    zmq_send(socket, request_buffer, rpc_request_size, 0);
    free(request_buffer);
}

// 发送 Auth 请求
void send_auth_request(void *socket, const char *uuid)
{
    static size_t rpc_request_size = 0;
    UNNCA__AuthRequest auth_request = UNNCA__AUTH_REQUEST__INIT;
    auth_request.uuid = (char *)uuid;
    UNNCA__RpcRequest rpc_request = UNNCA__RPC_REQUEST__INIT;
    rpc_request.auth_request = &auth_request;
    rpc_request.request_case = UNNCA__RPC_REQUEST__REQUEST_AUTH_REQUEST;
    rpc_request_size = unnca__rpc_request__get_packed_size(&rpc_request);
    uint8_t *request_buffer = (uint8_t *)malloc(rpc_request_size);
    unnca__rpc_request__pack(&rpc_request, request_buffer);
    dump_hex_string((const uint8_t *)request_buffer, rpc_request_size);
    zmq_send(socket, request_buffer, rpc_request_size, 0);
    free(request_buffer);
}

// 发送 Ping 请求
void send_ping_request(void *socket)
{
    static size_t rpc_request_size = 0;
    UNNCA__PingRequest ping_request = UNNCA__PING_REQUEST__INIT;
    UNNCA__RpcRequest rpc_request = UNNCA__RPC_REQUEST__INIT;
    rpc_request.ping_request = &ping_request;
    rpc_request.request_case = UNNCA__RPC_REQUEST__REQUEST_PING_REQUEST;
    rpc_request_size = unnca__rpc_request__get_packed_size(&rpc_request);
    uint8_t *request_buffer = (uint8_t *)malloc(rpc_request_size);
    unnca__rpc_request__pack(&rpc_request, request_buffer);
    dump_hex_string((const uint8_t *)request_buffer, rpc_request_size);
    zmq_send(socket, request_buffer, rpc_request_size, 0);
    free(request_buffer);
}

// 发送 AcceleratorInfo 请求
void send_accelerator_info_request(void *socket)
{
    static size_t rpc_request_size = 0;
    UNNCA__AcceleratorInfoRequest accelerator_info_request = UNNCA__ACCELERATOR_INFO_REQUEST__INIT;
    UNNCA__RpcRequest rpc_request = UNNCA__RPC_REQUEST__INIT;
    rpc_request.accelerator_info_request = &accelerator_info_request;
    rpc_request.request_case = UNNCA__RPC_REQUEST__REQUEST_ACCELERATOR_INFO_REQUEST;
    rpc_request_size = unnca__rpc_request__get_packed_size(&rpc_request);
    uint8_t *request_buffer = (uint8_t *)malloc(rpc_request_size);
    unnca__rpc_request__pack(&rpc_request, request_buffer);
    dump_hex_string((const uint8_t *)request_buffer, rpc_request_size);
    zmq_send(socket, request_buffer, rpc_request_size, 0);
    free(request_buffer);
}

// 接收响应
UNNCA__RpcResponse *receive_response(void *socket)
{
    uint8_t response_buffer[1024];
    int bytes_received = zmq_recv(socket, response_buffer, sizeof(response_buffer), 0);
    if (bytes_received < 0)
    {
        perror("zmq_recv failed");
        return NULL;
    }
    dump_hex_string(response_buffer, bytes_received);
    return unnca__rpc_response__unpack(NULL, bytes_received, response_buffer);
}

// 释放响应
void free_response(UNNCA__RpcResponse *response)
{
    if (response)
    {
        unnca__rpc_response__free_unpacked(response, NULL);
    }
}

// 解析 Auth 响应
void parse_auth_response(UNNCA__AuthResponse *response)
{
    if (response)
    {
        printf("Auth Response: success=%d, msg=%s\n", response->code, response->msg);
    }
}

// 解析 Detect 响应
void parse_detect_response(UNNCA__DetectResponse *response)
{
    if (response)
    {
        printf("Detect Response: box_count=%d\n", response->box_count);
        for (size_t i = 0; i < response->n_boxes; i++)
        {
            UNNCA__Box *box = response->boxes[i];
            printf("========== Box %zu:\n·-> x=%d\n·-> y=%d\n·-> width=%d\n·-> height=%d\n·-> confidence=%.2f\n·-> label=%s\n",
                   i, box->x, box->y, box->width, box->height, box->confidence, box->label);
        }
    }
}

// 解析 Ping 响应
void parse_ping_response(UNNCA__PingResponse *response)
{
    if (response)
    {

        printf("Ping Response: timestamp=%d\n", response->status);
    }
}

// 解析 AcceleratorInfo 响应
void parse_accelerator_info_response(UNNCA__AcceleratorInfoResponse *response)
{
    if (response)
    {
        printf("========== Accelerator Info Response:\n·-> uuid=%s\n·-> vendor=%s\n·-> model=%s\n·-> name=%s\n·-> version=%s\n",
               response->uuid, response->vendor, response->model, response->name, response->version);
    }
}

void process_response(UNNCA__RpcResponse *response)
{
    if (response)
    {
        switch (response->response_case)
        {
        case UNNCA__RPC_RESPONSE__RESPONSE_AUTH_RESPONSE:
            parse_auth_response(response->auth_response);
            free_response(response);
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE_PING_RESPONSE:
            parse_ping_response(response->ping_response);
            free_response(response);
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE_DETECT_RESPONSE:
            parse_detect_response(response->detect_response);
            free_response(response);
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE_ACCELERATOR_INFO_RESPONSE:
            parse_accelerator_info_response(response->accelerator_info_response);
            free_response(response);
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE__NOT_SET:
            printf("Response not set\n");
            free_response(response);
            break;
        default:
            printf("Unknown request received, request->request_case: %d\n", response->response_case);
            free_response(response);
            break;
        }
    }
}

// 启动一个线程，每隔3秒发送一次ping
void *send_ping_thread(void *arg)
{
    void *socket = arg;
    while (1)
    {
        printf("Send Ping\n");
        send_ping_request(socket);
        UNNCA__RpcResponse *response = receive_response(socket);
        process_response(response);
        sleep(2);
    }
    return NULL;
}
