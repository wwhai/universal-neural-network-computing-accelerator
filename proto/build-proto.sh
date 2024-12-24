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
set -e
# 检查是否提供了proto文件名
if [ -z "$1" ]; then
    echo "Usage: $0 <proto_file>"
    exit 1
fi

PROTO_FILE=$1
PROTO_DIR=./
PROTO_NAME="api.proto"

# 检查proto文件是否存在
if [ ! -f "${PROTO_FILE}" ]; then
    echo "Proto file ${PROTO_FILE} not found."
    exit 1
fi

# 编译proto文件为C语言
protoc --proto_path=${PROTO_DIR} --c_out=${PROTO_DIR} ${PROTO_FILE}

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "Compiled ${PROTO_FILE} successfully."
else
    echo "Failed to compile ${PROTO_FILE}."
    exit 1
fi
