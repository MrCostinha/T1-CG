#include <GL/glut.h>
#include <math.h>

// Variáveis globais
float posicaoSkatista = 0.0f; // Posição do skatista (0 a 1 normalizada)
float direcao = 1.0f;         // Direção do movimento (+1 ou -1)
float velocidade = 0.009f;    // Velocidade do skatista

// Variáveis para controle da câmera
float cameraAngleY = 1.5f;    // Ângulo de rotação horizontal
float cameraAngleX = 0.0f;    // Ângulo de inclinação vertical
float cameraDistance = 5.0f;  // Distância da câmera ao centro

// Calcula o ângulo de inclinação da curva
float calculaAngulo(float t) {
    float dx = -sin(t) * 1.5f; // Derivada de x = cos(t) * 1.5
    float dy = cos(t);         // Derivada de y = sin(t)
    return atan2(dy, dx) * 180.0f / 3.14159f; // Ângulo em graus
}

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

// Desenha o skatista dentro da pista
void desenhaSkatista() {
    float x, y, angulo;

    // Determina a posição do skatista ao longo da pista
    float t = 3.5f + posicaoSkatista * 2.5f;    // Mapeia posição para a curva
    x = cos(t) * 1.31f;                         // Calcula coordenada x
    y = sin(t) + 0.24f;                         // Calcula coordenada y
    angulo = calculaAngulo(t);                  // Calcula o ângulo de inclinação

    // Desenha o skatista com inclinação
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(angulo, 0.0f, 0.0f, 1.0f);    // Inclina o skatista conforme o ângulo

    // Corpo do skatista
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.0f);
    glVertex2f(0.05f, 0.0f);
    glVertex2f(0.05f, 0.2f);
    glVertex2f(-0.05f, 0.2f);
    glEnd();

    // Cabeça do skatista
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(0.05f * cos(theta), 0.25f + 0.05f * sin(theta));
    }
    glEnd();

    // Pernas do skatista
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glBegin(GL_QUADS);
    // Perna esquerda
    glVertex2f(-0.03f, 0.0f);
    glVertex2f(-0.01f, 0.0f);
    glVertex2f(-0.01f, -0.1f);
    glVertex2f(-0.03f, -0.1f);
    // Perna direita
    glVertex2f(0.01f, 0.0f);
    glVertex2f(0.03f, 0.0f);
    glVertex2f(0.03f, -0.1f);
    glVertex2f(0.01f, -0.1f);
    glEnd();

    // Pés do skatista
    glColor3f(0.3f, 0.3f, 0.3f); // Cinza escuro
    glBegin(GL_QUADS);
    // Pé esquerdo
    glVertex2f(-0.04f, -0.1f);
    glVertex2f(-0.01f, -0.1f);
    glVertex2f(-0.01f, -0.12f);
    glVertex2f(-0.04f, -0.12f);
    // Pé direito
    glVertex2f(0.01f, -0.1f);
    glVertex2f(0.04f, -0.1f);
    glVertex2f(0.04f, -0.12f);
    glVertex2f(0.01f, -0.12f);
    glEnd();

    // Desenha a prancha do skate
    glColor3f(0.0f, 0.0f, 0.0f); // Preto
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.12f); // Abaixa a prancha
    glVertex2f(0.2f, -0.12f);  // Abaixa a prancha
    glVertex2f(0.2f, -0.15f);  // Abaixa a prancha
    glVertex2f(-0.2f, -0.15f); // Abaixa a prancha
    glEnd();

    // Desenha as rodas do skate
    glColor3f(0.8f, 0.8f, 0.8f); // Cinza claro (rodas)
    float rodaDistancia = 0.2f; // Distância das rodas da linha central da prancha

    // As rodas devem ser desenhadas em relação à prancha
    for (float offset = -0.15f; offset <= 0.15f; offset += 0.3f) { // Duas rodas
        float rodaX = offset; // Posição das rodas no eixo X, relativas à prancha
        float rodaY = -0.19f; // Posição das rodas no eixo Y, abaixo da prancha
        glPushMatrix();
        glTranslatef(rodaX, rodaY, 0.0f); // Ajusta a posição das rodas em relação à prancha
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 360; i += 10) {
            float theta = i * 3.14159f / 180.0f;
            glVertex2f(0.03f * cos(theta), 0.03f * sin(theta)); // Roda circular
        }
        glEnd();
        glPopMatrix();
    }

    glPopMatrix(); // Restaura a transformação do skatista
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define a visão da câmera
    glLoadIdentity();
    float eyeX = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    float eyeY = cameraDistance * sin(cameraAngleX);
    float eyeZ = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
    gluLookAt(eyeX, eyeY, eyeZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // Posiciona a câmera

    desenhaPista();
    desenhaSkatista();

    glutSwapBuffers();
}

// Interação via teclado
void teclado(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': // Aumenta a velocidade do skatista
        velocidade += 0.003f;
        break;
    case 's': // Diminui a velocidade do skatista
        velocidade -= 0.003f;
        break;
    case 'a':
        velocidade = 0.0f; // Para o skatista
        break;
    case 'd':
        velocidade = 0.009f; // Volta para a velocidade normal
        break;
    case 'q': // Sai do programa
        exit(0);
    }
    glutPostRedisplay();
}

void setas(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        cameraDistance -= 0.05f; // Aproxima a câmera
        if (cameraDistance < -5.0f) cameraDistance = -3.0f; // Limite de aproximação
        break;
    case GLUT_KEY_DOWN:
        cameraDistance += 0.05f; // Afasta a câmera
        if (cameraDistance > 5.0f) cameraDistance = 5.0f;  // Limite de afastamento
        break;
    glutPostRedisplay();
    }
}

// Atualiza a posição do skatista
void atualiza(int value) {
    posicaoSkatista += direcao * velocidade; // Incrementa ou decrementa a posição
    if (posicaoSkatista >= 1.0f || posicaoSkatista <= 0.0f) {
        direcao *= -1.0f; // Inverte a direção
    }
    glutPostRedisplay();
    glutTimerFunc(16, atualiza, 0);
}


// Configuração inicial
void inicializa() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Skatista vs Half-Pipe");

    inicializa();
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(setas);
    glutTimerFunc(16, atualiza, 0);
    glutMainLoop();

    return 0;
}