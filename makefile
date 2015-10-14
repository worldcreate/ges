CC = g++
CFLAGS = -O 
LDFLAGS =
INCLUDES = 
LIBS =
DEBUG =
TARGET = main
SRC=$(wildcard *.cpp) 
OBJS = $(SRC:.cpp=.o)
.SUFFIXES:	.cpp
# 生成規則 
all: $(TARGET)

debug: DEBUG=-g -DDEBUG

debug: $(TARGET)

test: 
	./$(TARGET) -T

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(DEBUG) -o $@ $(OBJS) $(LIBS) 
run:
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) .nfs* *~ *.core *.stackdump \#* core 
.cpp.o:
	$(CC) $(CFLAGS) $(DEBUG) $(INCLUDES) -c $< 