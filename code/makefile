CC = arm-linux-gcc
CPPFLAGS += -I ./inc
CPPFLAGS += -I ./inc/e_inc
CPPFLAGS += -w
LDFLAGS += -lpthread
LDFLAGS += -L ./lib/ -ljpeg -lpng16 -lz -lapi_v4l2_arm1

SOURCES := $(wildcard ./src/*.c ./*.c)
OBJECTS := $(SOURCES:.c=.o)

main: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ -Wall

%.o: %.c
	@$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf main $(OBJECTS)
