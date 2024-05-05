TARGET = pixgrab

CC     = gcc
INC    = -Isrc
LIBS   = -lX11
CFLAGS = -std=c2x -Wall -Wextra -pedantic

SOURCE_DIR = src
BUILD_DIR  = build

SOURCES = $(shell find $(SOURCE_DIR) -name "*.c")
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
OBJDIRS = $(dir $(OBJECTS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

