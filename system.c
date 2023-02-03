#include "system.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void System_runCommand(char* command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");

    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[BUFFER_MAX_LENGTH];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
            break;
        }
        // printf("--> %s", buffer); // Uncomment for debugging
    }

    // Get the exit code from pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command:  %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

void System_readFile(char* fileName, char* buffer)
{
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open %s.\n", fileName);
        exit(1);
    }

    fgets(buffer, BUFFER_MAX_LENGTH, pFile);

    fclose(pFile);
}

void System_writeFile(char* fileName, char* buffer)
{
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL) {
        printf("ERROR: Unable to open %s.\n", fileName);
        exit(1);
    }

    int charWritten = fprintf(pFile, buffer);
    if (charWritten <= 0) {
        printf("ERROR: Cannot write %s.\n", fileName);
        exit(1);
    }

    fclose(pFile);
}