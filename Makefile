.PONY: build clean create_buffer create_producer create_consumer kill_buffer restart_buffer 

INITIALIZER=initializer
FINISHER=finisher
PRODUCER=producer
CONSUMER=consumer
SEMFLAGS=-pthread -lrt -lm
DEFAULT_BNAME=Default
DEFAULT_BSIZE=256
DEFAULT_BTIME=3
BUILD_DIR=bin

BUFFER_NAME=test_buffer
BUFFER_SIZE=1024


build:
	@if [ ! -d ${BUILD_DIR} ]; then \
		mkdir ${BUILD_DIR}; \
	fi
	@gcc -o ${BUILD_DIR}/$(INITIALIZER).out $(INITIALIZER).c $(SEMFLAGS)
	@gcc -o ${BUILD_DIR}/$(FINISHER).out $(FINISHER).c $(SEMFLAGS)
	@gcc -o ${BUILD_DIR}/$(PRODUCER).out $(PRODUCER).c $(SEMFLAGS)
	@gcc -o ${BUILD_DIR}/$(CONSUMER).out $(CONSUMER).c $(SEMFLAGS)


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


create_buffer:
	@gcc -o ${BUILD_DIR}/$(INITIALIZER).out $(INITIALIZER).c $(SEMFLAGS)
	@echo "Creating Buffer <${BUFFER_NAME}> and size ${BUFFER_SIZE}"
	@${BUILD_DIR}/${INITIALIZER}.out -bn ${BUFFER_NAME} -bs ${BUFFER_SIZE}

create_producer:
	@gcc -o ${BUILD_DIR}/$(PRODUCER).out $(PRODUCER).c $(SEMFLAGS)
	@echo "Creating producer 0 with mean sleep of 3"
	@${BUILD_DIR}/${PRODUCER}.out -bn ${BUFFER_NAME} -ti 3

create_aconsumer:
	@gcc -o ${BUILD_DIR}/$(CONSUMER).out $(CONSUMER).c $(SEMFLAGS)
	@echo "Creating consumer 0 with mean sleep of 4 and auto mode"
	@${BUILD_DIR}/${CONSUMER}.out -bn ${BUFFER_NAME} -ti 4 -am 1

create_mconsumer:
	@gcc -o ${BUILD_DIR}/$(CONSUMER).out $(CONSUMER).c $(SEMFLAGS)
	@echo "Creating consumer 1 with mean sleep of 1 and manual mode"
	@${BUILD_DIR}/${CONSUMER}.out -bn ${BUFFER_NAME} -ti 1 -am 0

kill_buffer:
	@gcc -o ${BUILD_DIR}/$(FINISHER).out $(FINISHER).c $(SEMFLAGS)
	@echo "Killing <${BUFFER_NAME}> buffer"
	@${BUILD_DIR}/${FINISHER}.out -bn ${BUFFER_NAME}

restart_buffer:
	@make kill_buffer
	@make create_buffer


clean:
	@rm ${BUILD_DIR}/*.out
