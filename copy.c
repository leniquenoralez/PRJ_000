#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 4096;

int main(int argc, char **argv)
{

    struct stat sourceStat;
    struct stat destinationStat;
    char *DIRECTORY = "directory";
    char *FILE = "file";

    char *source = argv[1];
    char *destination = argv[2];

    if (lstat(source, &sourceStat) < 0)
    {
        fprintf(stderr, "failed getting source file info");
        return -1;
    }

    if (lstat(destination, &destinationStat) < 0)
    {
        fprintf(stderr, "failed getting destination file/directory info");
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
    int destinationFd;
    if (S_ISDIR(destinationStat.st_mode))
    {
        char *newFilePath = strncat(strncat(destination, "/",1), source, strlen(source));
        destinationFd = open(newFilePath, O_WRONLY | O_CREAT, 0666);
        if (destinationFd < 0)
        {
            fprintf(stderr, "error creating new file");
        }
        
        
    }else {
        destinationFd = open(destination, O_WRONLY);
    }

    while ((remainingBytes = read(sourceFd, buf, 4096)) > 0) {

        if (write(destinationFd, buf, remainingBytes) != remainingBytes)
        {
            close(sourceFd);
            close(destinationFd);
            fprintf(stderr, "error writing to destination file");
            return -1;
        }

        if (remainingBytes < 0)
        {
            close(sourceFd);
            close(destinationFd);
            fprintf(stderr, "error reading data from source file");
            return -1;
        }
    }
    close(sourceFd);
    close(destinationFd);
    return 0;
}