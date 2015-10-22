import socket
import os
UDP_IP = "10.0.0.20"
UDP_PORT = 5004
sock = socket.socket(socket.AF_INET, # Internet
    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))
while True:
      data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
      data = data.decode("utf-8")
      #print ("The IP address is", data)
      os.system ("cowsay " + data)
