/*
0: CANNON
1: BULLET
2: LIFE
3.. : ENEMIES
*/

/* PROCESSO PARA INSERIR UM INIMIGO NO JOGO
1) Colocar a leitura do inimigo no readObjects
2) Desenhar o objeto no Display
3) Inserir animacao em Animation
*/

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include "gl\glut.h"
#include <cmath>

using std::cout;
using std::endl;
using std::string;

using namespace std;

ifstream inFile;
int x;

// Referencia para ajudar a usar o array objects
int CANNON = 0;
int BULLET = 1;
int LIFE = 2;
int ENEMY01 = 3;
int ENEMY02 = 4;
int ENEMY03 = 5;
int ENEMY04 = 6;
int NUMBER_OF_ENEMITES = 2; // Numero de inimigos existentes no jogo

int life = 3; // numero de vidas iniciais

int WIDTH = 1200;
int HEIGHT = 800;
float EARTH_HEIGHT = 100.0;
int POINT_SIZE = 7.0;

int ShooterJump = 10;
int BulletJump = 3;
int CanShoot = 1;

int NUM_COLORS;
int colors[10][3];

typedef struct{
    float x, y;
    int altura, largura;
    int form[20][20];
    int visible;
    float radius;
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

float distance(float x1, float y1, float x2, float y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void calculateIntersection(){
    //#####################################################
    //CALCULAR A INTERSECAO DO ATIRADOR COM A VITAMINA AQUI

    for(int i = 3; i < (3+NUMBER_OF_ENEMITES); i++){
        Object enemy = objects[i];
        //So calcula interseccao se o inimigo e o projetil estiverem na tela
        if(enemy.visible && objects[BULLET].visible){
            //r1 = raio do projetil
            float bullet_radius = objects[BULLET].radius;
            float r1 = distance(objects[BULLET].x, objects[BULLET].y, objects[BULLET].x + bullet_radius, objects[BULLET].y + bullet_radius);

            //r2 = radio do inimigo
            float enemy_radius = enemy.radius;
            float r2 = distance(enemy.x, enemy.y, enemy.x + enemy_radius, enemy.y + enemy_radius);

            // distancia do centro do projetil até o centro do inimigo;
            float d1 = distance(objects[BULLET].x, objects[BULLET].y, enemy.x, enemy.y);

            // Soma dos raios
            float d2 = r1 + r2;

            if(d1 <= d2){
                objects[BULLET].visible = 0;
                objects[BULLET].x = objects[CANNON].x;
                objects[BULLET].y = objects[CANNON].y;
                objects[i].visible = 0;
                cout << "Matou o inimigo!" << endl;
                int ALL_DEAD = 1;
                for(int h = 3; h < (3+NUMBER_OF_ENEMITES); h++){
                    if(objects[h].visible) ALL_DEAD = 0;
                }
                if(ALL_DEAD){
                    cout << "Todos os inimigos mortos!" << endl;
                    exit(0);
                }
            }
        }
    }
}

void Animation(){
    if(life == 0){
        cout << "Jogador morreu!" << endl;
        exit(0);
    }

    calculateIntersection();

    if(objects[BULLET].y >= HEIGHT){
        objects[BULLET].y = EARTH_HEIGHT + POINT_SIZE + (POINT_SIZE / 2);
        CanShoot = 1;
        objects[BULLET].visible = 0;
    }

    if(objects[BULLET].visible){
        objects[BULLET].y += BulletJump;
    }

    //////// BEGIN ENEMY 01 ////////
    if(objects[ENEMY01].visible){
        if(objects[ENEMY01].y <= EARTH_HEIGHT){
            objects[ENEMY01].y = HEIGHT;
            life -= 1;
            cout << "Vida: " << life << endl;
        }
        objects[ENEMY01].y -= 0.3;
    }
    //////// END ENEMY 01 /////////

    //////// BEGIN ENEMY 02 ////////
    if(objects[ENEMY02].visible){
        if(objects[ENEMY02].y <= EARTH_HEIGHT){
            objects[ENEMY02].y = HEIGHT;
            life -= 1;
            cout << "Vida: " << life << endl;
        }
        objects[ENEMY02].y -= 0.1;
    }
    //////// END ENEMY 02 /////////

    glutPostRedisplay();
}

void readObjects(){
    int altura, largura;

    //Objects
    Object cannon;
    Object bullet;
    Object enemy01;
    Object enemy02;

    //Cannon properties
    cannon.visible = 1;
    cannon.x = WIDTH/2;;
    cannon.y = EARTH_HEIGHT;

    //Enemy 01 properties
    enemy01.visible = 1;
    enemy01.x = WIDTH/2;
    enemy01.y = HEIGHT;

    //Enemy 02 properties
    enemy02.visible = 1;
    enemy02.x = WIDTH/2 + (100);
    enemy02.y = HEIGHT;


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
    cannon.radius = cannon.largura/2;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            inFile >> cannon.form[i][j];
        }
    }
    objects[CANNON] = cannon;
    inFile.close();
    /////////////////////// END CANNON ///////////////////////

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
    bullet.radius = bullet.largura*2;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            inFile >> bullet.form[i][j];
        }
    }
    objects[BULLET] = bullet;
    inFile.close();
    /////////////////////// END BULLET ///////////////////////

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
    enemy01.radius = enemy01.largura*2;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            inFile >> enemy01.form[i][j];
        }
    }
    objects[ENEMY01] = enemy01;
    inFile.close();
    /////////////////////// END ENEMY 01 ///////////////////////

    /////////////////////// BEGIN ENEMY 02 ///////////////////////
    inFile.open("enemy02.txt");
    if(!inFile){
        cout << "Não consegui abrir o arquivo do inimigo 02" << endl;
        exit(1);
    }else{
        cout << "Arquivo do inimigo 02 aberto" << endl;
    }
    inFile >> altura >> largura;
    enemy02.altura = altura;
    enemy02.largura = largura;
    enemy02.radius = enemy02.largura*2;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            inFile >> enemy02.form[i][j];
        }
    }
    objects[ENEMY02] = enemy02;
    inFile.close();
    /////////////////////// END ENEMY 02 ///////////////////////
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

    if(objects[BULLET].visible && CanShoot){
        objects[BULLET].x = objects[CANNON].x;
        objects[BULLET].visible = 1;
        CanShoot = 0;
    }
    if (objects[BULLET].visible){
        glPushMatrix();
            objects[BULLET].x = objects[CANNON].x;
            glTranslatef(objects[BULLET].x, objects[BULLET].y, 0);
            drawObject(objects[BULLET]);
        glPopMatrix();
    }

    //Draw enemy01
    if(objects[ENEMY01].visible){
        glPushMatrix();
            glTranslatef(objects[ENEMY01].x, objects[ENEMY01].y, 0);
            drawObject(objects[ENEMY01]);
        glPopMatrix();
    }

    //Draw enemy02
    if(objects[ENEMY02].visible){
        glPushMatrix();
            glTranslatef(objects[ENEMY02].x, objects[ENEMY02].y, 0);
            drawObject(objects[ENEMY02]);
        glPopMatrix();
    }

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
        objects[BULLET].visible = 1;
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
