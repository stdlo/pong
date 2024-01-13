RAYLIB_PATH = C:\raylib\raylib
# CFLAGS = -s -static -Os -std=c99 -Wall -I$(RAYLIB_PATH)\src -Iexternal -DPLATFORM_DESKTOP
CFLAGS = -static -std=c99 -Wall -I$(RAYLIB_PATH)\src -Iexternal -DPLATFORM_DESKTOP
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
SRC = pong.c
TARGET = pong.exe

build: $(SRC)
	cc -o $(TARGET) $(SRC) $(CFLAGS) $(LDFLAGS)

release: $(SRC)
	cc -o $(TARGET) $(SRC) $(CFLAGS) -Wl,--subsystem,windows $(LDFLAGS)

clean:
	rm $(TARGET)

%.exe:
	cc -o $@ $(basename $@).c $(CFLAGS) $(LDFLAGS)

run:
	./$(TARGET)
