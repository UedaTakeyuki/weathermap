weathermap: main.c ipc.c ipc.h displaysize.o displaysize.h
	# $@ target name
	# $< the first name of dependency
	# $^ all dependencies of the target
	# https://tex2e.github.io/blog/makefile/automatic-variables
	#gcc -g `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0` -o $@ $^ #displaysize.o
	gcc -g `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0` -o $@ main.c ipc.c displaysize.o
