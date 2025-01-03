# Universal Neural Network Computing Accelerator

## 一、项目简介
本项目是一个图像目标检测系统，使用 RV1106 芯片和 RKNN（瑞芯微神经网络）技术，并通过 ZeroMQ 实现客户端与服务器之间的通信，旨在高效地完成图像数据的传输与处理。客户端将图像发送至服务器，服务器利用 RV1106 的处理能力和 RKNN 技术进行目标检测，并将检测结果返回给客户端。

## 二、安装说明

### （一）依赖安装

#### 1. ZeroMQ
ZeroMQ 用于进程间的通信。
- **Linux 系统**：
  ```sh
  sudo apt-get install libzmq3-dev
  ```
- **Windows 系统**：
  从 ZeroMQ 的官方网站下载并安装预编译的 ZeroMQ 库。


#### 2. libav*
libav* 库可用于处理图像和视频相关的编解码、格式转换等操作，提供丰富的多媒体处理功能。
- **Linux 系统**：
  ```sh
  sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev
  ```
- **Windows 系统**：
  请从相应的软件源或官方网站下载 libav* 的 Windows 版本，并根据其提供的安装说明进行安装。


#### 3. RKNN
RKNN 是本项目实现神经网络推理的核心，为在 RV1106 芯片上部署和运行神经网络模型提供支持。
- 请严格按照瑞芯微官方的文档说明，在开发环境中安装 RKNN 工具包，并确保其与 RV1106 芯片兼容。


## 三、项目编译

### （一）服务器端编译
进入服务器目录，执行以下命令：
```sh
cd server
mkdir build && cd build
cmake..
make
```


### （二）客户端编译
进入客户端目录，执行以下命令：
```sh
cd client
mkdir build && cd build
cmake..
make
```


## 四、项目运行

### （一）启动服务器
首先，请确保 RV1106 设备已正确配置并连接。然后，在设备上运行服务器程序：
```sh
cd server/build
./server
```


### （二）启动客户端
在可以与服务器通信的设备上（例如同一网络中的计算机）运行客户端程序：
```sh
cd client/build
./client
```


## 五、技术细节

### （一）服务器端
- **detector.h 和 detector.c**：
  - `detect_image_gpu` 函数：根据 `use_cuda` 参数调用 GPU 计算辅助函数（使用 RKNN）进行目标检测。
  - `free_detection_result` 函数：释放检测结果结构体所占用的内存。


- **gpu_utils.h 和 gpu_utils.c**：
  - `perform_detection_rknn` 函数：使用 RV1106 芯片上的 RKNN 实现目标检测。该函数的主要步骤如下：
    1. 初始化 RKNN 上下文。
    2. 加载预训练的神经网络模型（例如已转换为 RKNN 格式的模型）。
    3. 对输入图像数据进行预处理，使其满足模型的输入要求。
    4. 使用 RKNN API 在 RV1106 上运行推理。
    5. 对输出结果进行后处理，提取检测结果，如边界框、标签和置信度得分。
    6. 将结果转换为 `DetectionResult` 结构体。


- **server.c**：
  - 创建 ZeroMQ 服务器套接字，等待接收来自客户端的图像数据。
  - 调用 `detect_image_gpu` 函数执行目标检测。
  - 将检测结果（物体的数量和位置信息）发送回客户端。


### （二）客户端
- **client.c**：
  - 创建 ZeroMQ 客户端套接字，将图像数据发送至服务器。
  - 接收并处理服务器返回的检测结果，包括检测到的物体数量及其详细信息。


## 六、性能优化与测试

### （一）性能优化
- **RKNN 优化**：
  - 利用 RKNN 的量化和剪枝功能优化用于 RV1106 的神经网络模型，以提高推理速度。
  - 调整 RKNN 配置，最大限度地利用 RV1106 的硬件资源。


- **ZeroMQ 优化**：
  - 调整套接字类型和缓冲区大小，优化消息的发送和接收。
  - 考虑使用 ZeroMQ 的高级特性，如消息队列和负载均衡。


### （二）性能测试
- 使用 RKNN 内置的性能分析工具监控 RV1106 上的推理性能。
- 使用网络性能测试工具监测 ZeroMQ 的通信性能。


