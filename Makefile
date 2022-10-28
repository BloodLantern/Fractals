PROGRAM=app

CC=x86_64-w64-mingw32-gcc
CFLAGS=-Ofast -Wall -Werror
CPPFLAGS=-Iinclude/ #des include externes au dossier
LDFLAGS=-Llibs/x86_64-w64-mingw32
LDLIBS=-lglfw3 -lcanvas -lstdc++ -lgdi32

CSRC=$(wildcard example/*.c)
OBJS=$(CSRC:.c=.o)

DEPS=$(OBJS:.o=.d)

.PHONY: all

all: $(PROGRAM)

-include $(DEPS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(PROGRAM).exe