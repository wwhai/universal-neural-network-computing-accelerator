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
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <zmq.h>
#include "api.pb-c.h"

#define MAX_CLIENTS 10
#define THREAD_POOL_SIZE 5

typedef struct
{
    void *socket;
    UNNCA__RpcRequest *request;
} DataPackage;

typedef struct
{
    DataPackage task_queue[MAX_CLIENTS];
    int task_count;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_cond;
    pthread_t threads[THREAD_POOL_SIZE];
} ThreadPool;

// 初始化线程池
void threadpool_init(ThreadPool *pool);
// 向线程池添加任务
void threadpool_add_task(ThreadPool *pool, void *socket, UNNCA__RpcRequest *request);
// 销毁线程池
void threadpool_destroy(ThreadPool *pool);

#endif // THREADPOOL_H