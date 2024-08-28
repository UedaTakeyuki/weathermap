SRCS    = src/main.c src/ipc/ipc.c src/screensize/screensize.c
#EXTPATH = ../getDisplaySize
#EXTOBJS = $(EXTPATH)/displaysize.o
HDRS    = src/ipc/ipc.h src/screensize/screensize.h # $(EXTPATH)/displaysize.h
ALL     = $(SRCS) $(EXTOBJS) $(HDRS) 

weathermap: $(ALL)
	# $@ target name
	# $< the first name of dependency
	# $^ all dependencies of the target
	# https://tex2e.github.io/blog/makefile/automatic-variables
	gcc -g  -pg `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0` -o $@ $(SRCS) $(EXTOBJS) # -I $(EXTPATH)
