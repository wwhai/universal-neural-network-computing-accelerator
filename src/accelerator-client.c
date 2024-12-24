#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.pb-c.h"

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
void send_detect_request(void *socket, const char *uuid)
{

    UNNCA__DetectRequest request = UNNCA__DETECT_REQUEST__INIT;
    request.width = 1920;
    request.height = 1080;
    request.data.data = (uint8_t *)"1234567890";
    UNNCA__RpcRequest rpc_request = UNNCA__RPC_REQUEST__INIT;
    rpc_request.detect_request = &request;
    rpc_request.request_case = UNNCA__RPC_REQUEST__REQUEST_DETECT_REQUEST;
    size_t rpc_request_size = unnca__rpc_request__get_packed_size(&rpc_request);
    uint8_t *request_buffer = (uint8_t *)malloc(rpc_request_size);
    unnca__rpc_request__pack(&rpc_request, request_buffer);
    zmq_send(socket, request_buffer, rpc_request_size, 0);
    printf("zmq_send bytes: ");
    for (size_t i = 0; i < rpc_request_size; i++)
    {
        printf("%02x ", request_buffer[i]);
    }
    printf("\n");
    free(request_buffer);
}
// 发送 Auth 请求
void send_auth_request(void *socket, const char *uuid)
{
    static uint8_t request_buffer[1024]; // 假设最大请求大小为1024字节
    static size_t rpc_request_size = 0;

    UNNCA__AuthRequest auth_request = UNNCA__AUTH_REQUEST__INIT;
    auth_request.uuid = (char *)uuid;
    UNNCA__RpcRequest rpc_request = UNNCA__RPC_REQUEST__INIT;
    rpc_request.auth_request = &auth_request;
    rpc_request.request_case = UNNCA__RPC_REQUEST__REQUEST_AUTH_REQUEST;

    rpc_request_size = unnca__rpc_request__get_packed_size(&rpc_request);
    if (rpc_request_size > sizeof(request_buffer))
    {
        fprintf(stderr, "Error: RPC request size exceeds buffer size\n");
        return;
    }

    unnca__rpc_request__pack(&rpc_request, request_buffer);
    zmq_send(socket, request_buffer, rpc_request_size, 0);
    printf("zmq_send bytes: ");
    for (size_t i = 0; i < rpc_request_size; i++)
    {
        printf("%02x ", request_buffer[i]);
    }
    printf("\n");
}

// 发送 Detect 请求
void send_detect_request(void *socket, int width, int height, const uint8_t *data, size_t data_len)
{
    UNNCA__DetectRequest request = UNNCA__DETECT_REQUEST__INIT;
    request.width = width;
    request.height = height;
    request.data.data = (uint8_t *)data;
    request.data.len = data_len;
    size_t request_size = unnca__detect_request__get_packed_size(&request);
    uint8_t *request_buffer = (uint8_t *)malloc(request_size);
    unnca__detect_request__pack(&request, request_buffer);
    zmq_send(socket, request_buffer, request_size, 0);
    free(request_buffer);
}

// 发送 Ping 请求
void send_ping_request(void *socket)
{
    UNNCA__PingRequest request = UNNCA__PING_REQUEST__INIT;
    size_t request_size = unnca__ping_request__get_packed_size(&request);
    uint8_t *request_buffer = (uint8_t *)malloc(request_size);
    unnca__ping_request__pack(&request, request_buffer);
    zmq_send(socket, request_buffer, request_size, 0);
    free(request_buffer);
}

// 发送 AcceleratorInfo 请求
void send_accelerator_info_request(void *socket)
{
    UNNCA__AcceleratorInfoRequest request = UNNCA__ACCELERATOR_INFO_REQUEST__INIT;
    size_t request_size = unnca__accelerator_info_request__get_packed_size(&request);
    uint8_t *request_buffer = (uint8_t *)malloc(request_size);
    unnca__accelerator_info_request__pack(&request, request_buffer);
    zmq_send(socket, request_buffer, request_size, 0);
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
        printf("AcceleratorInfo Response: name=%s, version=%s\n", response->name, response->version);
    }
}

int main()
{
    void *socket = init_client();
    send_auth_request(socket, "1234567890AAAAAA");
    UNNCA__RpcResponse *response = receive_response(socket);
    if (response)
    {

        switch (response->response_case)
        {
        case UNNCA__RPC_RESPONSE__RESPONSE_AUTH_RESPONSE:
            parse_auth_response(response->auth_response);
            free_response(response);
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE_PING_RESPONSE:
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE_DETECT_RESPONSE:
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE_ACCELERATOR_INFO_RESPONSE:
            break;
        case UNNCA__RPC_RESPONSE__RESPONSE__NOT_SET:
            break;
        default:
            printf("Unknown request received, request->request_case: %d\n", response->response_case);
            break;
        }
    }
    zmq_close(socket);
    zmq_ctx_destroy(socket);

    return 0;
}