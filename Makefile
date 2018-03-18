CC=g++
CFLAGS=-c -p -O3 -Wall -std=c++11
LDFLAGS=
SOURCES=main.cpp kor.cpp photo_info.cpp surf.cpp fftm.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=kor.out
#LFLAGS = -I/usr/include -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video
LFLAGS =  `pkg-config --cflags --libs opencv`
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.out
