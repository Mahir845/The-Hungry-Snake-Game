<div align="center">
  <h1 align="center">The Hungry Snake Game 🐍🧠</h1>
  <p align="center">
    A classic Snake game powered by a Reinforcement Learning agent!
    <br />
    <a href="https://github.com/Mahir845/The-Hungry-Snake-Game/issues">Report Bug</a>
    ·
    <a href="https://github.com/Mahir845/The-Hungry-Snake-Game/issues">Request Feature</a>
  </p>
</div>

<div align="center">

![Language](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Language](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Forks](https://img.shields.io/github/forks/Mahir845/The-Hungry-Snake-Game?style=for-the-badge&logo=github)
![Stargazers](https://img.shields.io/github/stars/Mahir845/The-Hungry-Snake-Game?style=for-the-badge&logo=github)

</div>

---

### About The Project

This project brings the classic Snake game to life with a modern twist: the snake is controlled not by a human, but by a Reinforcement Learning agent. It serves as a practical demonstration of a client-server architecture where a high-performance game client, built in C++, communicates in real-time with an intelligent Python-based AI server.

The AI observes the game state, makes strategic decisions, and sends commands back to the game, learning and improving its gameplay over time.

---

### 🛠️ Built With

This project was made possible by these incredible technologies:

* **Game Client (Frontend):**
    * ![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
    * **Raylib:** A simple and easy-to-use library to enjoy video games programming.
    * **nlohmann/json:** A robust JSON library for C++.

* **AI Server (Backend):**
    * ![Python](https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white)

* **Communication:**
    * **Winsock API:** For real-time TCP/IP socket communication between the client and server.

---

### 🚀 Getting Started

To get a local copy up and running, follow these simple steps.

#### Prerequisites

Make sure you have the following software installed on your machine:
* A C++ compiler (e.g., MinGW-w64 on Windows)
* Python 3.x
* Git

#### Installation & Setup

1.  **Clone the repository:**
    ```sh
    git clone [https://github.com/Mahir845/The-Hungry-Snake-Game.git](https://github.com/Mahir845/The-Hungry-Snake-Game.git)
    cd The-Hungry-Snake-Game
    ```

2.  **Configure C++ Include Paths:**
    > [!IMPORTANT]
    > You must update the hardcoded paths for `raylib.h` and `json.hpp` in the C++ source files to match the locations on your local machine.

3.  **Compile the C++ Game Client:**
    Use the following command, ensuring you replace the paths to your Raylib library files.
    ```sh
    g++ main.cpp -o snake_game.exe -I"C:\path\to\raylib\include" -L"C:\path\to\raylib\lib" -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32
    ```

---

### 🏃 Usage

To run the application, you must start the server first, followed by the game client.

1.  **Start the Python AI Server:**
    ```sh
    python your_server_script.py
    ```
    *(The server will now be listening for a connection.)*

2.  **Run the C++ Game Client:**
    ```sh
    ./snake_game.exe
    ```
    The game window will launch, and the AI will take control of the snake instantly!

---

### 🙏 Acknowledgements

A big thank you to the creators and maintainers of these amazing tools:
* [Raylib](https://www.raylib.com/)
* [nlohmann/json](https://github.com/nlohmann/json)
* [Shields.io](https://shields.io/) for the awesome badges.
