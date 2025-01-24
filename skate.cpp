#include <GL/glut.h>
#include <math.h>

// Variáveis globais
float posicaoSkatista = 0.0f; // Posição do skatista (0 a 1 normalizada)
float direcao = 1.0f;         // Direção do movimento (+1 ou -1)
float velocidade = 0.005f;    // Velocidade do skatista

// Variáveis para controle da câmera
float cameraAngleY = 1.5f;    // Ângulo de rotação horizontal
float cameraAngleX = 0.0f;    // Ângulo de inclinação vertical
float cameraDistance = 5.0f;  // Distância da câmera ao centro

// Desenha a half-pipe
void desenhaPista() {
    glColor3f(0.0f, 0.0f, 0.0f); // Preto
    glLineWidth(4.0f);
    
    // Laterais e base da pista
    glBegin(GL_LINE_STRIP);
    glVertex2f(-1.5f, 0.0f);
    glVertex2f(-1.8f, 0.0f);
    glVertex2f(-1.8f, -1.15f);
    glVertex2f(1.8f, -1.15f);
    glVertex2f(1.8f, 0.0f);
    glVertex2f(1.5f, 0.0f);
    glEnd();

    // Formato em "U"
    glBegin(GL_LINE_STRIP);
    for (float t = 3.14f; t <= 2.0f * 3.14f; t += 0.01f) {
        float x = cos(t) * 1.5f;
        float y = sin(t);
        glVertex2f(x, y);
    }
    glEnd();
}

// Desenha o skate com rodas e prancha
void desenhaSkate(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y - 0.18f, 0.0f); // Posiciona o skate próximo aos pés do skatista

    // Desenha a prancha do skate
    glColor3f(0.0f, 0.0f, 0.0f); // Preto
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.05f);
    glVertex2f(0.2f, -0.05f);
    glVertex2f(0.2f, -0.08f);
    glVertex2f(-0.2f, -0.08f);
    glEnd();

    // Desenha as rodas do skate
    glColor3f(0.8f, 0.8f, 0.8f); // Cinza claro (rodas)
    for (float offset = -0.15f; offset <= 0.15f; offset += 0.3f) { // Duas rodas
        glPushMatrix();
        glTranslatef(offset, -0.09f, 0.0f); // Ajuste da posição da roda
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 360; i += 10) {
            float theta = i * 3.14159f / 180.0f;
            glVertex2f(0.03f * cos(theta), 0.03f * sin(theta)); // Roda circular
        }
        glEnd();
        glPopMatrix();
    }

    glPopMatrix();
}


// Desenha o skatista dentro da pista
void desenhaSkatista() {
    float x, y;

    // Determina a posição do skatista ao longo da pista
    if (posicaoSkatista < 1.0f) { // Curva
        float t = 3.14f + posicaoSkatista * 3.14f;
        x = cos(t) * 1.5f;
        y = sin(t) + 0.28f;
    }

    // Desenha o skatista
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho (corpo)
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // Corpo
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.0f);
    glVertex2f(0.05f, 0.0f);
    glVertex2f(0.05f, 0.2f);
    glVertex2f(-0.05f, 0.2f);
    glEnd();

    // Cabeça
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(0.05f * cos(theta), 0.25f + 0.05f * sin(theta));
    }
    glEnd();

    // Pernas
    glColor3f(0.0f, 0.0f, 1.0f); // Azul (pernas)
    glBegin(GL_QUADS);
    // Perna esquerda
    glVertex2f(-0.03f, 0.0f);
    glVertex2f(-0.01f, 0.0f);
    glVertex2f(-0.01f, -0.2f);
    glVertex2f(-0.03f, -0.2f);
    // Perna direita
    glVertex2f(0.01f, 0.0f);
    glVertex2f(0.03f, 0.0f);
    glVertex2f(0.03f, -0.2f);
    glVertex2f(0.01f, -0.2f);
    glEnd();

    // Pés
    glColor3f(0.0f, 0.5f, 0.0f); // Verde (pés)
    glBegin(GL_QUADS);
    // Pé esquerdo
    glVertex2f(-0.04f, -0.2f);
    glVertex2f(-0.01f, -0.2f);
    glVertex2f(-0.01f, -0.23f);
    glVertex2f(-0.04f, -0.23f);
    // Pé direito
    glVertex2f(0.01f, -0.2f);
    glVertex2f(0.04f, -0.2f);
    glVertex2f(0.04f, -0.23f);
    glVertex2f(0.01f, -0.23f);
    glEnd();

    glPopMatrix();

    // Desenha o skate abaixo do skatista
    desenhaSkate(x, y);
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define a visão da câmera
    glLoadIdentity();
    float eyeX = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    float eyeY = cameraDistance * sin(cameraAngleX);
    float eyeZ = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
    gluLookAt(eyeX, eyeY, eyeZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    desenhaPista();
    desenhaSkatista();

    glutSwapBuffers();
}

// Atualiza a posição do skatista
void atualiza(int value) {
    posicaoSkatista += direcao * velocidade; // Incrementa ou decrementa a posição
    if (posicaoSkatista > 1.0f) {           // Volta ao início se ultrapassar 1
        posicaoSkatista = 1.0f;
        direcao = -1.0f; // Muda direção
    } else if (posicaoSkatista < 0.0f) {    // Reseta para o fim se ficar negativo
        posicaoSkatista = 0.0f;
        direcao = 1.0f; // Muda direção
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualiza, 0);
}

// Interação via teclado
void teclado(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': // Aumenta a velocidade do skatista
        velocidade += 0.002f;
        break;
    case 's': // Diminui a velocidade do skatista
        if (velocidade > 0.002f) // Evita velocidade negativa
            velocidade -= 0.002f;
        break;
    case 'q': // Tecla ESC para sair
        exit(0);
    }
    glutPostRedisplay();
}

// Interação via teclas de seta para rotacionar a visão
void teclasEspeciais(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: // Inclina a visão para cima
        cameraAngleX += 0.05f;
        if (cameraAngleX > 1.5f) cameraAngleX = 1.5f; // Limita a inclinação
        break;
    case GLUT_KEY_DOWN: // Inclina a visão para baixo
        cameraAngleX -= 0.05f;
        if (cameraAngleX < -1.5f) cameraAngleX = -1.5f;
        break;
    case GLUT_KEY_LEFT: // Rotaciona para a esquerda
        cameraAngleY -= 0.05f;
        break;
    case GLUT_KEY_RIGHT: // Rotaciona para a direita
        cameraAngleY += 0.05f;
        break;
    }
    glutPostRedisplay();
}

// Configuração inicial
void inicializa() {
    glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f); // Azul claro (céu)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 20.0); // Projeção perspectiva
    glMatrixMode(GL_MODELVIEW);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Skatista Interativo na Pista em U");

    inicializa();
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    glutTimerFunc(16, atualiza, 0); // Atualização em 60 FPS
    glutMainLoop();

    return 0;
}