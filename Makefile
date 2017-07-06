GCCPARAMS = -Iinclude -g

OBJECTS=obj/main.o \
	obj/cpu.o \

obj/%.o: src/%.c
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

all: 8086

8086: $(OBJECTS)
	gcc $(OBJECTS) -o 8086 $(GCCPARAMS)

clean:
	rm -rf 8086 obj
