
CC       = gcc
CFLAGS   = -Wall
LDFLAGS  = 
OBJFILES = list.o client.o server.o main.o
TARGET   = program.o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

run: all
	./program.o