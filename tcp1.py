#!/usr/bin/env python
import socket
   
    
TCP_IP = '10.0.0.21'
TCP_PORT = 5005
BUFFER_SIZE = 1024
MESSAGE = bytes("Hello", "utf-8")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv(BUFFER_SIZE)
s.close()
print ("received data:", data.decode("utf-8"))
  
