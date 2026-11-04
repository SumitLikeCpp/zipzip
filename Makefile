flags := -Wall -I. -std=c23
ldflags := 

.PHONY: clean

all: zz

zz: zipzip.o
	cc $(flags) $^ -o $@ $(ldflags)

zipzip.o: zipzip.c
	cc $(flags) -c $^

clean:
	rm -rf zz *.o