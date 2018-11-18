#import socket
from socket import socket, AF_INET, SOCK_STREAM

if __name__ == "__main__":
    cont = 'y'
    while(cont == 'y' or cont == 'Y'):
        host = "127.0.0.1"
        port = 4447
        s = socket(AF_INET, SOCK_STREAM, 0)
        s.connect((host, port))
        msg = input("Enter Fruit followed by the no Eg: M 10 :")
        s.send(msg.encode())
        msg = s.recv(100)
        print("Message from server: {}".format(msg.decode()))

        cont = input("Do you want to more transactions")
        s.close()
