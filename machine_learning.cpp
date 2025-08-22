#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#undef CloseWindow

#include "C:/Users/patel/Desktop/PROJECTS/raylib-5.5_win64_mingw-w64/include/raylib.h"
#include "C:/Users/patel/Desktop/PROJECTS/raylib-5.5_win64_mingw-w64/include/raymath.h"
#include "C:/Users/patel/Desktop/PROJECTS/json.hpp"

#include <iostream>
#include <deque>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
using namespace std;
using json = nlohmann::json;

SOCKET sock;

bool initSocket(const char* serverIP, int port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        cout << "WSAStartup failed\n";
        return false;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cout << "Socket creation failed\n";
        return false;
    }

    sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(serverIP);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cout << "Failed to connect to Python server\n";
        return false;
    }

    cout << "Connected to Python server!\n";
    return true;
}

void sendData(std::string data) {
    data.push_back('\n');
    send(sock, data.c_str(), data.size(), 0);
}

std::string recvData() {
    char buffer[2048];
    int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return std::string(buffer);
    }
    return "";
}

int cellsize = 25;
int cellcount = 30;
int offset = 75;

Color green = {173,204,96,255};
Color darkgreen = {43,51,24,255};

double lastupdatedtime = 0;

bool eventtrigger(double interval) {
    double currenttime = GetTime();
    if(currenttime - lastupdatedtime >= interval){
        lastupdatedtime = currenttime;
        return true;
    }
    return false;
}

bool IsElementThere(deque<Vector2> snakebody, Vector2 element) {
    for(unsigned int i=0;i<snakebody.size();i++) {
        if(Vector2Equals(snakebody[i], element)) {
            return true;
        }
    }
    return false;
}

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakebody) {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = genrandpos(snakebody);
    }

    Vector2 genrandcell() {
        float x = GetRandomValue(0, cellcount-1);
        float y = GetRandomValue(0, cellcount-1);
        return Vector2{x,y};
    }

    Vector2 genrandpos(deque<Vector2> snakebody) {
        Vector2 position;
        do {
            position = genrandcell();
        } while(IsElementThere(snakebody, position));
        return position;
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void draw() {
        DrawTexture(texture, offset+ position.x*cellsize, offset+ position.y*cellsize, WHITE);
    }
};

class Snake {
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addseg = false;

    void draw() {
        for(unsigned int i=0;i<body.size();i++) {
            float x=body[i].x;
            float y=body[i].y;
            Rectangle segment = Rectangle{offset+x*cellsize, offset+y*cellsize, (float)cellsize, (float)cellsize};
            DrawRectangleRounded(segment, 0.5, 6, darkgreen);
        }
    }

    void update() {
        body.push_front(Vector2Add(body[0], direction));
        if(addseg==true){
            addseg=false;
        } else {
            body.pop_back();
        }
    }

    void reset() {
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
        addseg = false;
    }
};

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool play = true;
    int score = 0;
    double gametime;
    const double fixedtime=10.0;

    Game() : snake(), food(snake.body) {
        gametime= GetTime();
    }

    void draw() {
        snake.draw();
        food.draw();
    }

    void update() {
        if(play){
            snake.update();
            checkcollision();
            edgecollision();
            selfcollision();
        }
    }

    void GameOver() {
        snake.reset();
        food.position = food.genrandpos(snake.body);
        play = false;
        score = 0;
        gametime=GetTime();

    }

    void checkcollision() {
        if(Vector2Equals(food.position, snake.body[0])) {
            food.position = food.genrandpos(snake.body);
            snake.addseg = true;
            score++;
        }
    }

    void edgecollision() {
        if(snake.body[0].x==cellcount || snake.body[0].x==-1) {
            GameOver();
        }
        if(snake.body[0].y==cellcount || snake.body[0].y==-1) {
            GameOver();
        }
    }

    void selfcollision() {
        deque<Vector2> headtail = snake.body;
        headtail.pop_front();
        if(IsElementThere(headtail, snake.body[0])) {
            GameOver();
        }
    }
};

int main () {
    InitWindow(2*offset + cellsize*cellcount, 2*offset + cellsize*cellcount, "snake_window");
    SetTargetFPS(60);

    if(!initSocket("127.0.0.1", 5000)) {
        cout << "Exiting...\n";
        return -1;
    }

    Game game = Game();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(green);

        if(game.play && GetTime() - game.gametime > game.fixedtime){

            game.GameOver();

        }

        if(eventtrigger(0.2)){
            game.update();

            json state;
            vector<vector<int>> snakePos;
            for(auto &seg : game.snake.body) {
                snakePos.push_back({(int)seg.x, (int)seg.y});
            }
            state["snake"] = snakePos;
            state["food"] = {(int)game.food.position.x, (int)game.food.position.y};
            state["score"] = game.score;
            state["play"] = game.play;
            sendData(state.dump());
            string response = recvData();
            if(!response.empty()) {
                auto action = json::parse(response);
                if(action["action"] == "UP"    && game.snake.direction.y != 1)  game.snake.direction={0,-1};
                if(action["action"] == "DOWN"  && game.snake.direction.y != -1) game.snake.direction={0,1};
                if(action["action"] == "LEFT"  && game.snake.direction.x != 1)  game.snake.direction={-1,0};
                if(action["action"] == "RIGHT" && game.snake.direction.x != -1) game.snake.direction={1,0};
                game.play = true;
            }
        }


        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellsize*cellcount+10, (float)cellsize*cellcount+10}, 5,darkgreen);
        DrawText("Hungry Snake", offset-5, 20,40,darkgreen);
        DrawText(TextFormat("%i",game.score), offset-5, offset+cellsize*cellcount+10,40,darkgreen);

        game.draw();

        EndDrawing();
    }

    game.food.~Food();
    closesocket(sock);
    WSACleanup();
    CloseWindow();
    return 0;
}
