#include "raylib.h"
#include<bits/stdc++.h>
const int width=600;
const int height=600;
const int cellsize=40;
int highest=0;

struct coords{
    int x,y;
    coords(){x=0;y=0;}
    coords(int x1,int y1){
        x=x1;
        y=y1;
    }
    coords(const coords& c){
        x=c.x;
        y=c.y;
    }
};

struct snake{
    std::deque<coords> body;
    int size;
    Color color;
    char direction='r';
    float timer=0.0f;
    coords currentfood;
    bool isdead=false;

    snake(){}

    coords randomFood(){
        std::vector<coords> available;
        
        for(int x=0; x<width; x+=cellsize){
            for(int y=0; y<height; y+=cellsize){
                bool occupied = false;
                for(auto& seg : body){
                    if(seg.x==x && seg.y==y){
                        occupied = true;
                        break;
                    }
                }
                if(!occupied) available.push_back(coords(x,y));
            }
        }
        
        int index = rand() % available.size();
        DrawRectangle(available[index].x,available[index].y,cellsize-2,cellsize-2,(Color){200,0,50,255});
        return available[index];
    }
    
    void init(){
        coords x(0,0);
        body.clear();
        body.push_front(x);
        size=1;
        currentfood=randomFood();
    }
    
    void draw(){
        for (auto& it:body){
            Rectangle rect = {(float)it.x, (float)it.y, (float)cellsize-2, (float)cellsize-2};
            DrawRectangleRounded(rect, 0.4f, 8, (Color){100,200,200,255});
        }
        Rectangle foodrect = {(float)currentfood.x, (float)currentfood.y, (float)cellsize-2, (float)cellsize-2};
        DrawRectangleRounded(foodrect, 0.4f, 8, (Color){200,0,50,255});
    }

    void input(){
        if(IsKeyPressed(KEY_RIGHT)) direction='r';
        if(IsKeyPressed(KEY_LEFT))  direction='l';
        if(IsKeyPressed(KEY_UP))    direction='u';
        if(IsKeyPressed(KEY_DOWN))  direction='d';
    }
    bool deadend(coords next){
        if(next.x>width || next.y>height || next.x<0 || next.y<0){
            return true;
        }
        for(auto& it:body){
            if(next.x==it.x && next.y==it.y){
                return true;
            }
        }
        return false;
    }

    void move(){
        float interval=0.25f;

        timer+=GetFrameTime();
        if(timer>=interval){
            coords head(body.front());
            coords next;
            switch(direction){
                case 'r': {
                    coords temp((head.x+cellsize),head.y);
                    next=temp;
                    break;
                }
                case 'l': {
                    coords temp(head.x-cellsize,head.y);
                    next=temp;
                    break;
                }
                case 'u': {
                    coords temp(head.x,head.y-cellsize);
                    next=temp;
                    break;
                }
                case 'd': {
                    coords temp(head.x,(head.y+cellsize));
                    next=temp;
                    break;
                }
            }
            if(next.x==currentfood.x && next.y==currentfood.y){
                    body.push_front(next);
                    draw();
                    currentfood=randomFood();
                }
                else{
                    if(deadend(next))isdead=true;
                    body.push_front(next);
                    body.pop_back();
                    draw();
                }
            timer=0.0f;
        }
        size=body.size();
    }
};

bool gameover(int score){
    DrawRectangle(0,0,width,height,BLACK);
    
    DrawText("Game Over!",200,200,40,RED);
    DrawText(TextFormat("Score:%d",score),260,260,20,WHITE);
    DrawText("Press R to restart",160,400,30,WHITE);

    if(IsKeyPressed(KEY_R)){
        return true;
    }
    return false;
}
int main() {
    InitWindow(width, height, "Snake!");
    SetTargetFPS(60);
    snake s;
    s.init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
       for(int x=0;x<width;x+=cellsize){
           for(int y=0;y<height;y+=cellsize){
               DrawRectangle(x,y,cellsize-2,cellsize-2,(Color){60,60,100,255});
           }
       }
        if(!s.isdead){
            s.input();
            s.draw();
            s.move();
        }
        else{
            if(gameover(s.size)){
                s=snake();
                s.init();
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
