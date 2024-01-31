SRCS    = main.c ipc.c
EXTOBJS = displaysize.o
HDRS    = ipc.h displaysize.h
ALL     = $(SRCS) $(EXTOBJS) $(HDRS) 

weathermap: $(ALL)
	# $@ target name
	# $< the first name of dependency
	# $^ all dependencies of the target
	# https://tex2e.github.io/blog/makefile/automatic-variables
	gcc -g `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0` -o $@ $(SRCS) $(EXTOBJS)
