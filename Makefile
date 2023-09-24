weathermap: main.c displaysize.o displaysize.h
	# $@ target name
	# $< the first name of dependency
	# https://tex2e.github.io/blog/makefile/automatic-variables
	gcc -g `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0` -o $@ $< displaysize.o
