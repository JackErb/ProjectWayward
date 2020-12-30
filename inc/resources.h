#pragma once

#include <string>

namespace resources {
char *ReadTextFile(const std::string &file_name, int *length);

unsigned char *ReadPng(const std::string &file_name, int *width, int *height);
void FreePng(unsigned char *data);
}
