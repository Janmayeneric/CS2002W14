CC = clang
RM = rm -f
DFLAG = -g
GFLAGS = -Wall -Wextra
CFLAGS = $(DFLAG) $(GFLAGS) -c
LFLAGS = $(DFLAG) $(GFLAGS)
LIBFLAGS = -pthread

all: TestStack TestBlockingStack

TestStack: TestStack.o Stack.o ListNode.o
	$(CC) $(LFLAGS) TestStack.o ListNode.o Stack.o -o TestStack $(LIBFLAGS)

TestBlockingStack: TestBlockingStack.o BlockingStack.o ListNode.o
	$(CC) $(LFLAGS) TestBlockingStack.o BlockingStack.o ListNode.o -o TestBlockingStack $(LIBFLAGS)

ListNode.o: ListNode.c ListNode.h
	$(CC) $(CFLAGS) ListNode.c -o ListNode.o

Stack.o: Stack.c Stack.h
	$(CC) $(CFLAGS) Stack.c -o Stack.o

TestStack.o: TestStack.c myassert.h
	$(CC) $(CFLAGS) TestStack.c -o TestStack.o

BlockingStack.o: BlockingStack.c BlockingStack.h
	$(CC) $(CFLAGS) BlockingStack.c

TestBlockingStack.o: TestBlockingStack.c myassert.h
	$(CC) $(CFLAGS) TestBlockingStack.c

clean:
	$(RM) TestStack TestBlockingStack *.o
