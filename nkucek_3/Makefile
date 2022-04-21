COMPILER=g++ -std=c++11
EXECUTABLES=seam

all:
	make $(EXECUTABLES)

seam: seamcarving435.cpp
	$(COMPILER) -o $@ $^

clean:
	rm -f $(EXECUTABLES)
