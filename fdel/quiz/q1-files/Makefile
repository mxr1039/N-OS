
#
# Name: Francisco Delger
# Date:
# 
# targets build, m, s.o, run and clean are required.
# 

COPTS=

build:
	make m

m: m.c s.o 
	gcc -w -o m m.c s.o 

s.o: s.c s.h
	gcc -w -c s.c
run: m
	./m 83
clean:
	-rm m
submit:
	@echo svn add your work
	svn commit -m "submitted for grading"


