SRCS    = src/main.c src/ipc/*.c src/screensize/*.c src/restrictcpu/*.c src/widget/*.c src/webView/*.c
#EXTPATH = ../getDisplaySize
#EXTOBJS = $(EXTPATH)/displaysize.o
HDRS    = src/ipc/*.h src/screensize/*.h src/restrictcpu/*.h src/widget/*.h src/webView/*.h
ALL     = $(SRCS) $(EXTOBJS) $(HDRS) 

weathermap: $(ALL)
	# $@ target name
	# $< the first name of dependency
	# $^ all dependencies of the target
	# https://tex2e.github.io/blog/makefile/automatic-variables
	gcc $(SRCS) $(EXTOBJS) -g  -pg `pkg-config --cflags --libs webkit2gtk-4.0` -o $@ # -I $(EXTPATH)
