#ifndef _encrypt_conn_h_
#define _encrypt_conn_h_
void static inline proxy_conns(int fd1, int fd2, void *key, size_t key_len);
pid_t enable_encryption(int outfd, void *key, size_t key_len);
#endif
