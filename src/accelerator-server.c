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
#include "threadpool.h"
#include "heartbeat_timer.h"

int main()
{
    ThreadPool pool;
    threadpool_init(&pool);
    ClientInfo *clients;
    int client_count;
    init_clients(&clients, &client_count);
    start_heartbeat_timer();

    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    start_server(context, socket);

    zmq_pollitem_t items[1];
    items[0].socket = socket;
    items[0].events = ZMQ_POLLIN;
    items[0].revents = 0;

    while (1)
    {
        int rc = zmq_poll(items, 1, -1);
        if (rc == -1)
        {
            perror("zmq_poll failed");
            break;
        }

        if (items[0].revents & ZMQ_POLLIN)
        {
            zmq_msg_t received_msg;
            zmq_msg_init(&received_msg);
            int bytes_received = zmq_msg_recv(&received_msg, socket, 0);
            if (bytes_received < 0)
            {
                perror("zmq_recv failed");
                zmq_msg_close(&received_msg);
                continue;
            }
            const uint8_t *data = (const uint8_t *)zmq_msg_data(&received_msg);
            dump_hex_string(data, bytes_received);
            UNNCA__RpcRequest *request = unnca__rpc_request__unpack(NULL, bytes_received, data);
            if (!request)
            {
                perror("Failed to unpack request");
                zmq_msg_close(&received_msg);
                continue;
            }
            threadpool_add_task(&pool, socket, request);
            zmq_msg_close(&received_msg);
        }
    }

    stop_server(context, socket);
    threadpool_destroy(&pool);
    stop_heartbeat_timer();
    zmq_ctx_destroy(context);
    return 0;
}