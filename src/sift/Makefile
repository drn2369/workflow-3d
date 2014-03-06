CXX = g++

INCLUDE = -I/usr/include

LIB = -L/usr/lib

CXXFLAGS = -Wall -O3 ${INCLUDE}

LDFLAGS = ${LIB} -lsiftgpu

siftGPU: siftGPU.o 
	  ${CXX} ${LDFLAGS} -o ../../bin/$@ $^ ${LDFLAGS}

siftGPU.o: siftGPU.cpp
	    ${CXX} -c ${CXXFLAGS} $<

clean:
	rm siftGPU.o
