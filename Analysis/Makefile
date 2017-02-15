EXEC_BASE=ISO_b
EXEC_MAIN=ISO_m
EXEC_PRE=ISO_p
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

SRCP =                   	\
	../nauty/nauty.c	\
	../nauty/nautil.c	\
	../nauty/naugraph.c	\
	../NautyGraph.cpp	\
	../AutoGraph.cpp	\
	../Isomorphism.cpp	\
	../Timer.cpp		\
	main_p.cpp

OBJP =  ${SRCP:.cpp=.o}


#------------------------------------------------------------

all: ${EXEC_BASE} ${EXEC_MAIN} ${EXEC_PRE}

base: ${EXEC_BASE}

main: ${EXEC_MAIN}

pre: ${EXEC_PRE}

${EXEC_MAIN}: ${OBJM}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_MAIN} ${OBJM}

${EXEC_BASE}: ${OBJB}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_BASE} ${OBJB}

${EXEC_PRE}: ${OBJP}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_PRE} ${OBJP}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+

clean:
	rm ${EXEC_PRE} ${EXEC_BASE} ${EXEC_MAIN} *.o *~ *# -rf
