# QNX SDP 8.0 AARCH64 Build

CC = qcc
CFLAGS = -Wall -O2 -Vgcc_ntoaarch64le
LDFLAGS = -lsocket

TARGET = rpm_server
SRC = src/rpm_server.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
