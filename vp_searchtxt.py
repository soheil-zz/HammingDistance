from simple_vp import *
import socket, select
import sys
import time
import pylab as plt


def hamming(a,b):
    return bin(a ^ b).count('1')

def levenshtein(a,b):
    "Calculates the Levenshtein distance between a and b."
    n, m = len(a), len(b)
    if n > m:
        # Make sure n <= m, to use O(min(n,m)) space
        a,b = b,a
        n,m = m,n

    current = range(n+1)
    for i in range(1,m+1):
        previous, current = current, [i]+[0]*n
        for j in range(1,n+1):
            add, delete = previous[j]+1, current[j-1]+1
            change = previous[j-1]
            if a[j-1] != b[i-1]:
                change = change + 1
            current[j] = min(add, delete, change)

    return current[n]


def main(file_name):

    f = open(file_name)
    f.next()
    aset = [int(w) for w in f]
    rad = 30
    # distance = levenshtein
    distance = hamming

    s = time.time()
    print "\ninput set %s points" % len(aset)
    print "creating tree..."
    root = VpNode(aset, distance=distance)
    print "created: %s nodes" % VpNode.ids
    print "done in s: %s" % (time.time() - s)
    print "searching..."

    CONNECTION_LIST = []    # list of socket clients
    RECV_BUFFER = 4096 # Advisable to keep it as an exponent of 2
    PORT = 8880
         
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # this has no effect, why ?
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(("0.0.0.0", PORT))
    server_socket.listen(10)
 
    # Add server socket to the list of readable connections
    CONNECTION_LIST.append(server_socket)
     
    #now keep talking with the client
    while 1:
        # Get the list sockets which are ready to be read through select
        read_sockets,write_sockets,error_sockets = select.select(CONNECTION_LIST,[],[])
 
        for sock in read_sockets:
             
            #New connection
            if sock == server_socket:
                # Handle the case in which there is a new connection recieved through server_socket
                sockfd, addr = server_socket.accept()
                CONNECTION_LIST.append(sockfd)
                print "Client (%s, %s) connected" % addr
                 
            #Some incoming message from a client
            else:
                # Data recieved from client, process it
                try:
                    #In Windows, sometimes when a TCP program closes abruptly,
                    # a "Connection reset by peer" exception will be thrown
                    data = sock.recv(RECV_BUFFER)
                    # echo back the client message
                    if data:
                        q = int(data)
                        se = VpSearch(root, q, rad, 10)
                        out = se.knn()
                        sock.send("\n".join(str(x) for x in out))
                        sock.close()
                        CONNECTION_LIST.remove(sock)
                 
                # client disconnected, so remove from socket list
                except:
                    # broadcast_data(sock, "Client (%s, %s) is offline" % addr)
                    print "Client (%s, %s) is offline" % addr
                    sock.close()
                    CONNECTION_LIST.remove(sock)
                    continue
         
    s.close()

    while True:
        q = int(raw_input(">>"))

        s = time.time()
        se = VpSearch(root, q, rad, 100)
        out = se.knn()
        print se.stat
        print "founded %s results" % len(out)
        print "done in s: %s" % (time.time() - s)
        print "\n"
        print out
        print


if __name__ == '__main__':
    file_name = sys.argv[1]
    main(file_name)
