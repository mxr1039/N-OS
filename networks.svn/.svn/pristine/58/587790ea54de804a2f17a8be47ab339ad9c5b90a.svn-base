#
# Name: pikachu
# Date: 14 feb 2016
# 
# Note: Makefiles begin with a captal M
#   Indentation MUST be with a tab
#
# tagets build, test, clean and submit are required.
# 

COPTS= 
#COPTS= -Wall

# adjust these macro values accordingly.
# you may have to adjust PORT= to claim an available port
PORT= 33031
FILE_S = file-on-server.txt
FILE_C = file-on-client.txt
SERVER_HOST = localhost

AESD = tiny-AES128-C

all:
	@echo "Targets: build, test, clean, submit"

build:
	make ttftp-enc

aes.o: aes.c aes.h
	cc ${COPTS} -c -o $@ $<

ttftp-enc-server.o: ttftp-enc-server.c ttftp-enc.h
	cc ${COPTS} -c -o $@ $<

ttftp-enc-client.o: ttftp-enc-client.c ttftp-enc.h
	cc ${COPTS} -c -o $@ $<

ttftp-enc: ttftp-enc.c ttftp-enc-server.o ttftp-enc-client.o aes.o ttftp-enc.h 
	cc ${COPTS} -o $@ $< ttftp-enc-client.o ttftp-enc-server.o aes.o

run-server: ttftp-enc
	echo `date` >> ${FILE_S}
	./ttftp-enc ${PORT}
	
test: ttftp-enc
	echo `date` >> ${FILE_S}
	./ttftp-enc -L  ${PORT} &
	@echo "server started"	
	./ttftp-enc -h ${SERVER_HOST} -f ${FILE_S} ${PORT} > ${FILE_C}
	diff ${FILE_S} ${FILE_C}

clean:
	-rm ttftp-enc ttftp-enc-server.o ttftp-enc-client.o ${FILE_S} ${FILE_C}
	-rm aes.o
	-rm -rf ${AESD}
	
aes-git:
	git clone https://github.com/kokke/tiny-AES128-C.git

submit:
	@echo svn add your work
	svn commit -m "submitted for grading"


