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
int nActiveForms = 0;
int clickCount = 0;
int xPos, yPos;

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

    /*
    if (displayCallCount % 2 == 0) {
        handleFormLifecycle();

    }*/
    glFlush();
}

void myMouse(int button, int state, int x, int y) {
    int invertedY = H - y;

    char* buttonName = (button == GLUT_LEFT_BUTTON) ? "LEFT" :
        (button == GLUT_MIDDLE_BUTTON) ? "MIDDLE" :
        (button == GLUT_RIGHT_BUTTON) ? "RIGHT" : "UNKNOWN";

    char* stateName = (state == GLUT_DOWN) ? "DOWN" : "UP";

    printf("Mouse event:\n");
    printf("  Button: %s\n", buttonName);
    printf("  State:  %s\n", stateName);
    printf("  Raw Position:     (%d, %d)\n", x, y);
    printf("  Inverted Y Pos:   (%d, %d)\n\n", x, invertedY);

    //Exercise 3
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        clickCount++;
        if (clickCount == 2) {
            addRandomFormAt(a, N, W, H, x, invertedY);
			clickCount = 0;
        }
    }
    /*
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        void deleteRandForm();
    }
    */
    //
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        int res = addRandomFormAt(a, N, W, H, x, invertedY);

        if (addRandomFormAt) {
            nActiveForms++;
        }

        if (res == 1) {
            printf("Added new form at position %d:\n", res - 1);
            printfForm(a[res - 1]);
        }
        else {
            printf("\n\n---------------------------");
            printf("\n|No space to add new form.|");
            printf("\n---------------------------\n\n");

        }
        glutPostRedisplay();
    }

}

void myKeyboard(unsigned char key, int x, int y) {
    printf("Tecla: %c (%d)\n", key, key);

    switch (key) {
        // 'd' key
        case 100:
        case 68:
            deleteRandForm();
			break;
        case 113:
        case 81:
			exit(0);
			break;
        default:
            break;
    }
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
    glClearColor(0.1, 0.1, 0.1, 1.0);

    //Rato
	glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);

    //initForms();
    glutDisplayFunc(myDisplay);
    glutMainLoop();

    return 0;
}
