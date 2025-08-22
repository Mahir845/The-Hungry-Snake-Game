import socket
import json
import random

def start_server(host="127.0.0.1", port=5000):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((host, port))
    s.listen(1)
    print("Waiting for C++ client...")
    conn, addr = s.accept()
    print("Connected to:", addr)

    actions = ["UP","DOWN","LEFT","RIGHT"]
    buffer = ""

    while True:
        data = conn.recv(2048).decode()
        if not data:
            break

        buffer += data

        while "\n" in buffer:
            line, buffer = buffer.split("\n", 1)
            if not line.strip():
                continue
            try:
                game_state = json.loads(line)
                print("Game state:", game_state)
            except json.JSONDecodeError as e:
                print("JSON decode error:", e, "on line:", line)
                continue
            
            action = {"action": random.choice(actions)}
            conn.sendall((json.dumps(action) + "\n").encode())

    conn.close()

start_server()
