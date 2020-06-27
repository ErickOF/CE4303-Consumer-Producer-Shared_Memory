.PONY: build run clean

INITIALIZER=initializer
FINISHER=finisher
PRODUCER=producer
SEMFLAGS = -pthread -lrt
DEFAULT_BNAME = Default
DEFAULT_BSIZE = 256
DEFAULT_BTIME = 3


build:
	@gcc -o bin/$(INITIALIZER).out $(INITIALIZER).c $(SEMFLAGS)
	@gcc -o bin/$(FINISHER).out $(FINISHER).c $(SEMFLAGS)
	@gcc -o bin/$(PRODUCER).out $(PRODUCER).c $(SEMFLAGS)

test-default:
	@echo "Creating Buffer"
	@bin/${INITIALIZER}.out -bn ${DEFAULT_BNAME} -bs ${DEFAULT_BSIZE}

	@echo "Finisher"
	@bin/${FINISHER}.out -bn ${DEFAULT_BNAME}

test-default-init:
	@echo "Creating Buffer"
	@bin/${INITIALIZER}.out -bn ${DEFAULT_BNAME} -bs ${DEFAULT_BSIZE}

test-default-producer:
	@echo "Producer"
	@bin/${PRODUCER}.out -bn ${DEFAULT_BNAME} -ti ${DEFAULT_BTIME}

test-default-killer:
	@echo "Finisher"
	@bin/${FINISHER}.out -bn ${DEFAULT_BNAME}

clean:
	@rm bin/*.out
