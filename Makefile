CC      := gcc
CFLAGS  := -I/usr/local/include \
           -I/usr/local/include/libraw \
           -I/usr/include/libpng \
           -I/usr/include/libcamera \
           -pthread
LDFLAGS := -L/usr/local/lib -lraw -lpng -lcamera -lpthread

SRC     := src/main.c \
           src/capture.c \
           src/raw2gray.c \
           src/analysis.c \
           src/datastructures/heap.c

TARGET  := rem_cam

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)