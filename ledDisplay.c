#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "ledDisplay.h"
#include "system.h"
#include "periodTimer.h"
#include "shutdownManager.h"

// I2C bus and address
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x20

// I2C values for LED Display
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

// Files for LED Display
#define GPIO_DIRECTION_FILE "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_FILE "/sys/class/gpio/gpio%d/value"

// GPIO values for LED Display
static int gpioDigits[2] = {61, 44};

// Top and Bottom values for LED Display Patterns
static unsigned char topPattern[10] = {0x86, 0x12, 0x0E, 0x06, 0x8A, 0x8C, 0x8C, 0x14, 0x8E, 0x8E};
static unsigned char bottomPattern[10] = {0xA1, 0x80, 0x31, 0xB0, 0x90, 0xB0, 0xB1, 0x04, 0xB1, 0x90};

// I2C file descriptor
static int i2cFileDesc;

// File setup functions
static void Display_setDigitHardware(int digit);
static void Display_setFilesGpio(char* fileName, char* value, int gpio);
static void Display_setFilesGpioAll(char* fileName, char* value);

// Display thread
static pthread_t displayThread;
static void* Display_threadFunction(void* args);

// Displaying digit
static int displayDigit;

void Display_init(void)
{
    System_runCommand("config-pin P9_18 i2c");
    System_runCommand("config-pin P9_17 i2c");

    Display_setFilesGpioAll(GPIO_DIRECTION_FILE, "out");

    i2cFileDesc = System_initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    System_writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    System_writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);
}

void Display_cleanup(void)
{
    Display_setFilesGpioAll(GPIO_VALUE_FILE, "0");
    close(i2cFileDesc);
}

void Display_setDigit(int digit)
{
    displayDigit = digit;
}

void Display_startDisplaying(void)
{
    pthread_create(&displayThread, NULL, &Display_threadFunction, NULL);
}

void Display_stopDisplaying(void)
{
    pthread_join(displayThread, NULL);
}

static void* Display_threadFunction(void* args)
{
    while(!Shutdown_isShuttingDown()) {
        Display_setDigitHardware(displayDigit);
    }
    return NULL;
}

static void Display_setDigitHardware(int digit)
{
    if (digit > 99) {
        digit = 99;
    }

    int leftNum = digit / 10;
    int rightNum = digit - (leftNum * 10);
    int digitArray[] = {leftNum, rightNum};

    for (int i = 0; i < 2; i++)
    {
        Display_setFilesGpioAll(GPIO_VALUE_FILE, "0");
        
        int digitValue = digitArray[i];
        System_writeI2cReg(i2cFileDesc, REG_OUTA, bottomPattern[digitValue]);
        System_writeI2cReg(i2cFileDesc, REG_OUTB, topPattern[digitValue]);

        Display_setFilesGpio(GPIO_VALUE_FILE, "1", gpioDigits[i]);

        Timer_sleepForMs(5);
    }
    Display_setFilesGpioAll(GPIO_VALUE_FILE, "0");
}

static void Display_setFilesGpio(char* fileName, char* value, int gpio)
{
    char filePath[BUFFER_MAX_LENGTH];
    snprintf(filePath, BUFFER_MAX_LENGTH, fileName, gpio);
    System_writeFile(filePath, value);
}

static void Display_setFilesGpioAll(char* fileName, char* value)
{
    for (int i=0; i < 2; i++) {
        char filePath[BUFFER_MAX_LENGTH];
        snprintf(filePath, BUFFER_MAX_LENGTH, fileName, gpioDigits[i]);
        System_writeFile(filePath, value);
    }
}