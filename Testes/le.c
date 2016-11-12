#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd, n, n_bytes;
    char str[80];

    close(1);
    //open("novo.txt", O_CREAT|O_WRONLY, 0600);
    mkfifo("canal.txt", 0600);

    fd = open(argv[1], O_RDONLY);
    read(fd, &n, sizeof(n));
    printf("n = %d\n", n);
    while( n_bytes = read(fd, str, 79) ){
        str[n_bytes] = '\0';
        printf("%s", str);
    }

    //close(fd);
    unlink(argv[1]);
    return 0;
}
