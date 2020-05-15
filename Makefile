CC = clang
RM = rm -f
DFLAG = -g
GFLAGS = -Wall -Wextra
CFLAGS = $(DFLAG) $(GFLAGS) -c
LFLAGS = $(DFLAG) $(GFLAGS)
LIBFLAGS = -pthread

all: TestStack TestBlockingStack

test: test.o
	$(CC) $(LFLAGS) test.o -o test $(LIBFLAGS)

TestStack: TestStack.o Stack.o ListNode.o
	$(CC) $(LFLAGS) TestStack.o ListNode.o Stack.o -o TestStack $(LIBFLAGS) -fsanitize=address

TestBlockingStack: TestBlockingStack.o BlockingStack.o ListNode.o
	$(CC) $(LFLAGS) TestBlockingStack.o BlockingStack.o ListNode.o -o TestBlockingStack $(LIBFLAGS) -fsanitize=address

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

test.o: test.c
	$(CC) $(CFLAGS) test.c -o test.o

clean:
	$(RM) TestStack test TestBlockingStack *.o
