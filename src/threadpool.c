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
#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "clog.h"
// 线程函数，从任务队列中取出任务进行处理
void *worker_thread(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;
    while (1)
    {
        pthread_mutex_lock(&pool->queue_mutex);
        while (pool->task_count == 0)
        {
            pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
        }
        DataPackage package = pool->task_queue[--pool->task_count];
        pthread_mutex_unlock(&pool->queue_mutex);
        switch (package.request->request_case)
        {
        case UNNCA__RPC_REQUEST__REQUEST_AUTH_REQUEST:
            printf("Auth request: uuid=%s\n", package.request->auth_request->uuid);
            handle_auth_request(package.socket, package.request->auth_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_PING_REQUEST:
            handle_ping_request(package.socket, package.request->ping_request);
            printf("Ping request received\n");
            break;
        case UNNCA__RPC_REQUEST__REQUEST_DETECT_REQUEST:
            printf("Detect request received\n");
            handle_detect_request(package.socket, package.request->detect_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST_ACCELERATOR_INFO_REQUEST:
            printf("AcceleratorInfo request received\n");
            handle_getinfo_request(package.socket, package.request->accelerator_info_request);
            break;
        case UNNCA__RPC_REQUEST__REQUEST__NOT_SET:
            printf("Request not set\n");
            report_error(package.socket, "Unknown request");
            break;
        default:
            printf("Unknown request received, request->request_case: %d\n", package.request->request_case);
            report_error(package.socket, "Unknown request");
            break;
        }
        unnca__rpc_request__free_unpacked(package.request, NULL);
    }
    return NULL;
}

// 初始化线程池
void threadpool_init(ThreadPool *pool)
{
    pool->task_count = 0;
    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->queue_cond, NULL);
    for (int i = 0; i < THREAD_POOL_SIZE; ++i)
    {
        if (pthread_create(&pool->threads[i], NULL, worker_thread, (void *)pool) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
}

// 向线程池添加任务
void threadpool_add_task(ThreadPool *pool, void *socket, UNNCA__RpcRequest *request)
{
    pthread_mutex_lock(&pool->queue_mutex);
    if (pool->task_count < MAX_CLIENTS)
    {
        pool->task_queue[pool->task_count].socket = socket;
        pool->task_queue[pool->task_count].request = request;
        pool->task_count++;
        pthread_cond_signal(&pool->queue_cond);
    }
    pthread_mutex_unlock(&pool->queue_mutex);
}

// 销毁线程池
void threadpool_destroy(ThreadPool *pool)
{
    pthread_mutex_destroy(&pool->queue_mutex);
    pthread_cond_destroy(&pool->queue_cond);
    for (int i = 0; i < THREAD_POOL_SIZE; ++i)
    {
        pthread_cancel(pool->threads[i]);
        pthread_join(pool->threads[i], NULL);
    }
}
