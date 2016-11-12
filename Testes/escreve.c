#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd, n=38, i;
    char str[80];

    close(0);
    //open("etc/passwd", O_RDONLY);

    fd = open(argv[1], O_WRONLY|O_CREAT, 0600);
    i = write(fd, &n, sizeof(n));

    do {
        fgets(str, 79, stdin);
        i = write (fd, str, strlen(str));
    } while (strncmp(str, "fim", 3) != 0);

    close(fd);

    return 0;
}
