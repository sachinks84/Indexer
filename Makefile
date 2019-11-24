CC = gcc

RM = rm -f 
CFLAGS  = -g -fPIC

TARGET = BestFitIndexer.so 
TARGET2 = Indexer.so

SRCS = Indexer_tree.c  BestFitIndexer.c 
SRC2 = Indexer.c

LDFLAGS      = -shared




OBJS = $(SRCS:.c=.o)
OBJ2 = $(SRC2:.c=.o)

all: $(TARGET) $(TARGET2)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) 

$(TARGET2): $(OBJ2)
	$(CC) -o $(TARGET2) $(OBJ2) $(LDFLAGS) 


.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean: 
	$(RM) *.o $(TARGET) $(TARGET2)

.PHONY: check
check: $(TARGET) $(TARGET2) 
	 @python test.py	
