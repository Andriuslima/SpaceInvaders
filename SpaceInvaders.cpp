#include <iostream>
#include <windows.h>
#include "gl\glut.h"

using std::cout;
using std::endl;

using namespace std;

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

void drawBullet(){
    glPushMatrix();
        glColor3f(0.4, 0.9, 0.3);
        glPointSize(BULLET_SIZE);
        glBegin(GL_POINTS);
            glVertex2f(0.0, 0.0);
        glEnd();
    glPopMatrix();
}

void drawEarth(){
    glPushMatrix();
        glColor3f (0.0, 1.0, 0.0);
        /*
        glBegin(GL_TRIANGLES);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, EARTH_HEIGHT);
            glVertex2f(WIDTH, EARTH_HEIGHT);
        glEnd();
        */
        /*
        glBegin(GL_TRIANGLES);
            glVertex2f(0.0, 0.0);
            glVertex2f(WIDTH, EARTH_HEIGHT);
            glVertex2f(WIDTH, 0.0);
        glEnd();
        */
        glBegin(GL_POLYGON);
            glVertex3f (0.0, 0.0, 0.0);
            glVertex3f (0.0, EARTH_HEIGHT, 0.0);
            glVertex3f (WIDTH, EARTH_HEIGHT, 0.0);
            glVertex3f (WIDTH, 0.0, 0.0);
        glEnd();

    glPopMatrix();

}

//void shoot(int pos){
//    control = 1;
//    glPushMatrix();
//        glTranslatef((float)pos, EARTH_HEIGHT+(SHOOTER_SIZE/2), 0.0);
//        drawBullet();
//    glPopMatrix();
//    cout << "Atira na posicao horizontal: " << pos << endl;
//}

//Aleteração aceleração
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

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Coloque aqui as chamadas das rotinas que desenha os objetos
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    drawEarth();

    glPushMatrix();
        glTranslatef(PosShooterX, EARTH_HEIGHT, 0); // Desenha
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
