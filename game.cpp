#include <GL/glut.h>
#include <GL/gl.h>
#include "game.h"
#include <stdio.h>
#include <windows.h>  
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include "MMSystem.h"
#include <fstream>
#include <algorithm>
#include <vector>

Vector2 Dir;
Vector2 SnakePos[MAXLENGTH] = {Vector2(0, 0, 1)};
bool AlreadyUpdate;

Color SnakeColor = Color(0,1,1,1);
Color SnakeHeadColor = Color(0,0,1,1);
Color WallColor = Color(1,0,0,1);
Color FoodColor = Color(1,1,1,1);
Color Gray = Color(1.5f, .5f, .5f, 1);
Color GrayLess = Color(1.3f, .3f, .3f, 1);

int SnakeSize = 1;

float cam_x;
float cam_z;
float z_offset = 0.01f;
float x_offset = 0.01f;

int Points;
// int snakeMoveFrameDelay;

std::vector<Obstacle> obstacles;
std::vector<Enemy> enemies;

bool FoodSpawned = false;
Vector2 FoodPos;
float FoodAngle;
float FoodSpawnDelay;

Vector2 getSnakeDirection() {
    return Dir;
}

GameState currentGameState = MENU;

void Enemy::moveRandomly() {
    // Gerar uma nova direção aleatória
    float directionX = (rand() % 3 - 1) * speed;
    float directionY = (rand() % 3 - 1) * speed;

    // Calcule a nova posição proposta
    float newX = this->position.x + directionX;
    float newY = this->position.y + directionY;

    // Verifique se a posição proposta está fora da arena
    if (newX < -ARENA_MAX_SIZE || newX > ARENA_MAX_SIZE || newY < -ARENA_MAX_SIZE || newY > ARENA_MAX_SIZE) {
        // Se a posição proposta estiver fora da arena, não mova o inimigo
        return;
    }

    // Se a nova posição estiver dentro de um obstáculo, não mova o inimigo
    if (!isPositionInObstacle(newX, newY)) {
        // Atualize a posição do inimigo
        this->position.x = newX;
        this->position.y = newY;
    }
}


void Enemy::detectPlayer(const Vector2& playerPosition) {
    // verificar se o jogador está próximo
    float dx = playerPosition.x - position.x;
    float dy = playerPosition.y - position.y;
    float distance = sqrt(dx*dx + dy*dy);
    
    // se o jogador está próximo, mover-se em direção ao jogador
    if (distance < 10.0f) {
        position.x += speed * dx / distance;
        position.y += speed * dy / distance;
    }
}

void drawCube(float x, float y, float z, float w, float h, float d, float size, Color color){
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor4f(color.r, color.g, color.b, color.a);
	glScalef(w, h, d);
	glutSolidCube(size);
	glPopMatrix();
}

void Score() {
    std::stringstream ss;
    ss << "Score: " << Points;
    std::string scoreText = ss.str();
    
    float x_position = 20.0f;
    float y_position = 27.0f;

    glRasterPos2f(x_position, y_position);
    for (int i = 0; i < scoreText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }
}

void drawMenu() {

    glColor3f(0.0, 1.0, 0.0);

    std::string titleText = "Snake EDEN ULTIMATE";
    float x_position = -5.0f;
    float y_position = 10.0f;
    glRasterPos2f(x_position, y_position);
    for (int i = 0; i < titleText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, titleText[i]);
    }

    glColor3f(1.0, 1.0, 1.0);

    std::string instructionText = "Pressione 's' para começar!";
    x_position = -5.0f; 
    y_position = 5.0f;
    glRasterPos2f(x_position, y_position);
    for (int i = 0; i < instructionText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, instructionText[i]);
    }
    
	drawHighScores();
	
}

bool compareScores(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
	
    return a.second > b.second;
    
}

void writeScore(std::string playerName, int score) {
	
    std::ofstream file;
    file.open("highscores.txt", std::ios_base::app);
    file << playerName << " " << score << "\n"; 
    file.close();
    
}

