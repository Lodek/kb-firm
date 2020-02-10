TRANSCRIBER = ../../../transcriber/transcriber.py
VCHEF = ../../../firmware/virtual-chef/
WD := $(shell pwd)
OUTPUTS = output-common output-double output-dth output-hold

all: $(OUTPUTS)

$(OUTPUTS): maps.h main
	./main $(subst output,input,$@) > $@

main:
	cd $(VCHEF) && make OUTPUT=$(WD)/main

maps.h: maps/*
	python $(TRANSCRIBER) --noprog maps > $(VCHEF)/maps.h

clean:
	rm output* main
