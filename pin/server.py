import zmq

context = zmq.Context()

print("Connecting to server")
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

# Recieve
message = socket.recv().decode("UTF-8")
print(message)

socket.send_string("Connected Server")

# Event loop - CLIENT SENDS FIRST
while True:
    rep = socket.recv().decode("UTF-8")

    if rep != "":
        print(rep)
        socket.send_string("Recieved")
    else:
        socket.send_string("")
