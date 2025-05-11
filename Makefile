CC      = gcc
CFLAGS  = -I/usr/include/libpng \
          -I/usr/include/libcamera \
          -pthread
LDFLAGS = -lraw -lpng -lcamera -lbluetooth -lpthread

# sources
SRC     = src/main.c \
          src/capture.c \
          src/raw2gray.c \
          src/analysis.c \
          src/bt_control.c

TARGET  = rem_cam

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)