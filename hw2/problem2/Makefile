TARGET = test_random
CC = gcc
LD = gcc

# Add your sources here
SRCS += main.c
SRCS += myrand.c
SRCS +=test_random.c

INCLUDES += -I.

OBJS = $(SRCS:.c=.o)
CFLAGS += -std=c11 # c11 standard
CFLAGS += -O0 # optimization is off
CFLAGS += -fno-common
CFLAGS += -Wall # turn on warnings
CFLAGS += -Wextra # extra warnings
CFLAGS += -pedantic # strict ISO warnings
CFLAGS += -Wno-unused-variable # Do not show unusued warnings
CFLAGS += -Wmissing-include-dirs
CFLAGS += -Wsign-compare
CFLAGS += -Wcast-align
CFLAGS += -Wconversion # neg int const implicitly converted to uint

LDFLAGS += -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	@echo "Successfully finished..."

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(TARGET)
	@rm -rf *.o

.PHONY: all clean
