GCC = gcc
OBJECTS = gnb.o
OPTIONS = -c -O2 -W
LDFLAGS = -lm
TARGET = gnb

default : $(OBJECTS)
	$(GCC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o : %.c
	$(GCC) $(OPTIONS) $< $(LDFLAGS)

run : $(OBJECTS) $(TARGET)
	$(GCC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	./$(TARGET)

clean :
	$(RM) *.o
	$(RM) $(TARGET)

