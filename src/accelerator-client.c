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

int main()
{
    UNNCAClient *client = init_client("tcp://127.0.0.1:5555", "client1");
    for (size_t i = 0; i < 1000000; i++)
    {

        {
            printf("Send Auth\n");
            send_auth_request(client->zmq_socket, "1234567890AAAAAA");
            UNNCA__RpcResponse *response = receive_response(client->zmq_socket);
            process_response(response);
        }
        {
            printf("Send Ping\n");
            send_ping_request(client->zmq_socket);
            UNNCA__RpcResponse *response = receive_response(client->zmq_socket);
            process_response(response);
        }
        {
            printf("Send AcceleratorInfo\n");
            send_accelerator_info_request(client->zmq_socket);
            UNNCA__RpcResponse *response = receive_response(client->zmq_socket);
            process_response(response);
        }
        {
            printf("Send Detect\n");
            send_detect_request(client->zmq_socket);
            UNNCA__RpcResponse *response = receive_response(client->zmq_socket);
            process_response(response);
        }
    }

    zmq_close(client->zmq_socket);
    zmq_ctx_destroy(client->zmq_socket);

    return 0;
}