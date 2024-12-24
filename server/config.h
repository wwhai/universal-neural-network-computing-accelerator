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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clog.h"
typedef struct
{
    int port;
    char modelPath[256];
    int maxConnections;
} Config;
Config config;
void init_config()
{
    log_set_show_full_caller_path(false);
    config.port = 5555;
    config.maxConnections = 2;
    strcpy(config.modelPath, (const char *)"./yolov8.rknn");
}
void parse_ini(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[512];
    char section[64] = "";
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Skip comments and empty lines
        if (line[0] == ';' || line[0] == '#' || strlen(line) == 0)
        {
            continue;
        }

        // Check for section
        if (line[0] == '[')
        {
            sscanf(line, "[%63[^]]", section);
            continue;
        }

        // Parse key-value pairs
        if (strcmp(section, "Server") == 0)
        {
            char key[64];
            char value[256];
            if (sscanf(line, "%63[^=]=%255[^\n]", key, value) == 2)
            {
                if (strcmp(key, "Port") == 0)
                {
                    config.port = atoi(value);
                }
                else if (strcmp(key, "ModelPath") == 0)
                {
                    strncpy(config.modelPath, value, sizeof(config.modelPath) - 1);
                }
                else if (strcmp(key, "MaxConnections") == 0)
                {
                    config.maxConnections = atoi(value);
                }
            }
        }
    }

    fclose(file);
}