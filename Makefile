.PONY: build

INITIALIZER=initializer
FINISHER=finisher

build:
	@gcc -o $(INITIALIZER).out $(INITIALIZER).c
	@gcc -o $(FINISHER).out $(FINISHER).c
