#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.pb-c.h"

int main()
{
    // 初始化 ZMQ 上下文和客户端 socket
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");
    int timeout = 1000; // 毫秒
    zmq_setsockopt(socket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    printf("Client connected to tcp://localhost:5555\n");

    // 创建并初始化 Request 消息
    UNNCA__DetectRequest request = UNNCA__DETECT_REQUEST__INIT;
    request.width = 640;
    request.height = 480;
    const char *image_data = "example image data";
    request.data.data = (uint8_t *)image_data;
    request.data.len = strlen(image_data);

    // 序列化 Request
    size_t request_size = unnca__detect_request__get_packed_size(&request);
    uint8_t *request_buffer = (uint8_t *)malloc(request_size);
    unnca__detect_request__pack(&request, request_buffer);

    // 发送请求
    zmq_send(socket, request_buffer, request_size, 0);
    printf("Sent Request: width=%d, height=%d, data_len=%zu\n",
           request.width, request.height, request.data.len);

    // 接收响应
    uint8_t response_buffer[1024];
    int bytes_received = zmq_recv(socket, response_buffer, sizeof(response_buffer), 0);
    if (bytes_received < 0)
    {
        perror("zmq_recv failed");
        free(request_buffer);
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return 1;
    }

    // 解析 Response
    UNNCA__DetectResponse *response = unnca__detect_response__unpack(NULL, bytes_received, response_buffer);
    if (!response)
    {
        fprintf(stderr, "Failed to unpack response\n");
        free(request_buffer);
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return 1;
    }

    printf("Received Response: box_count=%d\n", response->box_count);
    for (size_t i = 0; i < response->n_boxes; i++)
    {
        UNNCA__Box *box = response->boxes[i];
        printf("  Box %zu: x=%d, y=%d, width=%d, height=%d, confidence=%.2f, label=%s\n",
               i, box->x, box->y, box->width, box->height, box->confidence, box->label);
    }

    // 释放资源
    unnca__detect_response__free_unpacked(response, NULL);
    free(request_buffer);

    // 关闭 ZMQ socket 和上下文
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}
