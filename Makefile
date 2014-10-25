#Makefile für vsys Server & Client
#Files erzeugen und löschen

SNAME = server
CNAME = client
SFLAGS = -g -Wall -O -std=c99
CFLAGS = ${SFLAGS}

all : ${SNAME} ${CNAME}

${SNAME} : ${SNAME}_main.o
	gcc ${SFLAGS} -o vsys_${SNAME} ${SNAME}_main.o

${SNAME}_main.o : ${SNAME}_main.c
	gcc ${SFLAGS} -c ${SNAME}_main.c

${CNAME} : ${CNAME}_main.o
	gcc ${CFLAGS} -o vsys_${CNAME} ${CNAME}_main.o

${CNAME}_main.o : ${CNAME}_main.c
	gcc ${CFLAGS} -c ${CNAME}_main.c

clean: 
	rm -f vsys_${SNAME} vsys_${CNAME} ${SNAME}_main.o ${CNAME}_main.o
