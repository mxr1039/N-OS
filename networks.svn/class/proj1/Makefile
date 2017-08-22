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

all:
	@echo "Targets: build, test, clean, submit"

build:
	make arrange-h-e

arrange-h-e: arrange-h-e.c arrange-h-e.h
	cc ${COPTS} -o $@ $<

run: arrange-h-e
	./arrange-h-e -v name1:name2
	
test: arrange-h-e
	-rm test.out
	./test.pl > test.out
	diff test.ref test.out

clean:
	-rm arrange-h-e test.out

submit:
	@echo svn add your work
	svn commit -m "submitted for grading"


