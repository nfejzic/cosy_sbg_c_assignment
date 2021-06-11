CFLAGS=-MMD -MP -Wall

# This is used in class
PROGRAMS=programm
OBJECTS=city.o street.o

ALLOBJECTS=$(OBJECTS) $(patsubst %,%.o,$(PROGRAMS))

default: $(PROGRAMS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

programm: programm.o $(OBJECTS)
	$(CC) $(LFLAGS) -o $@ $^

clean:
	-rm -f $(ALLOBJECTS) $(ALLOBJECTS:%.o=%.d) $(PROGRAMS)

-include $(ALLOBJECTS:%.o=%.d)
