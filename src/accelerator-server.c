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
#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "clog.h"
#include "utils.h"
#include "api.pb-c.h"
#include "server.h"

#define WORKER_COUNT 4

// 工作线程函数
void *worker_routine(void *context)
{
    // 创建 DEALER socket，连接到 inproc 通道
    void *worker = zmq_socket(context, ZMQ_DEALER);
    zmq_connect(worker, "inproc://workers");

    while (1)
    {
        zmq_msg_t identity; // 用于保存客户端标识
        zmq_msg_init(&identity);
        zmq_msg_recv(&identity, worker, 0);
        log_debug("Received data from: %s", zmq_msg_data(&identity));
        zmq_msg_t received_msg; // 客户端请求数据
        zmq_msg_init(&received_msg);
        zmq_msg_recv(&received_msg, worker, 0);

        const uint8_t *data = (const uint8_t *)zmq_msg_data(&received_msg);
        size_t data_size = zmq_msg_size(&received_msg);
        dump_hex_string(data, data_size);

        // 解析 Protobuf 请求
        UNNCA__RpcRequest *request = unnca__rpc_request__unpack(NULL, data_size, data);
        if (!request)
        {
            perror("Failed to unpack request");
            zmq_msg_close(&identity);
            zmq_msg_close(&received_msg);
            continue;
        }

        // 处理请求
        switch (request->request_case)
        {
        case UNNCA__RPC_REQUEST__REQUEST_AUTH_REQUEST:
            handle_auth_request(worker, identity, request->auth_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_PING_REQUEST:
            handle_ping_request(worker, identity, request->ping_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_DETECT_REQUEST:
            handle_detect_request(worker, identity, request->detect_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_ACCELERATOR_INFO_REQUEST:
            handle_getinfo_request(worker, identity, request->accelerator_info_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST__NOT_SET:
            report_error(worker, identity, "UNNCA__RPC_REQUEST__REQUEST__NOT_SET");
            break;
        default:
            printf("Unknown request received: %d\n", request->request_case);
            report_error(worker, identity, "UNNCA__RPC_REQUEST__UNKNOWN_REQUEST");
            break;
        }

        unnca__rpc_request__free_unpacked(request, NULL);
        zmq_msg_close(&identity);
        zmq_msg_close(&received_msg);
    }

    zmq_close(worker);
    return NULL;
}

int main()
{
    void *context = zmq_ctx_new();
    void *frontend = zmq_socket(context, ZMQ_ROUTER);
    zmq_bind(frontend, "tcp://*:5555");
    void *backend = zmq_socket(context, ZMQ_DEALER);
    zmq_bind(backend, "inproc://workers");
    pthread_t workers[WORKER_COUNT];
    for (int i = 0; i < WORKER_COUNT; i++)
    {
        pthread_create(&workers[i], NULL, worker_routine, context);
    }
    zmq_proxy(frontend, backend, NULL);
    zmq_close(frontend);
    zmq_close(backend);
    zmq_ctx_destroy(context);
    return 0;
}
