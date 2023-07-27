#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#define MAX_X 1500
#define MAX_Y 1200
#define MAXLENGTH  100
#define ARENA_MAX_SIZE 25
#define ESC 27
#define DELTATIME 1.0f/10
#define FOODCOLLECTOFFSET 0.5f
#define FOODSPAWNDELAY 1
#define COLLISION_TOLERANCE 0.5f
// #define SNAKESPEED 60
#include <vector>
#include <string>

extern bool AlreadyUpdate;
extern float cam_x;
extern float cam_z;
extern int FPS;
struct Color
{
	float r;
	float g;
	float b;
	float a;
	Color(float red, float green, float blue, float alfa){
		r = red;
		g = green;
		b = blue;
		a = alfa;
	}
	
};

struct Vector2
{
	float x;
	float y;
	int move;
	
	void Set(float _x, float _y, int _move){
		x = _x;
		y = _y;
		move = _move;
	}
	
	Vector2(){
		x = 0;
		y = 0;
		move = 1;
	}
	
	Vector2(float _x, float _y, int _move){
		x = _x;
		y = _y;
		move = _move;
	}
	
};

struct Obstacle {
	
    Vector2 position;

    Obstacle(float x, float y) {
        position.x = x;
        position.y = y;
    }
    
};

extern std::vector<Obstacle> obstacles;

enum GameState {
	
    MENU,
    JOGANDO
    
};

extern GameState currentGameState;
extern Vector2 SnakePos[MAXLENGTH];
extern Vector2 getSnakeDirection();

class Enemy {
	
public:
    Vector2 position;
    float speed;

Enemy(float x, float y, float speed)
    : speed(speed) {
        position.Set(x, y, 0);
    }
    void moveRandomly(); 
    void detectPlayer(const Vector2& playerPosition);
    
};
extern std::string gameDifficulty;

extern std::vector<Enemy> enemies;
void calcCamRotation();
void drawCube(float x, float y, float z, float w, float h, float d, float size, Color color);
void writeScore(std::string playerName, int score);
std::vector<std::pair<std::string, int> > getHighScores();
void drawHighScores();
bool compareScores(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b);
void Arena();
void Snake();
bool isPositionInObstacle(float x, float y);
void drawMenu();
void checkTailCollision();
void setSnakeInput(Vector2 dir);
void moveSnake();
Vector2 getRandomPosInArena();
void handleFood();
void gameOver(char text[]);
void detectFoodColision();
void Score();
void increaseDifficulty();
void playGameOverSound();
void Ghost();
int checkOverlap(float x, float y, Obstacle obstacles[], int numObstacles);
#endif