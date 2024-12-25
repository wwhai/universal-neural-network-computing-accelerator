#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
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
    char uuid[40];
    uint32_t timeout;
    pthread_t heartbeat_thread;
    int heartbeat_running;
} Client;
void *heartbeat_thread_func(void *arg)
{
    Client *client = (Client *)arg;
    while (client->heartbeat_running)
    {
        zmq_send(client->zmq_socket, "PING", 4, 0);
        zmq_pollitem_t items[] = {
            {client->zmq_socket, 0, ZMQ_POLLIN, 0}};
        int rc = zmq_poll(items, 1, client->timeout);
        if (rc == -1)
        {
            // 错误处理
            break;
        }
        else if (rc == 0)
        {
            printf("Heartbeat timeout, server may be offline\n");
            break;
        }
        else
        {
            char buffer[4];
            zmq_recv(client->zmq_socket, buffer, 4, 0);
            if (strcmp(buffer, "PONG") == 0)
            {
                printf("Received PONG from server\n");
            }
            else
            {
                printf("Received unknown message: %s\n", buffer);
            }
        }
        usleep(HEARTBEAT_INTERVAL * 1000);
    }
    client->heartbeat_running = 0;
    return NULL;
}
void start_heartbeat_thread(Client *client)
{
    client->heartbeat_running = 1;
    pthread_create(&client->heartbeat_thread, NULL, heartbeat_thread_func, client);
}
// 初始化 ZMQ 上下文和客户端 socket
void *init_client(void)
{
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");
    int timeout = 1000; // 毫秒
    zmq_setsockopt(socket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    printf("Client connected to tcp://localhost:5555\n");
    return socket;
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
            printf("  Box %zu: x=%d, y=%d, width=%d, height=%d, confidence=%.2f, label=%s\n",
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
        printf("AcceleratorInfo Response: uuid=%s, vendor=%s, model=%s, name=%s, version=%s\n",
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
        send_ping_request(socket);
        UNNCA__RpcResponse *response = receive_response(socket);
        process_response(response);
        sleep(3);
    }
    return NULL;
}
int main()
{
    void *socket = init_client();
    {
        send_auth_request(socket, "1234567890AAAAAA");
        UNNCA__RpcResponse *response = receive_response(socket);
        process_response(response);
    }
    {
        send_ping_request(socket);
        UNNCA__RpcResponse *response = receive_response(socket);
        process_response(response);
    }
    {
        send_accelerator_info_request(socket);
        UNNCA__RpcResponse *response = receive_response(socket);
        process_response(response);
    }
    {
        send_detect_request(socket);
        UNNCA__RpcResponse *response = receive_response(socket);
        process_response(response);
    }
    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, send_ping_thread, socket);
    pthread_detach(ping_thread);
    sleep(20);
    zmq_close(socket);
    zmq_ctx_destroy(socket);

    return 0;
}