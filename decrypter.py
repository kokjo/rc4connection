import socket
import select
import os
import sys
from Crypto.Cipher import ARC4
import pwn


def open_encrypted_socket(addr, port, key="secret"):
    fd1, fd2 = socket.socketpair()
    pid = os.fork()
    if pid == 0:
        fd1.close()
        enc_rc4 = ARC4.new(key)
        dec_rc4 = ARC4.new(key)
        sock = socket.socket()
        sock.connect((addr, port))
        while True:
            r,w,x = select.select([sock, fd2], [], [])
            if sock in r:
                data = sock.recv(1024)
                if data == "":
                    break;
                data = dec_rc4.decrypt(data)
                fd2.send(data)
            if fd2 in r:
                data = fd2.recv(1024)
                if data == "":
                    break;
                data = enc_rc4.encrypt(data)
                sock.send(data)
        sock.close()
        fd2.close()
        exit(0)
    if pid > 0:
        fd2.close()
        s = pwn.sock.sock("default")
        s.sock = fd1
        s.settimeout("default")
        return s
    fd1.close()
    fd2.close()
    return None

r = open_encrypted_socket("127.0.0.1", 5555)
r.interactive()
