all: writer reader LEITOR-PAI LEITOR-paralelo2 LEITOR-paralelo3 LEITOR-MONITOR parallelWriter MONITOR

MONITOR: functions.o MONITOR.o
	gcc -pthread -o MONITOR functions.o MONITOR.o
	
MONITOR.o: MONITOR.c functions.h
	gcc -g -c MONITOR.c

LEITOR-MONITOR: functions.o LEITOR-MONITOR.o
	gcc -pthread -o LEITOR-MONITOR functions.o LEITOR-MONITOR.o
	
LEITOR-MONITOR.o: LEITOR-MONITOR.c functions.h
	gcc -g -c LEITOR-MONITOR.c

LEITOR-paralelo3: functions.o LEITOR-paralelo3.o
	gcc -pthread -o LEITOR-paralelo3 functions.o LEITOR-paralelo3.o
	
LEITOR-paralelo3.o: LEITOR-paralelo3.c functions.h
	gcc -g -c LEITOR-paralelo3.c

LEITOR-paralelo2: functions.o LEITOR-paralelo2.o
	gcc -pthread -o LEITOR-paralelo2 functions.o LEITOR-paralelo2.o
	
LEITOR-paralelo2.o: LEITOR-paralelo2.c functions.h
	gcc -g -c LEITOR-paralelo2.c
	
LEITOR-PAI: functions.o LEITOR-PAI.o
	gcc -o LEITOR-PAI functions.o LEITOR-PAI.o
	
LEITOR-PAI.o: LEITOR-PAI.c functions.h
	gcc -g -c LEITOR-PAI.c
	
parallelWriter: functions.o parallelWriter.o
	gcc -pthread -o parallelWriter functions.o parallelWriter.o

parallelWriter.o: parallelWriter.c functions.h
	gcc -g -c parallelWriter.c
	
writer: functions.o writer.o
	gcc -o writer functions.o writer.o

writer.o: writer.c functions.h
	gcc -g -c writer.c

reader: functions.o reader.o
	gcc -o reader functions.o reader.o	
	
reader.o: reader.c functions.h
	gcc -g -c reader.c
	
functions.o: functions.c functions.h
	gcc -g -c functions.c
	
clean: 
	rm -f *.o *.txt *~ writer reader parallelWriter LEITOR-PAI LEITOR-paralelo2 LEITOR-paralelo3 LEITOR-MONITOR MONITOR
cleanFiles:
	rm -f *.o *.txt
