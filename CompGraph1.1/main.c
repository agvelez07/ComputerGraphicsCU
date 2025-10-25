#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#define N 10
#define W 640
#define H 480

Form a[N];
Form palleteForms[N];

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

void palette() {
    drawPalette(palleteForms);
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
	displayCallCount++;

    palette();
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

void mouse(int button, int state, int x, int y) {
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

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        clickCount++;
        int colorIndex = paletteColorSelected(palleteForms, x, invertedY);
         
        if (clickCount == 1 && colorIndex != 0) {
            selectPalleteColor(palleteForms, colorIndex);
            glutPostRedisplay();
            clickCount = 0;
            return;
        }

 
        if (clickCount == 2) {
            if (colorIndex == 0) {
                if (addFormAT(a, palleteForms, N, W, H, x, invertedY)) {
                    printf("Form created at (%d,%d)\n", x, invertedY);
                    nActiveForms++;
                }
                else {
                    printf("\n\n---------------------------\n");
                    printf("| No space to add new form |\n");
                    printf("---------------------------\n\n");
                }
            }
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
        
        int res = addFormAT(a, palleteForms,  N, W, H, x, invertedY);
		int isFormAlreadyThere = formExistsAt(a, N, x, invertedY);

        if (isFormAlreadyThere) {
            printf("\n\n-----------------------------------------");
            printf("\n| Form already exists at this position. |");
            printf("\n-----------------------------------------\n\n");
            glutPostRedisplay();
			return;
        }

        if (addFormAT) {
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

int main(int argc, char** argv) {
    createPaleteForms(palleteForms);
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

    glutMainLoop();
    return 0;
}