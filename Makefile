include $(AGENT_CONFIG)

SRC_DIR = ./src
INC    += -I./src

ifeq ($(G_DEVTOOLS), i386_gcc_linux)
	TARGET = producer-pc 
	liba   = ./rdkafka/i386/lib/librdkafka.a
	INC   += -I./rdkafka/i386/include
	CFLAG  = -g 
endif

ifeq ($(G_DEVTOOLS), arm_gcc_linux)
	TARGET = producer-arm$(VERSION)
	liba   = ./rdkafka/arm/gcc$(VERSION)/lib/librdkafka.a
	INC   += -I./rdkafka/arm/gcc$(VERSION)/include
	CFLAG  = -g
endif

ifeq ($(G_DEVTOOLS), mips_gcc_linux)
	TARGET = producer-mips$(VERSION)
	liba  += ./rdkafka/mips/gcc$(VERSION)/lib/librdkafka.a
	INC   += -I./rdkafka/mips/gcc$(VERSION)/include
	CFLAG  = -g
endif

SRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OBJ := $(patsubst %.c,%.o,$(SRC))

all:$(TARGET) clean

$(TARGET):$(OBJ)
	$(CC) -o $(TARGET) $(LIB) $(OBJ) $(liba) -lpthread -lrt -ldl  

%.o:%.c
	$(CC) -c $(INC) $(CFLAG) $< -o $@

clean:
	rm -rf $(OBJ)
	@echo "$(TARGET) make success!"
	@echo ""
