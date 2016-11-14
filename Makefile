EXEC_NAME=ISO
CC=g++

CFLAGS= -w -Wno-write-strings -O3 -std=c++11 -g

SRC =                   	\
	nauty/nauty.c		\
	nauty/nautil.c		\
	nauty/naugraph.c	\
	NautyGraph.cpp		\
	Isomorphism.cpp		\
	main.cpp

OBJ =  ${SRC:.cpp=.o}

#------------------------------------------------------------

all: ${EXEC_NAME}

${EXEC_NAME}: ${OBJ}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_NAME} ${OBJ}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+

clean:
	rm ${EXEC_NAME} *.o *~ *# -rf
