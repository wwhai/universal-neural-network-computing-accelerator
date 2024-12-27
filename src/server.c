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

#include "server.h"
#include "config.h"
void start_server(void *context, void *socket)
{
    zmq_bind(socket, "tcp://*:5555");
    log_info("Server is running at tcp://*:5555");
}

void stop_server(void *context, void *socket)
{
    zmq_close(socket);
    zmq_ctx_destroy(context);
}
void handle_getinfo_request(void *socket, const UNNCA__AcceleratorInfoRequest *request)
{
    UNNCA__AcceleratorInfoResponse accelerator_info_response = UNNCA__ACCELERATOR_INFO_RESPONSE__INIT;
    accelerator_info_response.uuid = UUID;
    accelerator_info_response.name = NAME;
    accelerator_info_response.version = VERSION;
    accelerator_info_response.vendor = VENDOR;
    accelerator_info_response.model = MODEL;
    UNNCA__RpcResponse rpc_response = UNNCA__RPC_RESPONSE__INIT;
    rpc_response.accelerator_info_response = &accelerator_info_response;
    rpc_response.response_case = UNNCA__RPC_RESPONSE__RESPONSE_ACCELERATOR_INFO_RESPONSE;
    size_t rpc_response_size = unnca__rpc_response__get_packed_size(&rpc_response);
    uint8_t *response_buffer = (uint8_t *)malloc(rpc_response_size);
    unnca__rpc_response__pack(&rpc_response, response_buffer);
    zmq_send(socket, response_buffer, rpc_response_size, 0);
    free(response_buffer);
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
    auth_response.msg = "AUTH_SUCCESS";
    UNNCA__RpcResponse rpc_response = UNNCA__RPC_RESPONSE__INIT;
    rpc_response.auth_response = &auth_response;
    rpc_response.response_case = UNNCA__RPC_RESPONSE__RESPONSE_AUTH_RESPONSE;
    size_t rpc_response_size = unnca__rpc_response__get_packed_size(&rpc_response);
    uint8_t *response_buffer = (uint8_t *)malloc(rpc_response_size);
    unnca__rpc_response__pack(&rpc_response, response_buffer);
    zmq_send(socket, response_buffer, rpc_response_size, 0);
    free(response_buffer);
}

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
// 实现错误报告函数
void report_error(void *socket, const char *error_msg)
{
    UNNCA__ErrorResponse error_response = UNNCA__ERROR_RESPONSE__INIT;
    error_response.code = 0;
    error_response.msg = (char *)error_msg;
    UNNCA__RpcResponse rpc_response = UNNCA__RPC_RESPONSE__INIT;
    rpc_response.error_response = &error_response;
    rpc_response.response_case = UNNCA__RPC_RESPONSE__RESPONSE_ERROR_RESPONSE;
    size_t rpc_response_size = unnca__rpc_response__get_packed_size(&rpc_response);
    uint8_t *response_buffer = (uint8_t *)malloc(rpc_response_size);
    unnca__rpc_response__pack(&rpc_response, response_buffer);
    zmq_send(socket, response_buffer, rpc_response_size, 0);
    free(response_buffer);
}