#****************************************************************************#
#                                                                            #
# 		This is the STOAT-V.0.1 make-file. Just do the               #
#		adjustments and type make.                                   #
#                                                                            #
#								  STOAT 2007 #
#****************************************************************************#

# Compiler and flags
CC = clang++
CFLAGS = -Wall -O -std=c++17 -Wno-deprecated -arch arm64 -stdlib=libc++
LDFLAGS = -stdlib=libc++

# Object files
OBJS = main.o class.o phelp.o in2ch.o intvalue.o  # intvalue.o must be here

# Target executable
TARGET = stoat

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f *.o $(TARGET) *.*~ *~

# Install the executable into the bin directory
install:
	mv ./$(TARGET) ../bin

# Phony targets
.PHONY: all clean install