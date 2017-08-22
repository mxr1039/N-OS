
# makefile for proj5
# author: bjr
# create: 12 nov 2016
# last update: 22 nov 2016

# 
# use target installfuse to install the fuse developer package
#

ZERO= /dev/zero
VD= vd.dmg
MNT= mnt

DOT=  | head -2 | tail -n +2
NODOT= | tail -n +2

minifat: minifat.c dos_filenames.o minifat.h
	cc $< `pkg-config fuse --cflags --libs` -o $@ dos_filenames.o
	
dos_filenames.o: dos_filenames.c dos_filenames.h
	cc -o $@ -c $<

${VD}:
	-rm ${VD}
	dd if=${ZERO} of=${VD} count=1024 bs=1024

run: minifat ${VD}
	-mkdir ${MNT}
	./minifat -vcd ${MNT} ${VD}
	hexdump -C ${VD}

test-start: 
	make clean
	make minifat
	mkdir ${MNT}
	make ${VD}
	./minifat -cd ${MNT} ${VD}

test:
	make test-start
	make test-stage1-basic
	make test-stage1-dirlarge
	diff test-stage1.out test-stage1.ref

test-stage1-basic:
	touch ${MNT}/myfile.txt
	touch ${MNT}/a.c
	touch ${MNT}/b.h
	rm ${MNT}/a.c
	echo "-" > test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out
	mkdir ${MNT}/mydir.dir
	mkdir ${MNT}/adir
	mkdir ${MNT}/bdir
	rmdir ${MNT}/adir
	echo "-" >> test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out

test-stage1-dirlarge:
	cd ${MNT}; touch a b c d e f g h i j k l m n o p q r s t u v w x y z 
	cd ${MNT}; touch a.a b.b c.c d.d e.e f.f
	echo "-" >> test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out
	cd ${MNT}; rm a.a b.b c.c d.d e.e f.f
	echo "-" >> test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out

clean:
	-fusermount -u ${MNT}
	-rmdir ${MNT}
	-rm minifat ${VD} dos_filenames.o test-stage1.out

submit:
	svn commit -m "submitted for credit"
	
installfuse:
	sudo apt-get install libfuse-dev
	sudo apt-get install pkg-config


