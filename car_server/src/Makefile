all: car_server

OBJS = car_server.o socket.o pack.o camera.o moter_cntl.o car_cntl.o uart.o

CC = gcc
CCFLAGS = -Wall -c -o


socket.o:socket.c socket.h msg_output.h
	$(CC) -c socket.c -o socket.o

uart.o:uart.c uart.h msg_output.h
	$(CC) -c uart.c -o uart.o

pack.o:pack.c pack.h camera.h  car_cntl.h moter_cntl.h packCmd_port.h  msg_output.h
	$(CC) -c pack.c -o pack.o
	
camera.o:camera.c camera.h msg_output.h
	$(CC) -c camera.c -o camera.o

car_cntl.o:car_cntl.c car_cntl.h moter_cntl.h camera.h socket.h uart.h msg_output.h
	$(CC) -c car_cntl.c -o car_cntl.o

moter_cntl.o:moter_cntl.c moter_cntl.h msg_output.h
	$(CC) -c moter_cntl.c -o moter_cntl.o
	
car_server.o:car_server.c socket.h pack.h  car_cntl.h  msg_output.h 
	$(CC) $(CCFLAGS) $@ $< $(LDFLAGS)

car_server: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	

clean:
	rm -f rbcfg *.o

