import zmq

context = zmq.Context()

print("Connecting to server")
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

# Recieve
message = socket.recv().decode("acii")
print(message)

socket.send_string("Connected Server")

# Event loop - CLIENT SENDS FIRST
while True:
    rep = socket.recv().decode("ascii")

    if rep == "quit\n":
        print("Stopping server")

    else:
        print(rep, end="")
        socket.send_string("Recieved")
