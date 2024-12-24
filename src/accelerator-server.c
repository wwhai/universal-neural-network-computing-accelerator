#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clog.h"
#include "api.pb-c.h"
// 处理认证请求的函数
void handle_auth_request(void *socket, const UNNCA__AuthRequest *request)
{
    UNNCA__AuthResponse auth_response = UNNCA__AUTH_RESPONSE__INIT;
    auth_response.code = 0;
    auth_response.msg = strdup("AUTH_SUCCESS");
    UNNCA__RpcResponse rpc_response = UNNCA__RPC_RESPONSE__INIT;
    rpc_response.auth_response = &auth_response;
    rpc_response.response_case = UNNCA__RPC_RESPONSE__RESPONSE_AUTH_RESPONSE;
    size_t rpc_response_size = unnca__rpc_response__get_packed_size(&rpc_response);
    uint8_t *response_buffer = (uint8_t *)malloc(rpc_response_size);
    unnca__rpc_response__pack(&rpc_response, response_buffer);
    zmq_send(socket, response_buffer, rpc_response_size, 0);
    free(response_buffer);
}
// 模拟服务端逻辑：处理 Request 并返回 Response
void process_request(void *socket, const UNNCA__DetectRequest *request)
{
    UNNCA__DetectResponse detect_response = UNNCA__DETECT_RESPONSE__INIT;
    unnca__detect_response__init(&detect_response);

    // 假设检测到 2 个框
    detect_response.box_count = 2;
    detect_response.n_boxes = 2;
    detect_response.boxes = (UNNCA__Box **)malloc(sizeof(UNNCA__Box *) * detect_response.n_boxes);

    for (int i = 0; i < detect_response.n_boxes; i++)
    {
        detect_response.boxes[i] = (UNNCA__Box *)malloc(sizeof(UNNCA__Box));
        unnca__box__init(detect_response.boxes[i]);
        detect_response.boxes[i]->x = i * 10;
        detect_response.boxes[i]->y = i * 20;
        detect_response.boxes[i]->width = 50 + i * 10;
        detect_response.boxes[i]->height = 60 + i * 10;
        detect_response.boxes[i]->confidence = 0.9f - i * 0.1f;
        detect_response.boxes[i]->label = strdup(i == 0 ? "Car" : "Person");
    }
    UNNCA__RpcResponse rpc_response = UNNCA__RPC_RESPONSE__INIT;
    rpc_response.detect_response = &detect_response;
    rpc_response.response_case = UNNCA__RPC_RESPONSE__RESPONSE_DETECT_RESPONSE;
    size_t rpc_response_size = unnca__rpc_response__get_packed_size(&rpc_response);
    uint8_t *response_buffer = (uint8_t *)malloc(rpc_response_size);
    unnca__rpc_response__pack(&rpc_response, response_buffer);
    zmq_send(socket, response_buffer, rpc_response_size, 0);
    // 释放资源
    for (int i = 0; i < detect_response.n_boxes; i++)
    {
        free(detect_response.boxes[i]->label);
        free(detect_response.boxes[i]);
    }
    free(detect_response.boxes);
    free(response_buffer);
}

int main()
{
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    zmq_bind(socket, "tcp://*:5555");

    log_info("Server is running at tcp://*:5555");

    while (1)
    {
        zmq_msg_t received_msg;
        zmq_msg_init(&received_msg);
        int bytes_received = zmq_msg_recv(&received_msg, socket, 0);
        if (bytes_received < 0)
        {
            perror("zmq_recv failed");
            zmq_msg_close(&received_msg);
            break;
        }
        fprintf(stderr, "Received %d bytes\n", bytes_received);
        const uint8_t *data = (const uint8_t *)zmq_msg_data(&received_msg);
        fprintf(stderr, "Received data: ");
        for (int i = 0; i < bytes_received; i++)
        {
            fprintf(stderr, "%02x ", data[i]);
        }
        fprintf(stderr, "\n");
        UNNCA__RpcRequest *request = unnca__rpc_request__unpack(NULL, bytes_received, data);
        if (!request)
        {
            fprintf(stderr, "Failed to unpack request");
            continue;
        }
        switch (request->request_case)
        {
        case UNNCA__RPC_REQUEST__REQUEST_AUTH_REQUEST:
            printf("Auth request: uuid=%s\n", request->auth_request->uuid);
            handle_auth_request(socket, request->auth_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_PING_REQUEST:
            printf("Ping request received\n");
            break;
        case UNNCA__RPC_REQUEST__REQUEST_DETECT_REQUEST:
            printf("Detect request received\n");
            break;
        case UNNCA__RPC_REQUEST__REQUEST_ACCELERATOR_INFO_REQUEST:
            printf("AcceleratorInfo request received\n");
            break;
        case UNNCA__RPC_REQUEST__REQUEST__NOT_SET:
            printf("Request not set\n");
            break;
        default:
            printf("Unknown request received, request->request_case: %d\n", request->request_case);
            break;
        }
        zmq_msg_close(&received_msg);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}
