both: p_client p_server

client: p_client.c
	gcc -o client p_client.c -lpthread

server: p_server.c
	gcc -o server p_server.c

pin:
	gcc -o client client.c -lpthread
	gcc -o server p_server.c

clean:
	rm p_server p_client