client: client.c
	gcc -o client client.c -lczmq -lpthread

server: p_server.c
	gcc -o server p_server.c -lczmq

demo: server.c
	gcc -o server server.c -lczmq

pin:
	gcc -o client client.c -lczmq -lpthread
	gcc -o server p_server.c -lczmq
