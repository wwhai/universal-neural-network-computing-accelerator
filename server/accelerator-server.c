#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clog.h"
#include "api.pb-c.h"

// 模拟服务端逻辑：处理 Request 并返回 Response
UNNCA__Response *process_request(const UNNCA__Request *request)
{
    UNNCA__Response *response = (UNNCA__Response *)malloc(sizeof(UNNCA__Response));
    unnca__response__init(response);

    // 假设检测到 2 个框
    response->box_count = 2;
    response->n_boxes = 2;
    response->boxes = (UNNCA__Box **)malloc(sizeof(UNNCA__Box *) * response->n_boxes);

    for (int i = 0; i < response->n_boxes; i++)
    {
        response->boxes[i] = (UNNCA__Box *)malloc(sizeof(UNNCA__Box));
        unnca__box__init(response->boxes[i]);
        response->boxes[i]->x = i * 10;
        response->boxes[i]->y = i * 20;
        response->boxes[i]->width = 50 + i * 10;
        response->boxes[i]->height = 60 + i * 10;
        response->boxes[i]->confidence = 0.9f - i * 0.1f;
        response->boxes[i]->label = strdup(i == 0 ? "Car" : "Person");
    }

    return response;
}

int main()
{
    // 初始化 ZMQ 上下文和服务端 socket
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    zmq_bind(socket, "tcp://*:5555");

    log_info("Server is running at tcp://*:5555");

    while (1)
    {
        // 接收客户端请求
        uint8_t buffer[1024];
        int bytes_received = zmq_recv(socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0)
        {
            perror("zmq_recv failed");
            break;
        }

        // 解析 Request
        UNNCA__Request *request = unnca__request__unpack(NULL, bytes_received, buffer);
        if (!request)
        {
            fprintf(stderr, "Failed to unpack request");
            continue;
        }

        log_info("Received Request: width=%d, height=%d, data_len=%zu",
                 request->width, request->height, request->data.len);

        // 调用服务逻辑
        UNNCA__Response *response = process_request(request);

        // 序列化 Response
        size_t response_size = unnca__response__get_packed_size(response);
        uint8_t *response_buffer = (uint8_t *)malloc(response_size);
        unnca__response__pack(response, response_buffer);

        // 发送响应
        zmq_send(socket, response_buffer, response_size, 0);

        // 释放资源
        unnca__request__free_unpacked(request, NULL);
        free(response_buffer);
        for (size_t i = 0; i < response->n_boxes; i++)
        {
            free(response->boxes[i]->label);
            free(response->boxes[i]);
        }
        free(response->boxes);
        free(response);
    }

    // 关闭 ZMQ socket 和上下文
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}
