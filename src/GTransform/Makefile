CC=gcc
OPTFLAGS= -Wall -g

NM=GTransform

INCLUDE_PATH=  -I/usr/include -I/usr/include/opencv -I/home/drn2369/usr/include
LIB_PATH=  -L/usr/lib

CPPFLAGS=$(OPTFLAGS) $(INCLUDE_PATH)


NM_OBJS= main.o positions.o transform.o IO.o utils.o
NM_LIBS=  `pkg-config --libs opencv`


all: $(NM) 
%.o : %.cpp
	$(CXX) -c  $(CPPFLAGS) $<

$(NM): $(NM_OBJS)
	$(CXX) -o ../../bin/$@ $(CPPFLAGS) $(LIB_PATH) $(NM_OBJS) $(NM_LIBS)


clean:
	rm -f *.o *~ $(NM)
