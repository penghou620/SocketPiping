#! python

import socket
import sys
import time

if len(sys.argv) == 4:
  host = sys.argv[1]
  port = int(sys.argv[2])
  fname = sys.argv[3]

  max_len = 1024
  tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  tcp_socket.connect((host, port))

  start = time.time()
  with open(fname, 'r') as client_file:
    while 1:
      packet = client_file.read(max_len)
      if not packet:
        break
      tcp_socket.send(packet)
  print "elapsed ", (time.time() - start)

  tcp_socket.close()
else:
  print "usage: python-client ip port fname"