import socket




udp3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp3.bind((self.ot.host, self.ot.port[2]))
fd3=udp3.fileno()
udp3.setblocking(0)

