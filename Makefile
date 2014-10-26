#Makefile für vsys Server & Client
#Files erzeugen und löschen

SNAME = server
CNAME = client
SFLAGS = -g -Wall -O -std=c99 -pedantic -D_BSD_SOURCE
CFLAGS = ${SFLAGS}

all : ${SNAME} ${CNAME}

#Server

${SNAME} : ${SNAME}_main.o ${SNAME}_list.o ${SNAME}_get.o ${SNAME}_put.o
	gcc ${SFLAGS} -o vsys_${SNAME} ${SNAME}_main.o ${SNAME}_list.o ${SNAME}_get.o ${SNAME}_put.o

${SNAME}_main.o : ${SNAME}_main.c
	gcc ${SFLAGS} -c ${SNAME}_main.c

${SNAME}_list.o : ${SNAME}_list.c
	gcc ${SFLAGS} -c ${SNAME}_list.c


${SNAME}_get.o : ${SNAME}_get.c
	gcc ${SFLAGS} -c ${SNAME}_get.c


${SNAME}_put.o : ${SNAME}_put.c
	gcc ${SFLAGS} -c ${SNAME}_put.c


#Client


${CNAME} : ${CNAME}_main.o ${CNAME}_list.o ${CNAME}_get.o ${CNAME}_put.o
	gcc ${CFLAGS} -o vsys_${CNAME} ${CNAME}_main.o ${CNAME}_list.o ${CNAME}_get.o ${CNAME}_put.o

${CNAME}_main.o : ${CNAME}_main.c
	gcc ${CFLAGS} -c ${CNAME}_main.c

${CNAME}_list.o : ${CNAME}_list.c
	gcc ${CFLAGS} -c ${CNAME}_list.c

${CNAME}_get.o : ${CNAME}_get.c
	gcc ${CFLAGS} -c ${CNAME}_get.c

${CNAME}_put.o : ${CNAME}_put.c
	gcc ${CFLAGS} -c ${CNAME}_put.c




#Clean all

clean: 
	rm -f vsys_${SNAME} ${SNAME}_main.o ${SNAME}_list.o ${SNAME}_get.o ${SNAME}_put.o vsys_${CNAME}  ${CNAME}_main.o ${CNAME}_list.o ${CNAME}_get.o ${CNAME}_put.o


#(nicht vergessen neue Datein beim erstellen in das Makefile einzubinden!)
