CC = g++
CFLAGS = -g -fsanitize=address 

main: clean
	$(CC) $(CFLAGS) -o main main.cpp
	$(CC) $(CFLAGS) main.cpp -o main


clean:
	rm -rf main