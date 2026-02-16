CXXFLAGS = -Wall -Wextra -std=c++17 \
           $(shell pkg-config --cflags sdl3 sdl3-image)

LDFLAGS = $(shell pkg-config --libs sdl3 sdl3-image)

SRC = main.cpp Window.cpp WindowManager.cpp HelperManager.cpp Component.cpp Render.cpp GlobalBus.cpp NotificationBus.cpp TextureCache.cpp
TARGET = app

all:
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
