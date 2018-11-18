#import socket
from socket import socket, AF_INET, SOCK_STREAM
import sys
import datetime


class Fruit:
    qty = 0

    def __init__(self, qty, t_stamp=0, unique_clients=0):
        self.qty = qty
        self.t_stamp = t_stamp
        self.unique_clients = unique_clients

    def reduceQty(self, qty):
        self.qty = self.qty - qty


def parseRequest(msg):
    request = msg.split(" ")
    try:
        return request[0], int(request[1])
    except:
        print("Plese send proper request")
        sys.exit(1)


def printFruits():
    print("\tName\tqty\ttimestamp")
    print("\tMango\t{}\t{}".format(mango.qty, "sd"))
    print("\tOrange\t{}\t{}".format(orange.qty, "sd"))
    print("\tGuava\t{}\t{}".format(guava.qty, "sd"))


def doTransaction(reqFruit, qty):
    if(reqFruit == "M"):
        # Mango
        if(qty <= mango.qty):
            ret_message = "transaction successful"
            mango.reduceQty(qty)
            mango.t_stamp = datetime.datetime.now()
        else:
            ret_message = "transaction error"
    elif(reqFruit == "O"):
        # Orange
        if(qty <= orange.qty):
            ret_message = "transaction successful"
            orange.reduceQty(qty)
            orange.t_stamp = datetime.datetime.now()
        else:
            ret_message = "transaction error"
    elif(reqFruit == "G"):
        # Guava
        if(qty <= guava.qty):
            ret_message = "transaction successful"
            guava.reduceQty(qty)
            orange.t_stamp = datetime.datetime.now()
        else:
            ret_message = "transaction error"
    else:
        ret_message = "transaction error"
    return ret_message
def addIpToList(addr):
    clients.append(addr)
    print("Unique clients")
    print(clients)

clients = []
mango = Fruit(10)
orange = Fruit(15)
guava = Fruit(20)
if __name__ == "__main__":
    host = "127.0.0.1"
    port = 4447
    s = socket(AF_INET, SOCK_STREAM, 0)
    s.bind((host, port))
    s.listen(5)
    printFruits()
    print("Listening for connection..")
    while(True):
        q, addr = s.accept()
        # get the transaction request from the client
        msg = q.recv(100)
        print("MSG from client: '{}'".format(msg.decode()))
        # Parse the fruit name and qty
        reqFruit, qty = parseRequest(msg.decode())

        # Check if qty exist if yes then do transaction
        result = doTransaction(reqFruit, qty)
        print(result)
        if( result == "transaction successful"):
            addIpToList(addr)
        printFruits()
        q.send("Successfully done".encode())
    s.close()
