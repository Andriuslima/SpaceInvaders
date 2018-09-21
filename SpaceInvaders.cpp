/*
0: CANNON
1: BULLET
2: LIFE
*: ENEMIES
*/

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include "gl\glut.h"

using std::cout;
using std::endl;
using std::string;

using namespace std;

ifstream inFile;
int x;

int CANNON = 0;
int BULLET = 1;
int LIFE = 2;
int ENEMY01 = 3;
int ENEMY02 = 4;
int ENEMY03 = 5;
int ENEMY04 = 6;

int WIDTH = 1200;
int HEIGHT = 800;
float EARTH_HEIGHT = 100.0;
int POINT_SIZE = 7.0;

int ShooterJump = 10;
int BulletJump = 2;
int Shooted = 0;
int CanShoot = 1;

int NUM_COLORS;
int colors[10][3];

typedef struct{
    int x, y;
    int altura, largura;
    int form[20][20];
    int visible;
    int r;
}Object;

Object objects[7];

void readColors(){
    inFile.open("colors.txt");
    if(!inFile){
        cout << "Não consegui abrir o arquivo das cores";
        exit(1);
    }else{
        cout << "Arquivos das cores aberto" << endl;
    }

    while(inFile >> x){
        NUM_COLORS = x;
        for(int i = 0; i < NUM_COLORS; i++){
            int id, r, g, b;
            inFile >> id >> r >> g >> b;
            colors[i][0] = r;
            colors[i][1] = g;
            colors[i][2] = b;
        }
    }

    inFile.close();
}

void drawObject(Object o){
    int altura = o.altura;
    int largura = o.largura;
    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            int color;
            color = o.form[i][j];
            glColor3f(colors[color-1][0], colors[color-1][1], colors[color-1][2]);

            int x = ((j+1 - largura/2) * POINT_SIZE);
            int y = ((altura - i-1) * POINT_SIZE);

            glPointSize(POINT_SIZE);
            glBegin(GL_POINTS);
                glVertex2f(x, y);
            glEnd();
        }
    }
}

void drawEarth(){
    glPushMatrix();
        glColor3f (0.0, 1.0, 0.0);
        glBegin(GL_POLYGON);
            glVertex3f (0.0, 0.0, 0.0);
            glVertex3f (0.0, EARTH_HEIGHT, 0.0);
            glVertex3f (WIDTH, EARTH_HEIGHT, 0.0);
            glVertex3f (WIDTH, 0.0, 0.0);
        glEnd();

    glPopMatrix();

}

void moveShooterRight(){
    if( (objects[CANNON].x + (POINT_SIZE/2)) >= WIDTH ){
        return;
    } else {
        objects[CANNON].x += ShooterJump;
    }
}

void moveShooterLeft(){
    if( (objects[CANNON].x - (POINT_SIZE/2)) <= 0 ){
        return;
    } else {
        objects[CANNON].x -= ShooterJump;
    }
}

void Animation(){
    if(objects[BULLET].y >= HEIGHT){
        objects[BULLET].y = EARTH_HEIGHT + POINT_SIZE + (POINT_SIZE / 2);
        CanShoot = 1;
        Shooted = 0;
    }

    if(Shooted){
        objects[BULLET].y += BulletJump;
    }

    glutPostRedisplay();
}

