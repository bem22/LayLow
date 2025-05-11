CC       = gcc
CFLAGS   = -I/usr/include/libcamera -I/usr/include/libraw \
           -I/usr/include/opencv4 -I/usr/include/bluetooth -pthread
LDFLAGS  = -lcamera -lraw -lopencv_core -lopencv_imgproc \
           -lbluetooth -lpthread

# All of your .c files
SRC      = src/main.c src/capture.c src/raw2gray.c \
           src/analysis.c src/bt_control.c
TARGET   = rem_cam

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
