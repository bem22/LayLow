# Makefile for Synapse-Shot Camera System (C components)
# Place this Makefile in your project root, alongside the src/ folder

# Compiler & flags\ nCC       := gcc
CFLAGS   := -I/usr/include/libcamera \
             -I/usr/include/libraw \
             -I/usr/include/opencv4 \
             -I/usr/include/bluetooth
LDFLAGS  := -lcamera -lraw -lopencv_core -lopencv_imgproc -lbluetooth

# Source & object files
SRCS     := $(wildcard src/*.c)
OBJS     := $(SRCS:.c=.o)

# Target executable
TARGET   := rem_cam

# Default rule\ nall: $(TARGET)

# Link step\ n$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile C source files into objects\ n%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
