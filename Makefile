.PONY: build run clean

INITIALIZER=initializer
FINISHER=finisher
PRODUCER=producer
SEMFLAGS = -pthread -lrt

build:
	@gcc -o bin/$(INITIALIZER).out $(INITIALIZER).c $(SEMFLAGS)
	@gcc -o bin/$(FINISHER).out $(FINISHER).c $(SEMFLAGS)
	@gcc -o bin/$(PRODUCER).out $(PRODUCER).c $(SEMFLAGS)

test:
	@echo "Creating Buffer"
	@bin/${INITIALIZER}.out

	@echo "Finisher"
	@bin/${FINISHER}.out

clean:
	@rm bin/*.out
