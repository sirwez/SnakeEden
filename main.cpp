#include <GL/gl.h>
#include <GL/glut.h>
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include "MMSystem.h"
#include <cmath>
#include <ctime>
int FPS = 0;
int move = 0;
std::string gameDifficulty = "N00B";

bool cameraFollowSnake = false;
void display_callback();
void timer_callback(int);
void reshape_callback(int, int);
void keyboard (unsigned char key, int x, int y);

void init()
{
	glClearColor(1, 1, 1, 1.0);
	glEnable(GL_DEPTH_TEST);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	PlaySound(TEXT("music/theme.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
}

// Função para verificar se um inimigo está sobrepondo um obstáculo
int checkOverlap(float x, float y, std::vector<Obstacle> &obstacles) {
    for (int i = 0; i < obstacles.size(); i++) {
        if (obstacles[i].position.x == x && obstacles[i].position.y == y) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(MAX_X,MAX_Y);
    glutCreateWindow("Snake 3D");
    std::cout << " _______________________________________________________________________________________________________________________\n";
    std::cout << "   _____             _          ______ _____  ______ _   _   _    _ _   _______ _____ __  __       _______ ______ \n";
    std::cout << "  / ____|           | |        |  ____|  __ \\|  ____| \\ | | | |  | | | |__   __|_   _|  \\/  |   /\\|__   __|  ____|\n";
    std::cout << " | (___  _ __   __ _| | _____  | |__  | |  | | |__  |  \\| | | |  | | |    | |    | | | \\  / |  /  \\  | |  | |__   \n";
    std::cout << "  \\___ \\| '_ \\ / _` | |/ / _ \\ |  __| | |  | |  __| | . ` | | |  | | |    | |    | | | |\\/| | / /\\ \\ | |  |  __|  \n";
    std::cout << "  ____) | | | | (_| |   <  __/ | |____| |__| | |____| |\\  | | |__| | |____| |   _| |_| |  | |/ ____ \\| |  | |____ \n";
    std::cout << " |_____/|_| |_|\\__,_|_|\\_\\___| |______|_____/|______|_| \\_|  \\____/|______|_|  |_____|_|  |_/_/    \\_\\_|  |______|\n";
    std::cout << "                                                                                                                  \n";
    std::cout << "                                                                                                                  \n";
    std::cout << "_______________________________________________________________________________________________________________________\n";
    std::cout << " \n";
    std::cout << "------------------------------------------- Welcome to the Snake EDEN 3D - MENU ---------------------------------------\n";
    int numObstacles = 0;
    int numEnemies = 0;
    int userFPS = 5;
    
    int menuOption;
    bool startGame = false;
    
	while (!startGame) {
	    std::cout << "1  - Start Game\n";
	    std::cout << "2 - Game Options\n";
	    std::cout << "3 - Exit\n";
	    std::cin >> menuOption;
	
	    if (menuOption == 1) {
	        startGame = true; // Start the game
	    }
	    else if (menuOption == 2) { // Game options
	        std::cout << "Game Options:\n";
	        std::cout << "1 - Select Difficulty\n";
	        std::cout << "2 - Manually Set Game Parameters\n";
	        int optionChoice;
	        std::cin >> optionChoice;
	        if (optionChoice == 1) {
	            std::cout << "Select the Difficulty Level:\n";
	            std::cout << "1 - N00B\n";
	            std::cout << "2 - Challenge Ahead (Obstacles)\n";
	            std::cout << "3 - Dangerous Territory (Enemies)\n";
	            std::cout << "4 - Hardcore Hustle\n";
	            std::cout << "5 - Infernal Nightmare\n";
	            std::cout << "6 - The Impossible Gauntlet (It's really impossible to win)\n";
	
	            int difficultyChoice;
	            std::cin >> difficultyChoice;
	            switch(difficultyChoice){
	                case 1: // Easy
	                    numObstacles = 0;
	                    numEnemies = 0;
	                    userFPS = 5;
       	   	   	   	    gameDifficulty = "[N00B]";
	                    break;
	                case 2: // Normal (Obstacles)
	                    numObstacles = 5;
	                    numEnemies = 0;
	                    userFPS = 5;
       	   	   	   	    gameDifficulty = "[Challenge_Ahead]";
	                    break;
	                case 3: // Normal (Enemies)
	                    numObstacles = 0;
	                    numEnemies = 5;
	                    userFPS = 5;
	                    gameDifficulty = "[Dangerous_Territory]";
	                    break;
	                case 4: // Hard
	                    numObstacles = 5;
	                    numEnemies = 5;
	                    userFPS = 5;
	                    gameDifficulty = "[Hardcore_Hustle]";
	                    break;
	                case 5: // Infernal
	                    numObstacles = 10;
	                    numEnemies = 15;
	                    userFPS = 5;
	                    gameDifficulty = "[Infernal_Nightmare]";
	                    break;
	                case 6: // Impossible
	                    numObstacles = 25;
	                    numEnemies = 25;
	                    userFPS = 25;
	                    gameDifficulty = "[Impossible_Gauntlet]";
	                    break;
	            }
	        } else if (optionChoice == 2) {
	            std::cout << "Enter the number of obstacles: ";
	            std::cin >> numObstacles;
	            std::cout << "Enter the number of enemies: ";
	            std::cin >> numEnemies;
	            std::cout << "Set the speed of the snake (1-60): ";
	            std::cin >> userFPS;
	        }
	    }
	    else if (menuOption == 3) {
	        return 0; // Exit the program
	    }
	}
	FPS = userFPS;

	// Gerador de obstáculos
	srand(time(NULL));
	for(int i = 0; i < numObstacles; i++){
	    float x = rand() % ((ARENA_MAX_SIZE-1) + 1 - (-ARENA_MAX_SIZE+1)) + (-ARENA_MAX_SIZE+1);
	    float y = rand() % ((ARENA_MAX_SIZE-1) + 1 - (-ARENA_MAX_SIZE+1)) + (-ARENA_MAX_SIZE+1);
	    obstacles.push_back(Obstacle(x, y));
	}
	
	// Gerador de inimigos
	for(int i = 0; i < numEnemies; i++){
	    float x, y;
	    do {
	        x = rand() % ((ARENA_MAX_SIZE-1) + 1 - (-ARENA_MAX_SIZE+1)) + (-ARENA_MAX_SIZE+1);
	        y = rand() % ((ARENA_MAX_SIZE-1) + 1 - (-ARENA_MAX_SIZE+1)) + (-ARENA_MAX_SIZE+1);
	    } while (checkOverlap(x, y, obstacles));
	    enemies.push_back(Enemy(x, y, 0.5f));
	}
    init();
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1000/FPS, timer_callback, 0);
    glutMainLoop();
    return 0;
}

void timer_callback(int){
	
	glutPostRedisplay();
	AlreadyUpdate = true;
	glutTimerFunc(1000/FPS, timer_callback, 0);
	
}

void display_callback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float ratio = MAX_X / MAX_Y;
    glViewport(0, 0, (GLsizei)MAX_X, (GLsizei)MAX_Y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120, ratio, 1, 50);

    Vector2 snakeDirection = getSnakeDirection();

    if (cameraFollowSnake) {
        // câmera na cabeça
        float lookAtX = SnakePos[0].x + snakeDirection.x;
        float lookAtY = SnakePos[0].y + snakeDirection.y;
        gluLookAt(SnakePos[0].x, SnakePos[0].y, 5.0, // aumentar a altura da câmera
                  lookAtX, lookAtY, 1.0,   // inclinar a câmera para baixo
                  0.0, 0.0, 1.0);
    } else {
        // câmera fixa
        gluLookAt(cam_x, cam_z, 20.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
    }

    glMatrixMode(GL_MODELVIEW);

    if (currentGameState == JOGANDO) {
        Arena();
        moveSnake();
        Snake();
        Score();
        handleFood();
    } else if (currentGameState == MENU) {
        drawMenu();
    }    
    glutSwapBuffers();
}



void reshape_callback(int w, int h)
{
	float ratio = w/h;
	glViewport(0, 0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120,ratio,1,50);
    Vector2 snakeDirection = getSnakeDirection();

    if (cameraFollowSnake) {
        // câmera na cabeça
        float lookAtX = SnakePos[0].x + snakeDirection.x;
        float lookAtY = SnakePos[0].y + snakeDirection.y;
        gluLookAt(SnakePos[0].x, SnakePos[0].y, 5.0, // aumentar a altura da câmera
                  lookAtX, lookAtY, 1.0,   // inclinar a câmera para baixo
                  0.0, 0.0, 1.0); // eixo Z é para cima
    } else {
        // câmera fixa
        gluLookAt(cam_x, cam_z, 20.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
    }

    glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y){
    Vector2 dir;
    if (currentGameState == JOGANDO) {
        if (!AlreadyUpdate) { return; }
	switch (key) {
		case 'w': 
			if(move == 2){
				break;
			}
			dir.Set(0, 1, 1);
			move = 1;
			setSnakeInput(dir);
		break;
		case 's': // Mover para baixo
			if(move == 1){
				break;
			}
			dir.Set(0, -1, 2);
			move = 2;
			setSnakeInput(dir);
		break;
		case 'a': // Mover para esquerda
			if(move == 4){
				break;
			}
			dir.Set(-1, 0, 3);
			move = 3;
			setSnakeInput(dir);
		break;
		case 'd': // Mover para direita
			if(move == 3){
				break;
			}
			dir.Set(1, 0, 4);
			move = 4;
			setSnakeInput(dir);
		break;
		case ESC:
			exit(0);
		break;
		case 'i':
			cam_z += 1;
		break;
		case 'k':
			cam_z -= 1;
		break;
		case 'l':
			cam_x += 1;
		break;
		case 'j':
			cam_x -= 1;
		break;
	    case 'c':
 		case 'C':
        cameraFollowSnake = !cameraFollowSnake; // inverte o valor de cameraFollowSnake
        break;

		default:
		break;
	}
}
    else if (currentGameState == MENU) {
        if (key == 's' || key == 'S') {
            // Inicia o jogo se 's' ou 'S' for pressionado
            currentGameState = JOGANDO;
        }
    }
	AlreadyUpdate = false;
}