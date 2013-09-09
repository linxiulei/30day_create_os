TOOLPATH = z_tools/
INCPATH  = z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del



IPL = ipl.bin

ipl.bin: ipl.nas
	nasm ipl.nas -o ipl.bin

bootpack.gas: bootpack.c
	$(CC!)  bootpack.c -o bootpack.gas

bootpack.nas: bootpack.gas
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj: 
	$(NASK) bootpack.nas bootpack.obj bootpack.lst

naskfunc.obj : naskfunc.nas Makefile
	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

bootpack.bim : bootpack.obj naskfunc.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj

bootpack.hrb: bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

daishu.bin : daishu.nas Makefile
	nasm daishu.nas -o daishu.bin

daishuos.img: daishu.bin bootpack.hrb
	cat daishu.bin bootpack.hrb > daishuos.img

daishuos.bin: ipl.bin daishuos.img
	$(EDIMG)   imgin:z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:daishuos.img to:@: \
		imgout:daishuos.bin

bin: daishuos.bin
	make daishuos.bin

run: daishuos.bin
	./boot_img.sh

clean: 
	rm -f ./*.bin