#Makefile für vsys Server & Client
#Files erzeugen und löschen
#

SNAME = server
CNAME = client
SFLAGS = -g -Wall -O -std=c99 -pedantic -D_BSD_SOURCE 
LDFLAGS = -DLDAP_DEPRECATED -lldap -llber 
CFLAGS = ${SFLAGS}
DIST = vsys_Bogensperger-Andel

all : ${SNAME} ${CNAME}

#Server

${SNAME} : ${SNAME}_main.o ${SNAME}_list.o ${SNAME}_get.o ${SNAME}_put.o ${SNAME}_auth.o ${SNAME}_blacklistfunktions.o 
	gcc ${SFLAGS} -o vsys_${SNAME} ${SNAME}_main.o ${SNAME}_list.o ${SNAME}_get.o ${SNAME}_put.o ${SNAME}_blacklistfunktions.o  ${SNAME}_auth.o ${LDFLAGS}

${SNAME}_main.o : ${SNAME}_main.c
	gcc ${SFLAGS} -c ${SNAME}_main.c

${SNAME}_list.o : ${SNAME}_list.c
	gcc ${SFLAGS} -c ${SNAME}_list.c

${SNAME}_blacklistfunktions.o : ${SNAME}_blacklistfunktions.c
	gcc ${SFLAGS} -c ${SNAME}_blacklistfunktions.c

${SNAME}_auth.o : ${SNAME}_auth.c
	gcc ${SFLAGS} -c ${SNAME}_auth.c ${LDFLAGS}

${SNAME}_get.o : ${SNAME}_get.c
	gcc ${SFLAGS} -c ${SNAME}_get.c

${SNAME}_put.o : ${SNAME}_put.c
	gcc ${SFLAGS} -c ${SNAME}_put.c


#Client


${CNAME} : ${CNAME}_main.o ${CNAME}_list.o ${CNAME}_get.o ${CNAME}_put.o password.o ${CNAME}_auth.o 
	gcc ${CFLAGS} -o vsys_${CNAME} ${CNAME}_main.o ${CNAME}_list.o ${CNAME}_get.o ${CNAME}_put.o password.o ${CNAME}_auth.o ${LDFLAGS}

${CNAME}_main.o : ${CNAME}_main.c
	gcc ${CFLAGS} -c ${CNAME}_main.c

${CNAME}_list.o : ${CNAME}_list.c
	gcc ${CFLAGS} -c ${CNAME}_list.c

${CNAME}_auth.o : ${CNAME}_auth.c
	gcc ${CFLAGS} -c ${CNAME}_auth.c

${CNAME}_get.o : ${CNAME}_get.c
	gcc ${CFLAGS} -c ${CNAME}_get.c

${CNAME}_put.o : ${CNAME}_put.c
	gcc ${CFLAGS} -c ${CNAME}_put.c

password.o : password.c
	gcc ${CFLAGS} -c password.c




#Clean all

clean: 
	rm -f vsys_${SNAME} ${SNAME}_main.o ${SNAME}_list.o ${SNAME}_get.o ${SNAME}_put.o vsys_${CNAME}  ${CNAME}_main.o ${CNAME}_list.o ${CNAME}_get.o ${CNAME}_put.o ${SNAME}_blacklistfunktions.o ${CNAME}_auth.o  ${SNAME}_auth.o password.o

# Dist und Distclean


dist:	
	rm -f -r $(DIST)
	mkdir $(DIST)
	cp ${CNAME}_main.c ${CNAME}.h ${CNAME}_auth.c ${CNAME}_get.c ${CNAME}_list.c ${CNAME}_put.c password.c Dokumentation ${SNAME}_main.c ${SNAME}_auth.c ${SNAME}.h ${SNAME}_blacklistfunktions.c ${SNAME}_get.c ${SNAME}_list.c ${SNAME}_put.c Makefile $(DIST)
	tar cvfz $(DIST).tar.gz $(DIST)


distclean: 
	rm -f -r $(DIST)
	rm -f $(DIST).tar.gz


#(nicht vergessen neue Datein beim erstellen in das Makefile einzubinden!)