## 七、错误处理与日志记录

### （一）错误处理
- 在代码中添加错误检查，特别是对于 RKNN 操作、ZeroMQ 操作和文件操作。
- 使用 `assert` 语句对关键步骤进行调试。


### （二）日志记录
- 集成日志库（如 `log4c` 或 `syslog`），记录程序的运行状态、错误信息和性能指标，以便于调试和优化。


## 八、扩展与修改

### （一）模型升级
- 在 `gpu_utils.c` 中更新神经网络模型，以提高目标检测的准确性和速度。可以使用流行的深度学习框架（如 TensorFlow 或 PyTorch）训练自己的模型，并将其转换为 RKNN 格式。


### （二）算法增强
- 通过更新 `detector.c` 和 `gpu_utils.c` 实现不同的目标检测算法和模型。


### （三）ZeroMQ 增强
- 实现更高级的通信模式（例如发布-订阅），以满足更复杂的分布式系统需求。
- 增加多线程支持，以便同时处理多个客户端的请求。


## 九、注意事项
- 确保 RV1106 设备已正确配置所需的驱动程序和库。
- 执行 RKNN 模型转换时，请确保操作的正确性，保证转换后的模型与 RV1106 兼容。
- 注意 RV1106 上的内存使用，避免内存泄漏和溢出。


## 十、使用示例
以下是系统工作的一个简单示例：
1. 客户端从文件（例如 `../images/test_image.jpg`）读取图像。
2. 客户端使用 ZeroMQ 将图像数据发送至服务器。
3. 服务器接收图像，使用 RV1106 上的 RKNN 进行目标检测，并返回结果。
4. 客户端接收检测结果，并打印每个检测到的物体的位置、大小、置信度和标签信息。

## 十一、protobuf-c安装
要确认 `protobuf-c` 是否已经安装在您的系统上，可以使用以下几种方法：
### 方法 1: 使用包管理器
#### 对于基于 Debian 的系统（如 Ubuntu）：
```sh
sudo apt-get update
dpkg -l | grep protobuf-c
```
如果 `protobuf-c` 已经安装，`dpkg -l` 命令的输出将包含与 `protobuf-c` 相关的行。
#### 对于基于 Red Hat 的系统（如 CentOS）：
```sh
sudo yum list installed | grep protobuf-c
```
或者，如果使用 `dnf`：
```sh
sudo dnf list installed | grep protobuf-c
```
如果 `protobuf-c` 已经安装，上述命令的输出将包含与 `protobuf-c` 相关的行。
### 方法 2: 使用 `find_package` 命令
在 CMakeLists.txt 文件中，您可以使用 `find_package` 命令来查找 `protobuf-c`：
```cmake
find_package(Protobuf-C REQUIRED)
if (Protobuf-C_FOUND)
    message(STATUS "protobuf-c found")
else()
    message(FATAL_ERROR "protobuf-c not found")
endif()
```
然后运行 `cmake`：
```sh
cmake .
```
如果 `protobuf-c` 已经安装并且可以被找到，CMake 将输出 "protobuf-c found"。
### 方法 3: 查找库文件
您也可以直接在文件系统中查找 `protobuf-c` 的库文件：
```sh
find /usr -name '*protobuf-c*'
```
或者如果您知道库文件的默认位置：
```sh
ls /usr/lib/x86_64-linux-gnu/libprotobuf-c.so
```
如果 `protobuf-c` 安装在系统上，上述命令应该返回库文件的路径。
### 方法 4: 使用 `pkg-config`
如果您的系统支持 `pkg-config`，您可以使用以下命令来检查 `protobuf-c` 是否安装：
```sh
pkg-config --exists protobuf-c
```
如果 `protobuf-c` 已安装，上述命令不会有任何输出。如果未安装，您将看到错误消息。
### 方法 5: 查看版本
您也可以使用 `pkg-config` 来查看已安装的 `protobuf-c` 版本：
```sh
pkg-config --modversion protobuf-c
```
如果 `protobuf-c` 已安装，这将返回安装的版本号。如果未安装，您将看到错误消息。
