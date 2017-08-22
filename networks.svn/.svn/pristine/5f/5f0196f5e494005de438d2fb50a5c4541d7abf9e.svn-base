
LIB= -lcrypto -lssl -lpthread
HOST= www.miami.edu
HTTPS_PORT= 443
POPHOST= ec2-184-73-91-87.compute-1.amazonaws.com
GPOP= pop.gmail.com
CERT= cert.pem

all:
	make clean
	make s_client
	make s_server

s_client: s_client.c s_client.h
	cc -c s_client.c 
	cc -o s_client s_client.o ${LIB}

s_server: s_server.c s_server.h
	cc -c s_server.c
	cc -o s_server s_server.o ${LIB}

clean:
	-rm s_client s_client.o s_server s_server.o

test: s_client
	cat http-preamble.txt | ./s_client -p ${HTTPS_PORT} ${HOST}

pop:
	./s_client -p pop3s ${GPOP}

server:
	./s_server

gpop:
	openssl s_client -connect ${GPOP}:pop3s

cert:
	openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ${CERT} -out ${CERT}

viewcert:
	openssl x509 -text -in cert.pem

