#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    float x;
    float y;
    int alive;
} Player;

typedef struct {
    float x;
    float y;
} Monster;

#define PLAYER_RADIUS 0.02f
#define POINT_RADIUS 0.015f
#define MONSTER_RADIUS 0.02f
#define NUM_MONSTERS 6
#define NUM_POINTS 80
#define SURVIVAL_TIME 30 // Tempo de sobrevivência em segundos

Point points[NUM_POINTS];
Player player;
Monster monsters[NUM_MONSTERS];
float monsterSpeed = 0.02f;
int numPoints;
int gameOver = 0;
int showMenu = 1;
int currentMonsterCount = NUM_MONSTERS;
float survivalTimer = 0.0f;

int moveUp = 0;
int moveDown = 0;
int moveLeft = 0;
int moveRight = 0;

// Funções do jogo
int checkCollisionWithPoints(float newX, float newY);
int checkCollisionWithMonsters();
void moveMonsters(int value);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void updatePlayerPosition(int value);
void display();
void mouse(int button, int state, int x, int y);
void initPoints();
void initPlayer();
void initMonsters();
void resetGame();
void drawGameOverScreen();
void drawMenu();
int getTextWidth(const char* text, void* font);
void updateSurvivalTimer(int value);

int checkCollisionWithPoints(float newX, float newY) {
    for (int i = 0; i < numPoints; i++) {
        float dx = newX - points[i].x;
        float dy = newY - points[i].y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < PLAYER_RADIUS + POINT_RADIUS) { // Ajuste de colisão
            return 1;
        }
    }
    return 0;
}

int checkCollisionWithMonsters() {
    for (int i = 0; i < currentMonsterCount; i++) {
        float dx = player.x - monsters[i].x;
        float dy = player.y - monsters[i].y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < PLAYER_RADIUS + MONSTER_RADIUS) {
            return 1;
        }
    }
    return 0;
}