std::vector<std::pair<std::string, int> > getHighScores() {
	
    std::ifstream file("highscores.txt");
    std::vector<std::pair<std::string, int> > scores;
    std::string name;
    
    int score;
    
    while (file >> name >> score) {
        scores.push_back(std::make_pair(name, score));
    }
    
    file.close();

    std::sort(scores.begin(), scores.end(), compareScores);

    if (scores.size() > 5) {
        scores.resize(5);
    }
    
    return scores;
    
}


void drawHighScores() {
	
    std::vector<std::pair<std::string, int> > highScores = getHighScores();
    
    float x_position = -4.0f; 
    float y_position = 20.0f;

    for (int i = 0; i < highScores.size(); i++) {
        std::stringstream ss;
        ss << " MVP: " << ": " << highScores[i].first << " " << highScores[i].second <<"###";
        std::string scoreText = ss.str();

        glRasterPos2f(x_position, y_position + i * 20.0f);
        for (int j = 0; j < scoreText.length(); j++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[j]);
        }
    }
    
}

void Ghost() {

    glutSolidSphere(0.5f, 20, 20);
    glPushMatrix();
    glTranslatef(-0.15f, 0.0f, 0.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 0.2f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 0.2f, 10, 10);
    glPopMatrix();
}

void Arena()
{	
	glPushMatrix();
	drawCube(0, ARENA_MAX_SIZE, 0, ARENA_MAX_SIZE * 2, 1, 2, 1, WallColor);
	glRotatef(90, 0, 0, 1);
	drawCube(0, ARENA_MAX_SIZE, 0, ARENA_MAX_SIZE * 2, 1, 2, 1, WallColor);
	glRotatef(90, 0, 0, 1);
	drawCube(0, ARENA_MAX_SIZE, 0, ARENA_MAX_SIZE * 2, 1, 2, 1, WallColor);
	glRotatef(90, 0, 0, 1);
	drawCube(0, ARENA_MAX_SIZE, 0, ARENA_MAX_SIZE * 2, 1, 2, 1, WallColor);
	glPopMatrix();

	glPushMatrix();
	
	for (int x = -ARENA_MAX_SIZE; x < ARENA_MAX_SIZE; x++){
		for (int y = -ARENA_MAX_SIZE; y < ARENA_MAX_SIZE; y++){
		
			drawCube(x, y, -1, 1, 1, 1, 1, (x%2 == 0 && y%2 == 0) ? Gray : GrayLess);
		}
	}
	
	for (std::vector<Obstacle>::const_iterator it = obstacles.begin(); it != obstacles.end(); ++it) {
	    const Obstacle& obstacle = *it;
	    glColor3f(1.0f, 0.0f, 0.0f);
	    glPushMatrix();
	    glTranslatef(obstacle.position.x, obstacle.position.y, 0.0f);
	    glutSolidCube(1.0);
	    glPopMatrix();
	}

	for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
	    Enemy& enemy = *it;
	    enemy.moveRandomly();
	    enemy.detectPlayer(SnakePos[0]);
	}
	
	for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
	    Enemy& enemy = *it;
	    glColor3f(1.0f, 0.0f, 0.0f);
	    glPushMatrix();
	    glTranslatef(enemy.position.x, enemy.position.y, 0.0f);
	    Ghost();
	    glPopMatrix();
	}
	
}