void readObjects(){
    int altura, largura;

    //Objects
    Object cannon;
    Object enemy01;
    Object bullet;

    //Cannon properties
    cannon.visible = 1;
    cannon.x = WIDTH/2;;
    cannon.y = EARTH_HEIGHT;

    //Enemy 01 properties
    enemy01.visible = 0;
    enemy01.x = WIDTH/2;
    enemy01.y = HEIGHT/2;

    //Bullet properties
    bullet.visible = 0;
    bullet.x = cannon.x;
    bullet.y = cannon.y;

    /////////////////////// BEGIN CANNON ///////////////////////
    inFile.open("cannon.txt");
    if(!inFile){
        cout << "Não consegui abrir o arquivo do atirador" << endl;
        exit(1);
    }else{
        cout << "Arquivo do atirador aberto" << endl;
    }
    inFile >> altura >> largura;
    cannon.altura = altura;
    cannon.largura = largura;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            inFile >> cannon.form[i][j];
        }
    }
    objects[CANNON] = cannon;
    inFile.close();
    /////////////////////// END CANNON ///////////////////////

    /////////////////////// BEGIN ENEMY 01 ///////////////////////
    inFile.open("enemy01.txt");
    if(!inFile){
        cout << "Não consegui abrir o arquivo do inimigo 01" << endl;
        exit(1);
    }else{
        cout << "Arquivo do inimigo 01 aberto" << endl;
    }
    inFile >> altura >> largura;
    enemy01.altura = altura;
    enemy01.largura = largura;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            inFile >> enemy01.form[i][j];
        }
    }
    objects[ENEMY01] = enemy01;
    inFile.close();
    /////////////////////// END ENEMY 01 ///////////////////////

    /////////////////////// BEGIN BULLET ///////////////////////
    inFile.open("bullet.txt");
    if(!inFile){
        cout << "Não consegui abrir o arquivo do projetil" << endl;
        exit(1);
    }else{
        cout << "Arquivo do projetil aberto" << endl;
    }
    inFile >> altura >> largura;
    bullet.altura = altura;
    bullet.largura = largura;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            inFile >> bullet.form[i][j];
        }
    }
    objects[BULLET] = bullet;
    inFile.close();
    /////////////////////// END ENEMY 01 ///////////////////////
}

void display( void ){
    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites logicos da area OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0,WIDTH,0,HEIGHT,0,1);

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Coloque aqui as chamadas das rotinas que desenha os objetos
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    drawEarth();

    // Draw Cannon
    glPushMatrix();
        glTranslatef(objects[CANNON].x, objects[CANNON].y, 0);
        drawObject(objects[CANNON]);
    glPopMatrix();

    if(Shooted && CanShoot){
        objects[BULLET].x = objects[CANNON].x;
        objects[BULLET].visible = 1;
        CanShoot = 0;
    }
    if (Shooted){
        glPushMatrix();
            glTranslatef(objects[BULLET].x, objects[BULLET].y, 0);
            drawObject(objects[BULLET]);
        glPopMatrix();
    }

    //Draw enemy01
    glPushMatrix();
        glTranslatef(objects[ENEMY01].x, objects[ENEMY01].y, 0);
        drawObject(objects[ENEMY01]);
    glPopMatrix();

    glutSwapBuffers();
}

void arrow_keys(int a_keys, int x, int y){
    switch(a_keys){
    case GLUT_KEY_UP: // Se pressionar UP vai para Full Screen
        glutFullScreen ( );
        break;
    case GLUT_KEY_DOWN: //Se pressionar DOWN reposiciona a janela
        glutPositionWindow (50,50);
        glutReshapeWindow ( 700, 500 );
        break;
    case GLUT_KEY_RIGHT: // Se pressionar RIGHT move o atirador para direita
        moveShooterRight();
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT: // Se pressionar LEFT move o atirador para esquerda
        moveShooterLeft();
        Animation();
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void keyboard(unsigned char key, int x, int y){
    switch ( key ){
    case 27: //Termina o programa qdo a tecla ESC for pressionada
        exit ( 0 );
        break;
    case ' ': // Atira
        Shooted = 1;
        glutPostRedisplay();
    default:
        break;
    }
}

void reshape(int w, int h){
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a �rea a ser ocupada pela �rea OpenGL dentro da Janela
    glViewport(0, 0, w, h);

    // Define os limites l�gicos da �rea OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0,WIDTH,0,HEIGHT,0,1);
}

void init(void){
    // Define a cor do fundo da tela
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    readColors();
    readObjects();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition(0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize(WIDTH, HEIGHT);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de t�tulo da janela.
    glutCreateWindow("Space Invaders - A New Experience");

    // executa algumas inicializacoes
    init();


    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // sera chamada automaticamente quando
    // for necess�rio redesenhar a janela
    glutDisplayFunc(display);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser� chamada automaticamente quando
    // o usu�rio alterar o tamanho da janela


    glutReshapeFunc(reshape);//Não ta funcionando

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser� chamada automaticamente sempre
    // o usu�rio pressionar uma tecla comum
    glutKeyboardFunc(keyboard);

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser� chamada
    // automaticamente sempre o usu�rio
    // pressionar uma tecla especial
    glutSpecialFunc(arrow_keys);

    glutIdleFunc(Animation);

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
