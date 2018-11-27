import socket
import threading

class Client(object):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    def __init__(self):
        self.s.connect(("127.0.0.1",4447))
        self.s.send(bytes(input("Enter your unique no."), "utf-8"))
        self.s.send(bytes(input("Enter the number that you want to chat with"), "utf-8"))
        iThread = threading.Thread(target=self.sendMsg)
        iThread.daemon = True
        iThread.start() #the sendMsg will run in the background
        while(True):
            data = self.s.recv(1024)
            if not data:
                break
            print(str(data,"utf-8"))


    def sendMsg(self):
        print("Group Chat")
        while True:
            self.s.send(bytes(input(""), 'utf-8'))
            
if __name__ == "__main__":
    client = Client()