void moveMonsters(int value) {
    if (!showMenu && player.alive) {
        for (int i = 0; i < currentMonsterCount; i++) {
            float dx = (player.x - monsters[i].x);
            float dy = (player.y - monsters[i].y);
            float distance = sqrt(dx * dx + dy * dy);
            if (distance > 0) {
                float newX = monsters[i].x + (dx / distance) * monsterSpeed;
                float newY = monsters[i].y + (dy / distance) * monsterSpeed;
                if (!checkCollisionWithPoints(newX, newY) && !checkCollisionWithPoints(newX, newY)) {
                    monsters[i].x = newX;
                    monsters[i].y = newY;
                }
            }
        }
        if (checkCollisionWithMonsters()) {
            player.alive = 0;
            gameOver = 1;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(100, moveMonsters, 0);
}

void updateSurvivalTimer(int value) {
    if (!showMenu && player.alive) {
        survivalTimer += 0.1f; // Incrementa o tempo de sobrevivência
        if ((int)survivalTimer % 15 == 0 && (int)survivalTimer > 0) {
            monsterSpeed += 0.005f; // Aumenta a velocidade dos monstros de forma gradual a cada 30 segundos
        }
    }
    glutPostRedisplay();
    glutTimerFunc(100, updateSurvivalTimer, 0);
}


void keyboard(unsigned char key, int x, int y) {
    if (showMenu || gameOver) {
        if (key == 'r') {
            resetGame();
            showMenu = 0;
            currentMonsterCount = NUM_MONSTERS;
            survivalTimer = 0.0f;
            monsterSpeed = 0.02f;
        } else if (key == 13) {
            exit(0);
        }
        return;
    }

    switch (key) {
        case 'w': moveUp = 1; break;
        case 's': moveDown = 1; break;
        case 'a': moveLeft = 1; break;
        case 'd': moveRight = 1; break;
    }
}

   


void keyboardUp(unsigned char key, int x, int y) {
    // Desativa a direção quando a tecla é liberada
    switch (key) {
        case 'w': moveUp = 0; break;
        case 's': moveDown = 0; break;
        case 'a': moveLeft = 0; break;
        case 'd': moveRight = 0; break;
    }
}

void updatePlayerPosition(int value) {
    if (player.alive && !showMenu) {
        float newX = player.x;
        float newY = player.y;

        if (moveUp) newY += 0.04f;
        if (moveDown) newY -= 0.04f;
        if (moveLeft) newX -= 0.04f;
        if (moveRight) newX += 0.04f;

        if (newX > 1.0f - PLAYER_RADIUS) newX = 1.0f - PLAYER_RADIUS;
        if (newX < -1.0f + PLAYER_RADIUS) newX = -1.0f + PLAYER_RADIUS;
        if (newY > 1.0f - PLAYER_RADIUS) newY = 1.0f - PLAYER_RADIUS;
        if (newY < -1.0f + PLAYER_RADIUS) newY = -1.0f + PLAYER_RADIUS;

        if (!checkCollisionWithPoints(newX, newY)) {
            player.x = newX;
            player.y = newY;
        }

        glutPostRedisplay();
    }
    glutTimerFunc(16, updatePlayerPosition, 0); // 60 FPS
}
//
void drawGameOverScreen() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.1f, 0.0f);
    //glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Cor de fundo vermelha
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f); // Cor do texto branca
    const char *text = "PERDEU BAHIANO!";  
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    char scoreText[50];
    sprintf(scoreText, "Tempo de sobrevivencia: %.1f segundos", survivalTimer);
    int textWidth = getTextWidth(scoreText, GLUT_BITMAP_HELVETICA_12);
    glRasterPos2f(-((float)textWidth / 800.0f) / 2.0f, -0.1f);
    for (const char *c = scoreText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    const char *restartText = "Pressione 'r' para reiniciar, caso nao queira descansar";
    textWidth = getTextWidth(restartText, GLUT_BITMAP_HELVETICA_12);
    glRasterPos2f(-((float)textWidth / 800.0f) / 2.0f, -0.2f);
    for (const char *c = restartText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void drawMenu() {
    glColor3f(1.0f, 1.0f, 1.0f);

    const char *title = "Echoes of the Void";
    int titleWidth = getTextWidth(title, GLUT_BITMAP_HELVETICA_18);
    glRasterPos2f(-((float)titleWidth / 800.0f) / 2.0f, 0.2f);
    for (const char *c = title; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    const char *startText = "INICIAR JOGO";
    int startTextWidth = getTextWidth(startText, GLUT_BITMAP_HELVETICA_18);
    glRasterPos2f(-((float)startTextWidth / 800.0f) / 2.0f, -0.1f);
    for (const char *c = startText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

int getTextWidth(const char* text, void* font) {
    int width = 0;
    for (const char* c = text; *c != '\0'; c++) {
        width += glutBitmapWidth(font, *c);
    }
    return width;
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && showMenu) {
        showMenu = 0;
        resetGame();
    }
}

void initPoints() {
    // Gera a grade de pontos (parede)
    numPoints = NUM_POINTS;
    for (int i = 0; i < numPoints; i++) {
        points[i].x = -1.0f + ((float)(i % 10) * 0.2f);
        points[i].y = 0.8f - ((float)(i / 10) * 0.2f);
    }
}

void initPlayer() {
    player.x = 0.0f;
    player.y = 0.0f;
    player.alive = 1;
}

void initMonsters() {
    for (int i = 0; i < currentMonsterCount; i++) {
        int validPosition = 0;
        while (!validPosition) {
            monsters[i].x = (rand() % 20 - 10) / 10.0f; // Gera monstros aleatoriamente
            monsters[i].y = (rand() % 20 - 10) / 10.0f;

            validPosition = 1; // Assume que a posição é válida

            // Verifica se a posição gerada colide com algum ponto (bola verde)
            for (int j = 0; j < numPoints; j++) {
                float dx = monsters[i].x - points[j].x;
                float dy = monsters[i].y - points[j].y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < MONSTER_RADIUS + POINT_RADIUS) {
                    validPosition = 0; // Colidiu, então a posição não é válida
                    break;
                }
            }
        }
    }
}


void resetGame() {
    initPlayer();
    initMonsters();
    gameOver = 0;
    survivalTimer = 0.0f;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (showMenu) {
        drawMenu();
    } else {
        glColor3f(0.0f, 1.0f, 0.0f);
        for (int i = 0; i < numPoints; i++) {
            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j < 360; j++) {
                float angle = j * 3.14159f / 180.0f;
                glVertex2f(points[i].x + cos(angle) * POINT_RADIUS, points[i].y + sin(angle) * POINT_RADIUS);
            }
            glEnd();
        }

        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        for (int j = 0; j < 360; j++) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(player.x + cos(angle) * PLAYER_RADIUS, player.y + sin(angle) * PLAYER_RADIUS);
        }
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < currentMonsterCount; i++) {
            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j < 360; j++) {
                float angle = j * 3.14159f / 180.0f;
                glVertex2f(monsters[i].x + cos(angle) * MONSTER_RADIUS, monsters[i].y + sin(angle) * MONSTER_RADIUS);
            }
            glEnd();
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        char timeText[50];
        sprintf(timeText, "Tempo: %.1f s", survivalTimer);
        glRasterPos2f(-0.9f, 0.9f);
        for (const char *c = timeText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }

        if (gameOver) {
            drawGameOverScreen();
        }
    }
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Echoes of the Void");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    initPoints();
    initPlayer();
    initMonsters();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutTimerFunc(100, moveMonsters, 0);
    glutTimerFunc(100, updateSurvivalTimer, 0);
    glutTimerFunc(16, updatePlayerPosition, 0);

    glutMainLoop();
    return 0;
}

