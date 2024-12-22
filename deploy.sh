# Copyright (C) 2024 wwhai
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

#!/bin/bash

# 项目名称
PROJECT_NAME="universal-neural-network-computing-accelerator"
PROJECT_DIR="$HOME/$PROJECT_NAME"
CLIENT_EXEC="$PROJECT_DIR/client/build/client"
SERVER_EXEC="$PROJECT_DIR/server/build/server"
CLIENT_SERVICE_FILE="/etc/systemd/system/${PROJECT_NAME}-client.service"
SERVER_SERVICE_FILE="/etc/systemd/system/${PROJECT_NAME}-server.service"


function install_dependencies() {
    echo "正在安装依赖..."
    sudo apt-get update
    sudo apt-get install -y libzmq3-dev libavcodec-dev libavformat-dev libavutil-dev
    # 假设 RKNN 工具包的安装命令如下，你可能需要根据实际情况修改
    # sudo apt-get install rknn-toolkit 或者从官网下载并安装
    echo "依赖安装完成。"
}

function compile_project() {
    echo "正在编译项目..."
    cd $PROJECT_DIR/client
    mkdir -p build && cd build
    cmake..
    make
    cd $PROJECT_DIR/server
    mkdir -p build && cd build
    cmake..
    make
    echo "项目编译完成。"
}

function create_service_files() {
    echo "正在创建服务文件..."
    # 创建客户端服务文件
    cat << EOF > $CLIENT_SERVICE_FILE
[Unit]
Description=${PROJECT_NAME} Client
After=network.target

[Service]
ExecStart=$CLIENT_EXEC
WorkingDirectory=$PROJECT_DIR/client/build
Restart=always
User=$USER

[Install]
WantedBy=multi-user.target
EOF

    # 创建服务器服务文件
    cat << EOF > $SERVER_SERVICE_FILE
[Unit]
Description=${PROJECT_NAME} Server
After=network.target

[Service]
ExecStart=$SERVER_EXEC
WorkingDirectory=$PROJECT_DIR/server/build
Restart=always
User=$USER

[Install]
WantedBy=multi-user.target
EOF
    echo "服务文件创建完成。"
}

function enable_services() {
    echo "正在启用服务并设置开机自启动..."
    sudo systemctl daemon-reload
    sudo systemctl enable ${PROJECT_NAME}-client.service
    sudo systemctl start ${PROJECT_NAME}-client.service
    sudo systemctl enable ${PROJECT_NAME}-server.service
    sudo systemctl start ${PROJECT_NAME}-server.service
    echo "服务已启用并设置为开机自启动。"
}

function uninstall() {
    echo "正在卸载项目..."
    # 停止服务
    sudo systemctl stop ${PROJECT_NAME}-client.service
    sudo systemctl stop ${PROJECT_NAME}-server.service
    # 禁用服务
    sudo systemctl disable ${PROJECT_NAME}-client.service
    sudo systemctl disable ${PROJECT_NAME}-server.service
    # 删除服务文件
    sudo rm $CLIENT_SERVICE_FILE $SERVER_SERVICE_FILE
    # 删除项目目录
    rm -rf $PROJECT_DIR
    echo "项目已卸载。"
}


case "$1" in
    install)
        install_dependencies
        compile_project
        create_service_files
        enable_services
    ;;
    uninstall)
        uninstall
    ;;
    *)
        echo "Usage: $0 [install|uninstall]"
        exit 1
    ;;
esac