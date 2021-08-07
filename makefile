OBJS	= solution.o
SOURCE	= solution.c
HEADER	= 
OUT	= solution
LFLAGS	 = -lpthread -lm -O3

all: solution

solution: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJS) $(OUT)