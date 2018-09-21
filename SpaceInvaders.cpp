#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "gl\glut.h"

using std::cout;
using std::endl;
using std::string;

using namespace std;

ifstream inFile;
int x;

int WIDTH = 1200;
int HEIGHT = 800;
float EARTH_HEIGHT = 100.0;

int ShooterJump = 10;
int BulletJump = 2;
int Shooted = 0;
int CanShoot = 1;

float SHOOTER_SIZE = 60.0;
float BULLET_SIZE = 30.0;
int PosShooterX = WIDTH/2;
int PosBulletY = EARTH_HEIGHT + SHOOTER_SIZE + (BULLET_SIZE / 2);
int PosBulletX = 0;

int NUM_COLORS;
int POINT_SIZE = 7.0;
int colors[10][3];

float enemy01PosX = 0;
float enemy01PosY = 0;

void readColors(){
    inFile.open("colors.txt");
    if(!inFile){
        cout << "Não cconsegui abrir o arquivo das cores";
        exit(1);
    }else{
        cout << "Arquivos das cores aberto" << endl;
    }

    while(inFile >> x){
        NUM_COLORS = x;
        cout << "O numero de cores e: " << NUM_COLORS << endl;

        for(int i = 0; i < NUM_COLORS; i++){
            int id, r, g, b;
            inFile >> id >> r >> g >> b;
            colors[i][0] = r;
            colors[i][1] = g;
            colors[i][2] = b;
            cout << "Cor de ID: " << i << " -> " << colors[i][0] << " " << colors[i][1] << " " << colors[i][2] << endl;
        }
    }

    inFile.close();
}

void drawEnemy01(){
    inFile.open("enemy01.txt");
    if(!inFile){
        cout << "Não cconsegui abrir o arquivo da nave inimiga 01" << endl;
        exit(1);
    }//else{
        //cout << "Arquivo da nave inimiga 01 aberto" << endl;
    //}

    int altura, largura;
    inFile >> altura >> largura;
    //cout << "Dimensoes da nave inimiga 01: " << altura << "x" << largura << endl;

    for(int i = 0; i < altura; i++){ // I e a linha
        for(int j = 0; j < largura; j++){ // J e a coluna
            int color;
            inFile >> color;
            //cout << "[ " << color << " (" << i << ", " << j << ") ]" << endl;

            glColor3f(colors[color-1][0], colors[color-1][1], colors[color-1][2]);

            //enemy01PosX = ((largura - j) * POINT_SIZE);
            enemy01PosX = ((j+1 - largura/2) * POINT_SIZE);
            enemy01PosY = ((altura/2 - i+1) * POINT_SIZE);

            glPointSize(POINT_SIZE);
            glBegin(GL_POINTS);
                glVertex2f(enemy01PosX, enemy01PosY);
            glEnd();
        }
    }


    inFile.close();
}

void drawBullet(){
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glPointSize(BULLET_SIZE);
        glBegin(GL_POINTS);
            glVertex2f(0.0, 0.0);
        glEnd();
    glPopMatrix();
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
    if( (PosShooterX+(SHOOTER_SIZE/2)) >= WIDTH ){
        return;
    } else {
        PosShooterX += ShooterJump;
    }
}

void moveShooterLeft(){
    if( (PosShooterX-(SHOOTER_SIZE/2)) <= 0 ){
        return;
    } else {
        PosShooterX -= ShooterJump;
    }
}

void drawShooter(){
    glColor3f(100.0, 0.0, 0.0);
    glPointSize(SHOOTER_SIZE);
    glBegin(GL_POINTS);
        glVertex2f(0.0, SHOOTER_SIZE/2);
    glEnd();
}

void Animation(){
    if(PosBulletY >= HEIGHT){
        PosBulletY = EARTH_HEIGHT + SHOOTER_SIZE + (BULLET_SIZE / 2);
        CanShoot = 1;
        Shooted = 0;
    }

    if(Shooted){
        PosBulletY += BulletJump;
    }

    glutPostRedisplay();
}

void display( void ){
    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites logicos da area OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0,WIDTH,0,HEIGHT,0,1);

    glBegin(GL_LINES);
        glVertex2f(0.0, 0.0);
        glVertex2f(WIDTH/2, HEIGHT/2);
    glEnd();

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Coloque aqui as chamadas das rotinas que desenha os objetos
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    drawEarth();

    glPushMatrix();
        glTranslatef(WIDTH/2, HEIGHT/2, 0);
        drawEnemy01();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(PosShooterX, EARTH_HEIGHT, 0);
        drawShooter();
    glPopMatrix();

    if(Shooted && CanShoot){
        PosBulletX = PosShooterX;
        CanShoot = 0;
    }
    if (Shooted){
        glPushMatrix();
            glTranslatef(PosBulletX, PosBulletY, 0);
            drawBullet();
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
        //shoot(PosShooterX);
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
    //drawEnemy01();

    readColors();
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
