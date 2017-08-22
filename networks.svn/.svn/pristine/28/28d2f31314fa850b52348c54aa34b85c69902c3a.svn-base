#
# makefile for netbounce example
# Jan 12,2016
# bjr
#

CC = gcc
LOCALHOST = localhost
PORT = 3333

all:
	make netbounce

netbounce: netbounce-server netbounce-client

netbounce-server: netbounce-server.c 
	${CC} -o $@ $<
	
netbounce-client: netbounce-client.c 
	${CC} -o $@ $<
	
test:
	@echo In one window, make run-server
	@echo Then, in another window, make run-client
	
run-server: netbounce-server
	@echo Ready to bounce!
	./netbounce-server -lv -p ${PORT}

run-client: netbounce-client
	./netbounce-client -h ${LOCALHOST} -p ${PORT} -v "The amazing net bounce!"

run-remote: netbounce-client
	./netbounce-client -h shiloh -p ${PORT} -v "The amazing net bounce!"

submit:
	svn commit -m "submitted for grade"

clean:
	-rm netbounce-client netbounce-server
