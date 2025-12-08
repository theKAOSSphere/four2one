#!/usr/bin/make -f
# Top-level Makefile for Four2One

all:
	$(MAKE) -C source

install:
	$(MAKE) -C source install

clean:
	$(MAKE) -C source clean

.PHONY: all install clean