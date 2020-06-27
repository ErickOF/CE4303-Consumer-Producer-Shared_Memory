.PONY: build run clean

INITIALIZER=initializer
FINISHER=finisher
PRODUCER=producer
CONSUMER=consumer
SEMFLAGS = -pthread -lrt
DEFAULT_BNAME = Default
DEFAULT_BSIZE = 256
DEFAULT_BTIME = 3
BUILD_DIR = bin


build:
	@if [ ! -d ${BUILD_DIR} ]; then \
		mkdir ${BUILD_DIR}; \
	fi
	@gcc -o ${BUILD_DIR}/$(INITIALIZER).out $(INITIALIZER).c $(SEMFLAGS)
	@gcc -o ${BUILD_DIR}/$(FINISHER).out $(FINISHER).c $(SEMFLAGS)
	@gcc -o ${BUILD_DIR}/$(PRODUCER).out $(PRODUCER).c $(SEMFLAGS)
	@gcc -o ${BUILD_DIR}/$(CONSUMER).out $(CONSUMER).c $(SEMFLAGS)

test-default:
	@echo "Creating Buffer"
	@${BUILD_DIR}/${INITIALIZER}.out -bn ${DEFAULT_BNAME} -bs ${DEFAULT_BSIZE}

	@echo "Finisher"
	@${BUILD_DIR}/${FINISHER}.out -bn ${DEFAULT_BNAME}

test-default-init:
	@echo "Creating Buffer"
	@${BUILD_DIR}/${INITIALIZER}.out -bn ${DEFAULT_BNAME} -bs ${DEFAULT_BSIZE}

test-default-producer:
	@echo "Producer"
	@${BUILD_DIR}/${PRODUCER}.out -bn ${DEFAULT_BNAME} -ti ${DEFAULT_BTIME}

test-default-consumer:
	@echo "Consumer"
	@${BUILD_DIR}/${CONSUMER}.out -bn ${DEFAULT_BNAME} -ti ${DEFAULT_BTIME}

test-default-killer:
	@echo "Finisher"
	@${BUILD_DIR}/${FINISHER}.out -bn ${DEFAULT_BNAME}

clean:
	@rm ${BUILD_DIR}/*.out
