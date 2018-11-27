import socket
import threading


class Server(object):
    # create a sockets
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Store the connected clients
    connections = []
    users = []

    def __init__(self):
        self.s.bind(("127.0.0.1",4447))
        self.s.listen(1)
        

    def handler(self, c, addr):  
        connectTo = str(c.recv(1024), "utf-8") #connect to unique id
        while True:
            data = c.recv(1024)
            # print("count {},".format(threading.active_count()))
            for conn in self.connections:
                if conn[1] == connectTo:
                    conn[0].send(data)
            if not data:
                print("{}:{} Disconnected".format(addr[0],addr[1]))
                for conn in self.connections:
                    if conn[0] == c:
                        self.connections.remove(conn)
                        break
                c.close()
                break
            # print(data)

    def run(self):
        print("Server started ....")
        while True:
            c, addr = self.s.accept()
            ownId = str(c.recv(1024), "utf-8")
            print("my no : {}".format(ownId))
            cThread = threading.Thread(target=self.handler, args=(c, addr))
            cThread.daemon = True #kill the thread when the main process exits
            cThread.start()
            self.connections.append([c,ownId])
            print("{}:{} Connected".format(addr[0], addr[1]))

if __name__ == "__main__":
    server = Server()
    server.run()
