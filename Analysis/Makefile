EXEC_BASE=ISO_b
EXEC_MAIN=ISO_m
CC=g++

CFLAGS= -w -Wno-write-strings -O3 -std=c++11 -g

SRCB =                   	\
	../nauty/nauty.c	\
	../nauty/nautil.c	\
	../nauty/naugraph.c	\
	../NautyGraph.cpp	\
	../AutoGraph.cpp	\
	../Isomorphism.cpp	\
	../Timer.cpp		\
	main_b.cpp

OBJB =  ${SRCB:.cpp=.o}

SRCM =                   	\
	../nauty/nauty.c	\
	../nauty/nautil.c	\
	../nauty/naugraph.c	\
	../NautyGraph.cpp	\
	../AutoGraph.cpp	\
	../Isomorphism.cpp	\
	../Timer.cpp		\
	main_m.cpp

OBJM =  ${SRCM:.cpp=.o}

#------------------------------------------------------------

all: ${EXEC_BASE} ${EXEC_MAIN}

base: ${EXEC_BASE}

main: ${EXEC_MAIN}

${EXEC_MAIN}: ${OBJM}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_MAIN} ${OBJM}

${EXEC_BASE}: ${OBJB}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_BASE} ${OBJB}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+

clean:
	rm ${EXEC_BASE} ${EXEC_MAIN} *.o *~ *# -rf
