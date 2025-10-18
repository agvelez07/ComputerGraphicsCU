#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#define N 10
#define W 640
#define H 480

Form a[N];
int drawFormCount = 0;
int displayCallCount = 0;


void initForms() {
    for (int i = 0; i < N; i++)
        a[i] = NULL;

    initRandomForms(a, N, W, H);

    for (int k = 0; k < N; k++)
        if (a[k] != NULL)
            printfForm(a[k]);
}

void handleFormLifecycle() {
    int pos = rand() % N;

    if (isEmpty(a, N)) {
        initForms();
        return;
    }

    if (!a[pos]) {
        handleFormLifecycle();
        return;
    }

    cleanForm(a, pos, N);
}

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
	displayCallCount++;

    for (int i = 0; i < N; i++) {
        if (a[i] != NULL) {
            drawForm(a[i]);
            drawFormCount++;
           
        }
    }
    printf("Draw Count: %d\n", drawFormCount);
    printf("Display Call Count: %d\n", displayCallCount);


    if (displayCallCount % 2 == 0) {
        handleFormLifecycle();

    }
    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutCreateWindow("Simple OpenGL Forms");

    //coordenadas do ecrã
    glViewport(0, 0, W, H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);

    // Fundo visível
    glClearColor(0.1, 0.1, 0.1, 1.0);

    initForms();
    glutDisplayFunc(myDisplay);
    glutMainLoop();

    return 0;
}
