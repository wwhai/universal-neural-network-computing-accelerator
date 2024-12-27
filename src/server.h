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

#ifndef SERVER_H
#define SERVER_H

#include <zmq.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "clog.h"
#include "utils.h"
#include "api.pb-c.h"

void start_server(void *context, void *socket);
void handle_getinfo_request(void *socket, const UNNCA__AcceleratorInfoRequest *request);
void handle_ping_request(void *socket, const UNNCA__PingRequest *request);
void handle_auth_request(void *socket, const UNNCA__AuthRequest *request);
void handle_detect_request(void *socket, const UNNCA__DetectRequest *request);
void report_error(void *socket, const char *error_msg);
void stop_server(void *context, void *socket);

#endif // SERVER_H