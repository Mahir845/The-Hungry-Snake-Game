#include <iostream>
#include "C:/Users/patel/Desktop/PROJECTS/raylib-5.5_win64_mingw-w64/include/raylib.h"
#include "C:/Users/patel/Desktop/PROJECTS/raylib-5.5_win64_mingw-w64/include/raymath.h"
#include <deque>

using namespace std;

int cellsize=25;
int cellcount=30;
int offset=75;

Color green={173,204,96,255};
Color darkgreen={43,51,24,255};

double lastupdatedtime= 0;

bool eventtrigger(double interval){
    double currenttime=GetTime();

    if(currenttime-lastupdatedtime>=interval){
        lastupdatedtime=currenttime;
        return true;
    }

    return false;

}

bool IsElementThere(deque<Vector2> snakebody, Vector2 element){

    for(unsigned int i=0;i<snakebody.size();i++){

        if(Vector2Equals(snakebody[i], element)){
            return true;
        }

    }
    return false;

}


class Food{
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakebody){
        Image image=LoadImage("Graphics/food.png");
        texture= LoadTextureFromImage(image);
        UnloadImage(image);
        position=genrandpos(snakebody);
        
    }

    Vector2 genrandcell(){
        float x=GetRandomValue(0,cellcount-1);
        float y=GetRandomValue(0,cellcount-1);
        return Vector2{x,y};

    }

    Vector2 genrandpos(deque<Vector2> snakebody){
        Vector2 position;

        do{

            position=genrandcell();

        }while(IsElementThere(snakebody, position));

        return position;
    }

    ~Food(){
        UnloadTexture(texture);
    }

    void draw(){
        DrawTexture(texture, offset+ position.x*cellsize, offset+ position.y*cellsize,WHITE);
        
    }



};

class Snake{
public:
    deque<Vector2> body={Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction={1,0};
    bool addseg=false;

    void draw(){
    for(unsigned int i=0;i<body.size();i++){
        float x=body[i].x;
        float y=body[i].y;

        Rectangle segment= Rectangle{offset+x*cellsize, offset+y*cellsize, (float)cellsize, (float)cellsize};
        DrawRectangleRounded(segment, 0.5,6, darkgreen);
    }

    
}

    void update(){
        body.push_front(Vector2Add(body[0], direction));

        if(addseg==true){
           addseg=false;
        }
        else{
            body.pop_back();
            
        }
    }

    void reset(){
        body={Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction={1,0};
        addseg=false;
        
    }

};

class Game{
    public:
    Snake snake=Snake();
    Food food=Food(snake.body);
    bool play=true;
    int score=0;

    void draw(){
        snake.draw();
        food.draw();
    }

    void update(){
        if(play){
            snake.update();
            checkcollision();
            edgecollision();
            selfcollision();
        }
    }

    void GameOver(){
        snake.reset();
        food.position=food.genrandpos(snake.body);
        play=false;
        score=0;
    }

    void checkcollision(){
        if(Vector2Equals(food.position, snake.body[0])){

            food.position=food.genrandpos(snake.body);
            snake.addseg=true;
            score++;

        }
    }

    void edgecollision(){
        if(snake.body[0].x==cellcount || snake.body[0].x==-1){

            GameOver();

        }

        if(snake.body[0].y==cellcount || snake.body[0].y==-1){
            GameOver();
        }
    }

    void selfcollision(){
        deque<Vector2> headtail= snake.body;
        headtail.pop_front();

        if(IsElementThere(headtail, snake.body[0])){
            GameOver();
            
        }
    }

};

int main () {
    InitWindow(2*offset + cellsize*cellcount, 2*offset + cellsize*cellcount, "snake_window");
    SetTargetFPS(60);
    

    Game game=Game();
    

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(green);
        
        if(eventtrigger(0.2)){
            game.update();
            
        }

        if(IsKeyPressed(KEY_UP) && game.snake.direction.y!=1){
            game.snake.direction={0,-1};
            game.play=true;
        }

        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y!=-1){
            game.snake.direction={0,1};
            game.play=true;
        }

        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x!=-1){
            game.snake.direction={1,0};
            game.play=true;
        }

        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x!=1){
            game.snake.direction={-1,0};
            game.play=true;
        }

        
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellsize*cellcount+10, (float)cellsize*cellcount+10}, 5,darkgreen);
        DrawText("Hungry Snake", offset-5, 20,40,darkgreen);
        DrawText(TextFormat("%i",game.score), offset-5, offset+cellsize*cellcount+10,40,darkgreen);
        game.draw();
        

        EndDrawing();
    }

    game.food.~Food();  
    CloseWindow();
    return 0;
}
