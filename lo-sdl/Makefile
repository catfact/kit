TARGET = lo-sdl

CC = gcc
LD = gcc

CFLAGS = -g -std=gnu11
LDFLAGS = 
LIB = -llo -lSDL2

OBJ_DIR = obj
SRC_DIR = src

_OBJ = main.o \

OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@  

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@ $(LIB)

clean:
	rm $(TARGET)
	rm $(OBJ_DIR)/*.o
