both: client server

client: p_client.c
	gcc -o client p_client.c -lpthread

server: p_server.c p_connection.c
	gcc -o server p_server.c p_connection.c

pin:
	gcc -o client client.c -lpthread
	gcc -o server p_server.c

clean:
	rm server client
