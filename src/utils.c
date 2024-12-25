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

#include "utils.h"

/**
 * @brief 打印十六进制字符串
 * @param str 要打印的字符串
 * @param len 字符串长度
 */
void dump_hex_string(const uint8_t *str, int len)
{
    if (str == NULL || len <= 0)
    {
        printf("Invalid input parameters\n");
        return;
    }
    printf("== == Dump (%d) bytes to hex: [", len);
    for (int i = 0; i < len; i++)
    {
        printf("%02x ", (unsigned char)str[i]);
    }
    printf("]\n");
}