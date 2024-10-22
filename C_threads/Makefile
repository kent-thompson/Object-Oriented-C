CC := gcc
CFLAGS = -std=c17 -g -Wall -Werror -Wextra -pedantic -fsanitize=address
LDFLAGS =
TARGET = cthreads
SRCS   = threads.c
OBJECTS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.depends)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

%.depends: %.c
	$(CC) -M $(CFLAGS) $< > $@

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)
