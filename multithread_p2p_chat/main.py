import socket
import threading
import sys
import time
from random import randint
class Server(object):
    connections = []
    peers = []
    def __init__(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("0.0.0.0",10000))
        s.listen(5)
        print("Server running")
        while True:
            c, addr = s.accept()
            cThread = threading.Thread(target=self.handler, args=(c, addr))
            cThread.daemon = True
            cThread.start()
            self.connections.append(c)
            self.peers.append(addr[0])
            print(self.connections)
            self.sendPeers()

    def handler(self,c, a):
        while True:
            data = c.recv(1024)
            for conn in self.connections:
                conn.send(data)
            if not data:
                print("disscn")
                self.connections.remove(c)
                self.peers.remove(a[0])
                c.close()
                self.sendPeers()
                break
            print(data)
        
    def sendPeers(self):
        p = ""
        for peer in self.peers:
            p = p + peer + ","

        for conn in self.connections:
            conn.send(b'\x11' + bytes(p,"utf-8"))

class Client(object):
    def __init__(self, addr):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.connect((addr,10000))
        iThread = threading.Thread(target=self.sendMsg, args=(s,))
        iThread.daemon = True
        iThread.start() #the sendMsg will run in the background
        while(True):
            data = s.recv(1024)
            if not data:
                break
            if data[0:1] == b'\x11':
                # print("Got peers ")
                self.updatePeers(data[1:])
            else:
                print(str(data, "utf-8"))


    def sendMsg(self, s):
        while True:
            s.send(bytes(input("en"), 'utf-8'))

    def updatePeers(self, data):
        p2p.peers = str(data,"utf-8").split(",")[:-1]

class p2p:
    peers = ["127.0.0.1"]

while True:
    try:
        print("Trying to connect")
        time.sleep(randint(1,5))
        print(p2p.peers)
        for peer in p2p.peers:
            try:
                client = Client(peer)
            except KeyboardInterrupt:
                sys.exit(0)
            except:
                pass
            try:
                server = Server()
            except KeyboardInterrupt:
                sys.exit(0)
            except:
                pass
    except KeyboardInterrupt:
        sys.exit(0)
    