void Snake() {
	
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {0.0f, 0.0f, 10.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glShadeModel(GL_FLAT);

    for (int i = 0; i < SnakeSize; i++) {
        glPushMatrix(); 
        
        glTranslatef(SnakePos[i].x, SnakePos[i].y, 0);

        // Se for a cabeça da cobra, desenhe uma esfera maior
        if (i == 0) {
            glColor3f(SnakeHeadColor.r, SnakeHeadColor.g, SnakeHeadColor.b);
            glutSolidSphere(1.0, 50, 50);


            glColor3f(1.0f, 0.0f, 0.0f);
            // Rotação dos olhos baseada na direção da cobra
            if (Dir.x == 1) { glRotatef(-90, 0, 0, 1); } 
            else if (Dir.x == -1) { glRotatef(90, 0, 0, 1); } 
            else if (Dir.y == 1) { glRotatef(0, 0, 0, 1); } 
            else if (Dir.y == -1) { glRotatef(180, 0, 0, 1); }

			glPushMatrix();
			glTranslatef(0.4, 0.2, 0.8); 
			glutSolidSphere(0.3, 20, 20); // Olho esquerdo
			glTranslatef(-0.6, 0.2, 0.1);
			glutSolidSphere(0.3, 20, 20); // Olho direito
			glPopMatrix();

        }
        // Para outros segmentos
        else {
            glColor3f(SnakeColor.r, SnakeColor.g, SnakeColor.b);
            glPushMatrix(); 
            glScalef(0.8f, 0.8f, 1.5f);
            glutSolidSphere(0.8, 30, 30);
            glPopMatrix();
        }

        glPopMatrix();
    }

    calcCamRotation();
    checkTailCollision();
    
}



void setSnakeInput(Vector2 dir){
	
	Dir = dir;
	SnakePos[0].move = dir.move;
	
}

void handleFood(){
	
	if (!FoodSpawned){
		
		if (FoodSpawnDelay <= 0){
			FoodPos = getRandomPosInArena();
			drawCube(FoodPos.x, FoodPos.y, 0, 1, 1, 1, .7f, FoodColor);
			FoodSpawned = true;
		} else{
			FoodSpawnDelay -= DELTATIME;
		}
		
	} else {
		detectFoodColision();
		glPushMatrix();
		glRotatef(FoodAngle, 1, 1, 0);
		drawCube(FoodPos.x, FoodPos.y, 0, 1, 1, 1, .7f, FoodColor);

		FoodAngle++;
		glPopMatrix();
	}
	
}

void incrementSnake(){
	
	if (SnakeSize < MAXLENGTH){
		SnakeSize++;
	}
	
}

void increaseDifficulty() {
    if (Points % 1 == 0) { 
        FPS++;
    }
}

void detectFoodColision(){
	if (fabs(SnakePos[0].x - FoodPos.x) < FOODCOLLECTOFFSET && fabs(SnakePos[0].x - FoodPos.x) > -FOODCOLLECTOFFSET){
		if (fabs(SnakePos[0].y - FoodPos.y) < FOODCOLLECTOFFSET && fabs(SnakePos[0].y - FoodPos.y) > -FOODCOLLECTOFFSET){
			Points++;
			incrementSnake();
			FoodSpawnDelay = FOODSPAWNDELAY;
			FoodSpawned = false;
			increaseDifficulty();
		}
	}
}

Vector2 getRandomPosInArena(){
	
    Vector2 pos;
    do {
	srand(time(NULL));
	pos.x = rand() % ((ARENA_MAX_SIZE-1) + 1 - (-ARENA_MAX_SIZE+1)) + (-ARENA_MAX_SIZE+1);
	srand(time(NULL));
	pos.y = rand() % ((ARENA_MAX_SIZE-1) + 1 - (-ARENA_MAX_SIZE+1)) + (-ARENA_MAX_SIZE+1);
    } while (isPositionInObstacle(pos.x, pos.y));

    return pos;
    
}

bool isPositionInObstacle(float x, float y) {
	
    const float obstacleRadius = 0.5f;
    
    for (std::vector<Obstacle>::const_iterator it = obstacles.begin(); it != obstacles.end(); ++it) {
    const Obstacle& obstacle = *it;
        if (x >= obstacle.position.x - obstacleRadius && x <= obstacle.position.x + obstacleRadius &&
            y >= obstacle.position.y - obstacleRadius && y <= obstacle.position.y + obstacleRadius) {
            return true;
        }
    }
    return false;
  
}

void gameOver(char text[]){
	
    char buffer [33];
    itoa(Points, buffer, 10);

    PlaySound(TEXT("music/gameover.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
    MessageBox(NULL, text, "Derrota", 0);
    MessageBox(NULL, buffer, "Sua Pontuação: ", 0);
    std::cout << " _______________________________________________________________________________________________________________________\n";
    std::cout << "  /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$$        /$$$$$$  /$$    /$$ /$$$$$$$$ /$$$$$$$ \n";
    std::cout << " /$$__  $$ /$$__  $$| $$$    /$$$| $$_____/       /$$__  $$| $$   | $$| $$_____/| $$__  $$\n";
    std::cout << "| $$  \\__/| $$  \\ $$| $$$$  /$$$$| $$            | $$  \\ $$| $$   | $$| $$      | $$  \\ $$\n";
    std::cout << "| $$ /$$$$| $$$$$$$$| $$ $$/$$ $$| $$$$$         | $$  | $$|  $$ / $$/| $$$$$   | $$$$$$$/\n";
    std::cout << "| $$|_  $$| $$__  $$| $$  $$$| $$| $$__/         | $$  | $$ \\  $$ $$/ | $$__/   | $$__  $$\n";
    std::cout << "| $$  \\ $$| $$  | $$| $$\\  $ | $$| $$            | $$  | $$  \\  $$$/  | $$      | $$  \\ $$\n";
    std::cout << "|  $$$$$$/| $$  | $$| $$ \\/  | $$| $$$$$$$$      |  $$$$$$/   \\  $/   | $$$$$$$$| $$  | $$\n";
    std::cout << " \\______/ |__/  |__/|__/     |__/|________/       \\______/     \\_/    |________/|__/  |__/\n";
    std::cout << "_______________________________________________________________________________________________________________________\n";
    std::cout << "Insira seu nome: ";
    std::string playerName;
    std::cin >> playerName;

    writeScore(playerName, Points);

    std::vector<std::pair<std::string, int> > highScores = getHighScores();

    std::stringstream ss;
    for (int i = 0; i < highScores.size(); i++) {
        ss << "# " << (i+1) << ": " << highScores[i].first << " " << highScores[i].second << "\n";
    }
    
    MessageBox(NULL, ss.str().c_str(), "High Scores", 0);
    exit(0);
}



void calcCamRotation(){
	
	if (cam_z >=2){
		z_offset = -.01f;
	} else if (cam_z <= -2){
		z_offset = .01f;
	}
	
	if (cam_x >=2){
		x_offset = -.01f;
	} else if (cam_x <= -2){
		x_offset = .01f;
	}
	
	cam_z += z_offset;
	cam_x += x_offset;
	
}

void checkOutOfBounds() {
	
    if (SnakePos[0].x >= ARENA_MAX_SIZE || SnakePos[0].x <= -ARENA_MAX_SIZE) {
        gameOver("Colidiu com a parede");
    }
    
    if (SnakePos[0].y >= ARENA_MAX_SIZE || SnakePos[0].y <= -ARENA_MAX_SIZE) {
        gameOver("Colidiu com a parede");
    }
    
    for (std::vector<Obstacle>::const_iterator it = obstacles.begin(); it != obstacles.end(); ++it) {
        const Obstacle& obstacle = *it;
        if (SnakePos[0].x == obstacle.position.x && SnakePos[0].y == obstacle.position.y) {
            gameOver("Você colidiu com um obstáculo!");
            break;
        }
    }
    
    for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
        Enemy& enemy = *it;
        float distance = sqrt(pow(enemy.position.x - SnakePos[0].x, 2) + pow(enemy.position.y - SnakePos[0].y, 2));
        if (distance < COLLISION_TOLERANCE) {
            gameOver("Você foi pego por um inimigo!");
        }
    }
    
}

void checkTailCollision(){
	
	for(int i = SnakeSize-1; i > 0; i--){
		if ((int)SnakePos[0].x == (int)SnakePos[i].x && (int)SnakePos[0].y == (int)SnakePos[i].y){
			gameOver("Colidiu com a própria cauda");
		}
	}
	
}

void moveSnake(){
	for(int i = SnakeSize-1; i > 0; i--){
		SnakePos[i].x = SnakePos[i-1].x;
		SnakePos[i].y = SnakePos[i-1].y;
		SnakePos[i].move = SnakePos[i-1].move;
		
	}
	SnakePos[0].x = round(SnakePos[0].x + (Dir.x));
	SnakePos[0].y = round(SnakePos[0].y + (Dir.y));
	checkOutOfBounds();
}