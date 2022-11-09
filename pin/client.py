import zmq
import asyncio

message = ""

async def get_input():
    while True:
        return input()

context = zmq.Context()

print("Connecting to server")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

# Send message
socket.send_string("Connected Client")

# Recieve
message = socket.recv().decode("UTF-8")
print(message)

async def main():
    global message

    # Event loop - CLIENT SENDS FIRST
    while True:
        socket.send_string(message)
        message = await get_input()

        rep = socket.recv().decode("UTF-8")
        if rep != "":
            print(rep)


if __name__ ==  '__main__':
    asyncio.run(main())
