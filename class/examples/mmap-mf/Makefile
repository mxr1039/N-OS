
ZERO= /dev/zero
VD= vd.dmg

mmap-mf: mmap-mf.c minifat.h
	cc -o mmap-mf mmap-mf.c
	
${VD}:
	-rm {$VD}
	dd if=${ZERO} of=${VD} count=1024 bs=1024

test: mmap-mf ${VD}
	./mmap-mf ${VD}
	hexdump -C ${VD}
	
clean:
	-rm mmap-mf ${VD}

	