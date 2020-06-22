.PONY: build run clean

INITIALIZER=initializer
FINISHER=finisher
EXAMPLES=write_read_example
PRODUCER=producer
SEMFLAGS = -pthread -lrt

build:
	@gcc -o bin/$(INITIALIZER).out $(INITIALIZER).c
	@gcc -o bin/$(FINISHER).out $(FINISHER).c
	@gcc -o bin/$(EXAMPLES).out $(EXAMPLES).c
	@gcc -o bin/$(PRODUCER).out $(PRODUCER).c $(SEMFLAGS)

run:
	@echo "Creating Buffer"
	@bin/${INITIALIZER}.out

	@echo "Test"
	@bin/${EXAMPLES}.out

	@echo "Finisher"
	@bin/${FINISHER}.out

clean:
	@rm bin/*.out
