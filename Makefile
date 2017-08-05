TARGET   = test

MPICC    = mpicc
# compiling flags here
MPI_CFLAGS   = -std=c99 -Wall -I.


SRCDIR   = src
HDRDIR   = inc
BINDIR   = bin

SRCS  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(HDRDIR)/*.h)

MPI_CFLAGS += -I$(SRCDIR) -I$(HDRDIR)

all : test


test: test.c $(SRCS)
	$(MPICC) $^ -g -o $(BINDIR)/$@ $(MPI_LDFLAGS) $(MPI_CFLAGS)

clean:
	rm -rf bin/*