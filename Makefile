CC = g++
CFLAGS = -Wall -Wextra -std=gnu++23 $(if $(release),-O3,-g)
INCLUDE = -I/usr/include/SDL2
LDLIBS = -lsndfile -lSDL2 -lSDL2_gfx -lSDL2pp -lportaudio
BIN = bin/audioviz

ifeq ($(FFT),kissfft)
CFLAGS += -DKISSFFT
INCLUDE += -I/usr/local/include/kissfft
LDLIBS += -lkissfft-float-openmp
endif

ifeq ($(FFT),fftw)
CFLAGS += -DFFTW
LDLIBS += -lfftw3
endif

compile:
	mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDE) $(LDLIBS) src/main.cpp -o $(BIN)

run: compile
	bin/audioviz

vg: compile
	valgrind bin/audioviz

gdb: compile
	gdb bin/audioviz

install: compile
	sudo cp $(BIN) /usr/local/bin
