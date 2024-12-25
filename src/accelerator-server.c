#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clog.h"
#include "utils.h"
#include "api.pb-c.h"

void handle_getinfo_request(void *socket, const UNNCA__AcceleratorInfoRequest *request)
{
    UNNCA__AcceleratorInfoResponse accelerator_info_response = UNNCA__ACCELERATOR_INFO_RESPONSE__INIT;
    accelerator_info_response.uuid = strdup("1000000000000000");
    accelerator_info_response.name = strdup("neural-network-computing-accelerator");
    accelerator_info_response.version = strdup("v0.0.1");
    accelerator_info_response.vendor = strdup("www.hootrhino.com");
    accelerator_info_response.model = strdup("UNNCA-RK3568");
    UNNCA__RpcResponse rpc_response = UNNCA__RPC_RESPONSE__INIT;
    rpc_response.accelerator_info_response = &accelerator_info_response;
    rpc_response.response_case = UNNCA__RPC_RESPONSE__RESPONSE_ACCELERATOR_INFO_RESPONSE;
    size_t rpc_response_size = unnca__rpc_response__get_packed_size(&rpc_response);
    uint8_t *response_buffer = (uint8_t *)malloc(rpc_response_size);
    unnca__rpc_response__pack(&rpc_response, response_buffer);
    zmq_send(socket, response_buffer, rpc_response_size, 0);
    free(response_buffer);
    free(accelerator_info_response.version);
    free(accelerator_info_response.vendor);
    free(accelerator_info_response.model);
    free(accelerator_info_response.name);
    free(accelerator_info_response.uuid);
}
void handle_ping_request(void *socket, const UNNCA__PingRequest *request)
{
    UNNCA__PingResponse ping_response = UNNCA__PING_RESPONSE__INIT;
    ping_response.status = 0;
    UNNCA__RpcResponse rpc_response = UNNCA__RPC_RESPONSE__INIT;
    rpc_response.ping_response = &ping_response;
    rpc_response.response_case = UNNCA__RPC_RESPONSE__RESPONSE_PING_RESPONSE;
    size_t rpc_response_size = unnca__rpc_response__get_packed_size(&rpc_response);
    uint8_t *response_buffer = (uint8_t *)malloc(rpc_response_size);
    unnca__rpc_response__pack(&rpc_response, response_buffer);
    zmq_send(socket, response_buffer, rpc_response_size, 0);
    free(response_buffer);
}
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
void handle_detect_request(void *socket, const UNNCA__DetectRequest *request)
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
        const uint8_t *data = (const uint8_t *)zmq_msg_data(&received_msg);
        dump_hex_string(data, bytes_received);
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
            handle_ping_request(socket, request->ping_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_DETECT_REQUEST:
            printf("Detect request received\n");
            handle_detect_request(socket, request->detect_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_ACCELERATOR_INFO_REQUEST:
            printf("AcceleratorInfo request received\n");
            handle_getinfo_request(socket, request->accelerator_info_request);
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
// 单独起来一个检查线程检查客户端的心跳状态:每次有AUTH数据包进来以后，通过校验，将当前这个UUID记录下来，每次心跳刷新倒计时。当倒计时==0的时候标记某个客户端离线。
void *check_client_heartbeat(void *arg)
{
    while (1)
    {


    }

}