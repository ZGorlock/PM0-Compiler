TARGET = pMachine
ODIR = obj
SDIR = src

CC = gcc
CFLAGS = -g -std=c99

HEADERS = $(wildcard $(SDIR)/*.h)
OBJECTS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(wildcard $(SDIR)/*.c))

default: $(TARGET)
all: default

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f pMachine $(ODIR)/*.o *~