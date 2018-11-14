import ssl
import socket
import time 
HOST = 'imap.gmail.com'
PORT = 993
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.settimeout(10)
wrappedSocket = ssl.wrap_socket(sock, ssl_version=ssl.PROTOCOL_TLSv1)
wrappedSocket.connect((HOST, PORT))
print wrappedSocket.recv(5000)

wrappedSocket.send('a001 LOGIN acshayo@gmail.com Madhima10\r\n')
print wrappedSocket.recv(5000)

wrappedSocket.send('a002 select INBOX\r\n')
print wrappedSocket.recv(5000)

wrappedSocket.send('a004 SEARCH SUBJECT \"AC Command\"\r\n')
print wrappedSocket.recv(5000)

wrappedSocket.send('a005 fetch 8 BODY[TEXT]\r\n')
print wrappedSocket.recv(5000)

wrappedSocket.send('a006 STORE 7 +FLAGS (\Deleted)\r\n')
print wrappedSocket.recv(5000)

wrappedSocket.send('a007 EXPUNGE\r\n')
print wrappedSocket.recv(5000)


wrappedSocket.send('a008 CLOSE\r\n')
print wrappedSocket.recv(5000)



msgindex = 0
def sendAndRecv(command):
	global msgindex
	msgindex+=1
	wrappedSocket.send('a00' + str(msgindex) + " " + command + '\r\n')
	time.sleep(1)
	print wrappedSocket.read(5000)