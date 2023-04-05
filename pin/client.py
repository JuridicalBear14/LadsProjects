import zmq
import asyncio

# Initialize server
context = zmq.Context()
print("Connecting to server")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5500")

# Setup user
user = "[PARK]"

message = "Connect"

def main():
    global message

    # Event loop - CLIENT SENDS FIRST
    while True:
        socket.send_string(user + "::" + message)
        message = input()

        rep = socket.recv().decode("ascii")
        if message != "quit":
            print(rep)

        else:
            socket.send_string("quit\n")
            break


if __name__ ==  '__main__':
    main()
