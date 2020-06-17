.PONY: build run

INITIALIZER=initializer
FINISHER=finisher
EXAMPLES=write_read_example
PRODUCER=producer

build:
	@gcc -o $(INITIALIZER).out $(INITIALIZER).c
	@gcc -o $(FINISHER).out $(FINISHER).c
	@gcc -o $(EXAMPLES).out $(EXAMPLES).c
	@gcc -o $(PRODUCER).out $(PRODUCER).c

run:
	@echo "Creating Buffer"
	@./${INITIALIZER}.out

	@echo "Test"
	@./${EXAMPLES}.out

	@echo "Finisher"
	@./${FINISHER}.out
