#
# Name:
# Date:
# 
# Note: Makefiles begin with a captal M
#   Indentation MUST be with a tab
#
# tagets build, test, clean and submit are required.
# 

COPTS= 
#COPTS= -Wall

# adjust these next three to your port and hosts
PORT= 33031
MC= shiloh
MS= antietam

all:
	@echo "Targets: build, test, clean, submit"

build:
	make passaround

passaround: passaround.c passaround.h 
	cc ${COPTS} -o $@ $< 

test-loc: passaround 
	./passaround -n 4 -m localhost:localhost:localhost ${PORT}

test-s: passaround
	./passaround -n 2 ${PORT}

test-c: passaround
	./passaround -n 3 -m ${MS}:${MC}:${MS}:${MC} ${PORT}

test: passaround
	-rm test.out
	./passaround -n 4 -m localhost:localhost:localhost ${PORT} > test.out
	./test-p2.pl ${MS} ${MC} test.out
	@echo on ${MS} run make test-s 
	./passaround -n 3 -m ${MS}:${MC}:${MS}:${MC} ${PORT} > test.out
	./test-p2.pl ${MS} ${MC} test.out

clean:
	-rm passaround test.out 

submit:
	@echo svn add your work
	svn commit -m "submitted for grading"

