#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
using namespace std;
enum eDir {
    stop = 0,lft = 1,upleft = 2,downleft = 3,rght = 4,upright = 5,downright = 6
};
class Ball{
    private:
        int x, y;
        int originalX, originalY;
        eDir direction;
    public:
        //constructor
        Ball(int posX, int posY) {
            originalX = posX;
            originalY = posY;
            x = posX;
            y = posY;
            direction = stop;
        }
        void reset() {
            x = originalX;
            y = originalY;
            direction = stop;
        }
        void changeDir(eDir d) {
            direction = d;
        }
        //make random move use rand function
        //make sure the random number is not 0, otherwise it stops, tried many times!
        void randomdir() {
            direction = (eDir)((rand() % 6) + 1);
        }
        inline int getX() { return x; }
        inline int getY() { return y; }
        inline eDir getDir() { return direction; }
        void move() {
            switch (direction) {
                case stop:
                    break;
                case lft:
                    x--; break;
                case rght:
                    x++; break;
                case upleft:
                    x--, y--; break;
                case upright:
                    x++, y--; break;
                case downleft:
                    x--, y++; break;
                case downright:
                    x++, y++; break;

            }
        }
        //overload cout function, just like formatting
        friend ostream& operator << (ostream& o, Ball c) {
            o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]" << endl;
        }   
};
class paddle {
    int x, y;
    int originalX, originalY;
public:
    //constructor
    paddle() { x = y = 0; }
    paddle(int posX, int posY): paddle() {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
    }
    void reset() {
        x = originalX;
        y = originalY;
    }
    inline int getX() { return x; }
    inline int getY() { return y; }
    inline void moveUp() { y--; }
    inline void moveDown() { y++; }
    friend ostream& operator << (ostream& o, paddle c) {
        o << "paddle [" << c.x << "," << c.y << "]" << endl;
    }
};
class gameManager{
    private:
        int width, height, scorePlayer1, scorePlayer2;
        bool quit = 1; //quit the game
        char upPlayer1, upPlayer2, downPlayer1, downPlayer2;
        Ball *ball;
        paddle *player1;
        paddle *player2;
    public:
        //constructor
        gameManager(int w, int h){
            //generate different random number, change the seed
            srand(time(NULL));
            upPlayer1 = 'w';
            upPlayer2 = 'i';
            downPlayer1 = 's';
            downPlayer2 = 'k';
            scorePlayer1 = scorePlayer2 = 0;
            width = w;
            height = h;
            ball = new Ball(w / 2, h / 2);
            player1 = new paddle(1, h/2 - 3);
            player2 = new paddle(w -2, h/2 -3);
        }
        //destuctor
        //need to realease the memory other wise would error
        ~gameManager(){
            delete player1, player2, ball;
        }
        void scoreUp(paddle * player){
            if(player == player1)scorePlayer1++;
            if(player == player2)scorePlayer2++;
            ball->reset();
            player1->reset();
            player2->reset();
        }
        void clearScreen(){
            COORD cursorPosition; cursorPosition.X = 0;
            cursorPosition.Y = 0;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
        }
        void draw(){
            clearScreen();
            for(int i = 0; i < width + 2; i++)cout << "__";
            cout << endl;
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    int ballX = ball->getX();
                    int ballY = ball->getY();
                    int player1X = player1->getX();
                    int player2X = player2->getX();
                    int player1Y = player1->getY();
                    int player2Y = player2->getY();
                    if(j == 0) cout << "[]";

                    if(ballX == j && ballY == i)cout << "<>";
                    else if(player1X == j && player1Y == i)cout << "||";
                    else if(player1X == j && player1Y + 1 == i)cout << "||";
                    else if(player1X == j && player1Y + 2== i)cout << "||";
                    else if(player1X == j && player1Y + 3== i)cout << "||";

                    else if(player2X == j && player2Y == i)cout << "||";
                    else if(player2X == j && player2Y + 1 == i)cout << "||";
                    else if(player2X == j && player2Y + 2== i)cout << "||";
                    else if(player2X == j && player2Y + 3== i)cout << "||";
                    else cout << "  ";

                    if(j == width - 1)cout << "[]";
                }
                cout << endl;
            }
            for(int i = 0; i < width + 2; i++)cout << "__";
            cout << endl;
            cout << "Score of Player 1: " << scorePlayer1 << endl << "Score of Player 2: " << scorePlayer2 << endl;
        }
        //where i feel i am making a game
        void input(){
            ball->move();
            int ballX = ball->getX();
            int ballY = ball->getY();
            int player1X = player1->getX();
            int player1Y = player1->getY();
            int player2X = player2->getX();
            int player2Y = player2->getY();
            if (_kbhit()){
                char cur = _getch();
                if(cur == upPlayer1){
                    if(player1Y > 0)player1->moveUp();
                }
                if(cur == upPlayer2){
                    if(player2Y > 0)player2->moveUp();
                }
                if(cur == downPlayer1){
                    if(player1Y + 4 < height)player1->moveDown();
                }
                if(cur == downPlayer2){
                    if(player2Y + 4 < height)player2->moveDown();
                }
                if(ball->getDir() == stop){
                    ball->randomdir();
                }
                if(cur == 'q'){
                    quit = 0;
                }
            }
        }
        void logic(){
            int ballX = ball->getX();
            int ballY = ball->getY();
            int player1X = player1->getX();
            int player1Y = player1->getY();
            int player2X = player2->getX();
            int player2Y = player2->getY();
            //when player 1 hit the ball  -> change the dir of the ball
            //mod 3  + 4, is to ensure that the ball would only go to opposite side
            for(int i = 0; i < 4; i++){
                if(ballX == player1X + 1 && ballY == player1Y + i){
                    ball->changeDir((eDir)(rand() % 3 + 4));
                }
            }
            //for player 2 vice-versa
            for(int i = 0; i < 4; i++){
                if(ballX == player2X - 1 && ballY == player2Y + i){
                    ball->changeDir((eDir)(rand() % 3 + 1));
                }
            }
            //when the ball hits the edge also need to change dir with normal sense 
            if(ballY == height - 1){
                ball->changeDir(ball->getDir() == downright? upright : upleft);
            }
            if(ballY == 1){
                ball->changeDir(ball->getDir() == upright? downright : downleft);
            }
            if(ballX == 0){
                scoreUp(player2);
            }
            if(ballX == width - 1){
                scoreUp(player1);
            }
        }
    void run(){
        while(quit){
            draw();
            input();
            logic();
        }

    }
};
int main()
{
    gameManager test(40, 20);
    test.run();
    return 0;
}