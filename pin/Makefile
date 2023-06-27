both: client server

client: p_client.c interface.c
	gcc -o client p_client.c interface.c -lpthread -lncurses

server: p_server.c
	gcc -o server p_server.c -lpthread

interface: interface.c
	gcc -o interface interface.c -lncurses

pin:
	gcc -o client client.c -lpthread
	gcc -o server p_server.c -lpthread

clean:
	rm server client
