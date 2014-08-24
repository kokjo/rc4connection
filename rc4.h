#ifndef _RC4_H_
#define _RC4_H_
struct rc4_state {
    unsigned char i, j;
    unsigned char s[256];
};

void rc4_init(struct rc4_state *state, unsigned char *key, size_t key_len);
void rc4_encrypt(struct rc4_state *state, char *inbuf, char *outbuf, size_t buflen);
#define rc4_decrypt rc4_encrypt
#endif
