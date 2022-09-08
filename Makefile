FLAGS = -Wall -g -std=gnu99
SENSOR = socket.o message.o
GATEWAY = socket.o message.o controller.o

all: gateway temperature humidity 

gateway: gateway.o ${GATEWAY}
	gcc ${FLAGS} -o $@ $^

temperature: temperature.o ${SENSOR}
	gcc ${FLAGS} -o $@ $^

humidity: humidity.o ${SENSOR}
	gcc ${FLAGS} -o $@ $^

%.o : %.c 
	gcc ${FLAGS} -c $< $^

gateway.o : gateway.c controller.h socket.h message.h
	gcc ${FLAGS} -c $< 
temperature.o : temperature.c message.h socket.h
	gcc ${FLAGS} -c $< 
humidity.o : humidity.c message.h socket.h
	gcc ${FLAGS} -c $< 
controller.o : controller.c controller.h 
	gcc ${FLAGS} -c $< 
message.o : message.c message.h
	gcc ${FLAGS} -c $< 
socket.o : socket.c socket.h
	gcc ${FLAGS} -c $< 
	
clean: 
	rm -f gateway temperature humidity *.o


