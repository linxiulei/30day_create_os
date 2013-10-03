OBJS_BOOTPACK = bootpack.obj naskfunc.obj fonts.obj \
								graphic.obj dsctbl.obj int.obj \
								fifo.obj debug.obj mouse.obj\
								keyboard.obj

TOOLPATH = z_tools/
INCPATH  = z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del



IPL = ipl.bin

ipl.bin: ipl.nas
	nasm ipl.nas -o ipl.bin

%.gas: %.c
	$(CC1)  $*.c -o $*.gas

%.nas: %.gas
	$(GAS2NASK) $*.gas $*.nas

%.obj: %.nas
	$(NASK) $*.nas $*.obj

fonts.bin : fonts.ascii Makefile
	$(MAKEFONT) fonts.ascii fonts.bin

fonts.obj : fonts.bin Makefile
	$(BIN2OBJ) fonts.bin fonts.obj _fonts

bootpack.bim : $(OBJS_BOOTPACK)
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)

bootpack.hrb: bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

daishu.bin : daishu.nas Makefile
	$(NASK) daishu.nas daishu.bin daishu.lst

daishuos.img: daishu.bin bootpack.hrb
	cat daishu.bin bootpack.hrb > daishuos.img

daishuos.bin: ipl.bin daishuos.img
	$(EDIMG)   imgin:z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:daishuos.img to:@: \
		imgout:daishuos.bin

bin: Makefile
	make daishuos.bin

debug: daishuos.bin
	mv  daishuos.bin bochs.img 
	bochs -qf bochs.conf

run: daishuos.bin
	./boot_img.sh

clean:
	rm -f ./*.bin ./*.img ./*.obj ./*.lst\
		bootpack.nas bootpack.gas bootpack.bim \
		bootpack.s bootpack.map bootpack.hrb\
		ipl naskfunc daishu\
		int.{nas,gas} graphic.{nas,gas} dsctbl.{nas,gas}\
		fifo.{nas,gas} debug.{nas,gas} \
		mouse.{nas,gas} keyboard.{nas,gas}


