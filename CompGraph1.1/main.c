#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#define N 10
#define W 640
#define H 480

Form a[N];
Form palleteForms[3];
Form selectFormType[7];

int drawFormCount = 0;
int displayCallCount = 0;
int nActiveForms = 0;
int clickCount = 0;
int xPos, yPos;
int mouseX = 0, mouseY = 0;

void initForms() {
    for (int i = 0; i < N; i++)
        a[i] = NULL;

    initRandomForms(a, N, W, H);

    for (int k = 0; k < N; k++)
        if (a[k] != NULL)
            printfForm(a[k]);
}

void deleteRandForm() {
    if (nActiveForms <= 0 || isEmpty(a, N)) {
        printf("\n\n---------------------------");
        printf("\n| No form left to Delete. |");
        printf("\n---------------------------\n\n");
        return;
    }

    int res = 0;
    int attempts = 0;
    int maxAttempts = N * 2;

    do {
        int formToDelete = rand() % N;
        attempts++;

        if (!a[formToDelete])
            continue;

        res = cleanForm(a, formToDelete, N);
        if (res == 1) {
            nActiveForms--;
            break;
        }

    } while (attempts < maxAttempts);

    if (isEmpty(a, N)) {
        printf("\n\n---------------------------");
        printf("\n| All forms deleted.     |");
        printf("\n---------------------------\n\n");
    }

    glutPostRedisplay();
}

void palette() {
    drawPalette(palleteForms);
    glFlush();
}

void shapes() {
    for (int i = 0; i < 7; i++) {
        drawForm(selectFormType[i]);
    }
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    displayCallCount++;

    palette();
    shapes();

    for (int i = 0; i < N; i++) {
        if (a[i] != NULL) {
            drawForm(a[i]);
            drawFormCount++;
        }
    }

    printf("Draw Count: %d\n", drawFormCount);
    printf("Display Call Count: %d\n", displayCallCount);

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    int invertedY = H - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        clickCount++;

        int colorIndex = selectedColor(palleteForms, x, invertedY);
        int shapeIndex = selectedShape(selectFormType, x, invertedY);

        if (colorIndex != 0) {
            selectPaleteColor(palleteForms, colorIndex);
            glutPostRedisplay();
            clickCount = 0;
            return;
        }

        if (shapeIndex != 0) {
            activeShape(selectFormType, shapeIndex);
            glutPostRedisplay();
            clickCount = 0;
            return;
        }

        if (clickCount == 2) {
            if (nActiveForms < N && addFormAT(a, palleteForms,selectFormType, N, W, H, x, invertedY)) {
                printf("Form created at (%d,%d)\n", x, invertedY);
                nActiveForms++;
            }
            else {
                printf("\n\n---------------------------\n");
                printf("| No space to add new form |\n");
                printf("---------------------------\n\n");
            }
            clickCount = 0;
        }
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        deleteRandForm();
    }
}

void updateMousePosition(int x, int y) {
    mouseX = x;
    mouseY = H - y;
}

void keyboard(unsigned char key, int x, int y) {
    printf("Tecla: %c (%d)\n", key, key);

    switch (key) {
    case 'd':
    case 'D':
        printf("Deleting form at current mouse position (%d, %d)\n", mouseX, mouseY);
        deleteFormAt(a, N, mouseX, mouseY);
        glutPostRedisplay();
        break;

    case 'q':
    case 'Q':
        exit(0);
        break;

    default:
        break;
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    createPaletteForms(palleteForms);
    createFormsToSelect(selectFormType, W, H);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutCreateWindow("Simple OpenGL Forms");

    glViewport(0, 0, W, H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(updateMousePosition);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
