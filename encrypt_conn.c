#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "rc4.h"

void static inline proxy_conns(int fd1, int fd2, void *key, size_t key_len){
    struct rc4_state dec_rc4;
    struct rc4_state enc_rc4;
    char buf[1024];
    int size;
    fd_set fds;

    rc4_init(&dec_rc4, key, key_len);
    rc4_init(&enc_rc4, key, key_len);

    while(1){
        FD_ZERO(&fds);
        FD_SET(fd1, &fds);
        FD_SET(fd2, &fds);
        select(FD_SETSIZE, &fds, NULL, NULL, NULL);
        if(FD_ISSET(fd1, &fds)){
            size = read(fd1, buf, sizeof(buf));
            if(size <= 0) break;
            rc4_decrypt(&dec_rc4, buf, buf, size);
            write(fd2, buf, size);
        }
        if(FD_ISSET(fd2, &fds)){
            size = read(fd2, buf, sizeof(buf));
            if(size <= 0) break;
            rc4_encrypt(&enc_rc4, buf, buf, size);
            write(fd1, buf, size);
        }
    }
}

pid_t enable_encryption(int outfd, void *key, size_t key_len){
    pid_t pid;
    int i;
    int socks[2];
    socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
    if((pid = fork()) > 0){
        //parent
        for(i=0; i<3; i++) dup2(socks[1], i);
        for(i=0; i<2; i++) close(socks[i]);
        return pid;
    } else {
        //child
        close(socks[1]);
        proxy_conns(outfd, socks[0], key, key_len);
        _exit(0);
    }
    return -1;
}
