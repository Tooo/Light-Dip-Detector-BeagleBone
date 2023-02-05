// system.h
// Functions that manages files and comands
#ifndef SYSTEM_H_
#define SYSTEM_H_

#define BUFFER_MAX_LENGTH 1024

// Run the given Command
void System_runCommand(char* command);

// Read/write a file, given the file path + name and buffer
void System_readFile(char* fileName, char* buffer);
void System_writeFile(char* fileName, char* buffer);

// I2C functions
int initI2cBus(char* bus, int address);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

#endif