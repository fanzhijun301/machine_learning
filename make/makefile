
CC = g++ 
CFLAGS = -g -Wall 
SRC = ../src/
INCLUDE = -I../include

OBJS = $(SRC)matrix.o
OBJS_PP = $(SRC)utest.o
LIB =  
TARGET = ../bin/regression

$(TARGET) : $(OBJS) $(OBJS_PP)
	$(CC) -o $(TARGET) $(OBJS) $(OBJS_PP) $(LIB)

$(OBJS) : $(SRC)%.o : $(SRC)%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJS_PP) : $(SRC)%.o : $(SRC)%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm $(OBJS) 
	rm $(OBJS_PP)
	rm $(TARGET)

