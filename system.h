#ifndef SYSTEM_H_
#define SYSTEM_H_

#define BUFFER_MAX_LENGTH 1024

void System_runCommand(char* command);

void System_readFile(char* fileName, char* buffer);

void System_writeFile(char* fileName, char* buffer);

#endif