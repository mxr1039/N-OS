#
#
# Makefile for proj5
# Author: bjr
# Created: 04/2016
# Last Update: 13/04/2016
#
#

regex-test: regex-test.c 
	cc regex-test.c -o regex-test
	./regex-test

base64-test: base64-test.c
	cc -o base64-test base64-test.c -lcrypto -lm -w
	./base64-test

curl-test: https.c
	cc -o https https.c -lcurl
	./https

clean:
	-rm regex-test base64-test https

