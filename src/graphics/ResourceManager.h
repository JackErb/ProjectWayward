#ifndef ResourceManager_h
#define ResourceManager_h

#include <string>

unsigned char *loadPng(std::string file_name, int *width, int *height);
void freePng(unsigned char *data);

char *loadTextFile(std::string file_name, int *length);


#endif  /* ResourceManager_h */
