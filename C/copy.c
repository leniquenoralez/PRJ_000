#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 4096;

int main(int argc, char **argv)
{

    // return error if argv length is less than 3
    struct stat sourceStat;
    struct stat targetStat;
    char *DIRECTORY = "directory";
    char *FILE = "file";

    char *source = argv[1];
    char *target = argv[2];

    if (lstat(source, &sourceStat) < 0)
    {
        fprintf(stderr, "failed getting source file info");
        return -1;
    }

    if (lstat(target, &targetStat) < 0)
    {
        fprintf(stderr, "failed getting target file/directory info");
        return -1;
    }
    if (!S_ISREG(sourceStat.st_mode))
    {
        fprintf(stderr, "Source must be a regular file, got");
        return -1;
    }

    int sourceFd = open(source, O_RDONLY);
    char buf[4096];
    int remainingBytes;
    int targetFd;
    if (S_ISDIR(targetStat.st_mode))
    {
        char *newFilePath = strncat(strncat(target, "/",1), source, strlen(source));
        targetFd = open(newFilePath, O_WRONLY | O_CREAT, 0666);
        if (targetFd < 0)
        {
            fprintf(stderr, "error creating new file");
        }
        
        
    }else {
        targetFd = open(target, O_WRONLY);
    }

    while ((remainingBytes = read(sourceFd, buf, 4096)) > 0) {

        if (write(targetFd, buf, remainingBytes) != remainingBytes)
        {
            close(sourceFd);
            close(targetFd);
            fprintf(stderr, "error writing to target file");
            return -1;
        }

        if (remainingBytes < 0)
        {
            close(sourceFd);
            close(targetFd);
            fprintf(stderr, "error reading data from source file");
            return -1;
        }
    }
    close(sourceFd);
    close(targetFd);
    return 0;
}