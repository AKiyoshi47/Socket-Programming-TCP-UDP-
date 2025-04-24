all: tcpclient.o tcpserver.o udpclient.o udpserver.o
	gcc -g -Wall tcpclient.o -o tcpclient
	gcc -g -Wall tcpserver.o -o tcpserver
	gcc -g -Wall udpclient.o -o udpclient
	gcc -g -Wall udpserver.o -o udpserver


tcpclient.o: tcpclient.c
	gcc -g -Wall -c tcpclient.c

tcpserver.o: tcpserver.c
	gcc -g -Wall -c tcpserver.c

udpclient.o: udpclient.c
	gcc -g -Wall -c udpclient.c

udpserver.o: udpserver.c
	gcc -g -Wall -c udpserver.c


clean:
	rm -f tcpclient.o tcpserver.o udpclient.o udpserver.o \
	      tcpclient tcpserver udpclient udpserver
