CXX = cl

CXXFLAGS = /nologo /std:c++20 /EHsc

TARGET = main.exe

SRCS = main.cpp parser.cpp thread.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS)

clean:
	del /q $(TARGET) *.obj
