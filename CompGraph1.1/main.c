#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define N 10

int winW = 640;
int winH = 480;

#define W (winW)
#define H (winH)
#define TOOLBAR_HEIGHT (H * 0.15)

Form a[N];
Form palleteForms[3];
Form selectFormType[7];

int nActiveForms = 0;
int isDragging = 0;
int isDraggingForm = 0;
int draggedFormIndex = -1;
int dragStartX, dragStartY;
int dragCurrentX, dragCurrentY;

int mouseX = 0, mouseY = 0;

float dragOffsetX = 0, dragOffsetY = 0;
float originalFormX = 0, originalFormY = 0;

void drawToolbarBackground() {
    float toolbarBottom = H - TOOLBAR_HEIGHT;

    glLoadIdentity();
    glBegin(GL_QUADS);
    glColor3f(0.15, 0.15, 0.15);
    glVertex2f(0.0, H);
    glVertex2f(W, H);
    glVertex2f(W, toolbarBottom);
    glVertex2f(0.0, toolbarBottom);
    glEnd();
}

void palette() {
    drawPalette(palleteForms);
    glFlush();
}

void shapes() {
    for (int i = 0; i < 7; i++)
        drawForm(selectFormType[i]);
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawToolbarBackground();
    palette();
    shapes();

    for (int i = 0; i < N; i++) {
        if (a[i] != NULL && i != draggedFormIndex) {
            drawForm(a[i]);
        }
    }

    if (isDragging && !isDraggingForm) {
        int collides = willFormCollide(palleteForms, selectFormType, a,
            N, dragStartX, dragStartY,
            dragCurrentX, dragCurrentY);

        if (collides) {
            glColor3f(1.0, 0.0, 0.0);
        }
        else {
            glColor3f(0.0, 1.0, 0.0);
        }

        glEnable(GL_COLOR_LOGIC_OP);
        glLogicOp(GL_XOR);
        drawPreviewShape(dragStartX, dragStartY, dragCurrentX, dragCurrentY);
        glDisable(GL_COLOR_LOGIC_OP);
    }

    if (isDraggingForm && draggedFormIndex >= 0) {
        Form f = a[draggedFormIndex];
        if (f) {
            float newX = dragCurrentX - dragOffsetX;
            float newY = dragCurrentY - dragOffsetY;
            drawFormAtPosition(f, newX, newY);
        }
    }

    glFlush();
}

void deleteRandForm() {
    for (int i = 0; i < N; i++) {
        if (a[i] != NULL) {
            cleanForm(a, i, N);
            nActiveForms--;
            glutPostRedisplay();
            return;
        }
    }
}

void mouse(int button, int state, int x, int y) {
    int invertedY = H - y;
    float toolbarBottom = H - TOOLBAR_HEIGHT;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int colorIndex = selectedColor(palleteForms, x, invertedY);
        if (colorIndex != 0) {
            selectPaleteColor(palleteForms, colorIndex);
            glutPostRedisplay();
            return;
        }

        int shapeIndex = selectedShape(selectFormType, x, invertedY);
        if (shapeIndex != 0) {
            activeShape(selectFormType, shapeIndex);
            glutPostRedisplay();
            return;
        }

        int formIndex = getFormIndexAt(a, N, x, invertedY);
        if (formIndex >= 0) {
            isDraggingForm = 1;
            draggedFormIndex = formIndex;

            Form f = a[formIndex];

            originalFormX = getFormX(f);
            originalFormY = getFormY(f);

            dragOffsetX = x - originalFormX;
            dragOffsetY = invertedY - originalFormY;

            dragStartX = x;
            dragStartY = invertedY;
            dragCurrentX = x;
            dragCurrentY = invertedY;

            printf("Movendo forma %d de (%.2f, %.2f)\n", formIndex, originalFormX, originalFormY);
            printf("Offset: (%.2f, %.2f)\n", dragOffsetX, dragOffsetY);
            return;
        }

        if (invertedY > toolbarBottom) {
            printf("Zona de ferramentas: nao e possivel criar formas aqui\n");
            return;
        }

        if (isPointInsideAnyForm(a, N, x, invertedY) ||
            isPointInsideAnyForm(palleteForms, 3, x, invertedY) ||
            isPointInsideAnyForm(selectFormType, 7, x, invertedY)) {
            printf("Area ocupada!\n");
            return;
        }

        isDragging = 1;
        dragStartX = x;
        dragStartY = invertedY;
        dragCurrentX = x;
        dragCurrentY = invertedY;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (isDraggingForm && draggedFormIndex >= 0) {
            Form f = a[draggedFormIndex];
            if (f) {
                float newX = dragCurrentX - dragOffsetX;
                float newY = dragCurrentY - dragOffsetY;

                printf("Tentando mover para (%.2f, %.2f)\n", newX, newY);

                moveForm(f, newX, newY);

                float formXSize = getFormXSize(f);
                float formYSize = getFormYSize(f);

                if (newX < 0 || newY < 0 ||
                    newX + formXSize > W || newY + formYSize > toolbarBottom ||
                    overlapsAny(f, palleteForms, 3) ||
                    overlapsAny(f, selectFormType, 7) ||
                    overlapsAnyExcept(f, a, N, draggedFormIndex)) {
                    moveForm(f, originalFormX, originalFormY);
                    printf("Movimento cancelado (colisao ou toolbar). Voltou para (%.2f, %.2f)\n", originalFormX, originalFormY);
                }
                else {
                    printf("Forma movida com sucesso para (%.2f, %.2f)!\n", newX, newY);
                }
            }

            isDraggingForm = 0;
            draggedFormIndex = -1;
            glutPostRedisplay();
            return;
        }

        if (isDragging) {
            if (nActiveForms < N) {
                float x0 = dragStartX;
                float y0 = dragStartY;
                float x1 = dragCurrentX;
                float y1 = dragCurrentY;

                float x = (x1 < x0 ? x1 : x0);
                float y = (y1 < y0 ? y1 : y0);
                float xSize = fabs(x1 - x0);
                float ySize = fabs(y1 - y0);

                if (xSize < 20 || ySize < 20) {
                    printf("Forma demasiado pequena\n");
                }
                else if (y + ySize > toolbarBottom) {
                    printf("Nao foi possivel criar forma na zona de ferramentas\n");
                }
                else {
                    int result = drawFormOnDrag(a, palleteForms, selectFormType,
                        N, W, (int)toolbarBottom, dragStartX, dragStartY, dragCurrentX, dragCurrentY);

                    if (result == 1) {
                        nActiveForms++;
                        printf("Forma criada! Total: %d/%d\n", nActiveForms, N);
                    }
                    else {
                        printf("Nao foi possivel criar forma\n");
                    }
                }
            }
            else {
                printf("Limite atingido (%d/%d)\n", nActiveForms, N);
            }
        }

        isDragging = 0;
        glutPostRedisplay();
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        int formIndex2 = getFormIndexAt(a, N, x, invertedY);
        if (formIndex2 >= 0) {
            cleanForm(a, formIndex2, N);
            nActiveForms--;
            printf("Forma eliminada! Total: %d/%d\n", nActiveForms, N);
            glutPostRedisplay();
        }
        else {
            printf("Nenhuma forma sob o cursor\n");
        }
    }
}

void mouseDrag(int x, int y) {
    if (isDragging || isDraggingForm) {
        dragCurrentX = x;
        dragCurrentY = H - y;
        glutPostRedisplay();
    }
}

void updateMousePosition(int x, int y) {
    mouseX = x;
    mouseY = H - y;
}

void keyboard(unsigned char key, int x, int y) {
    int workHeight = (int)(H - TOOLBAR_HEIGHT);

    if (key == 'd' || key == 'D') {
        int deleted = deleteFormAt(a, N, mouseX, mouseY);
        if (deleted == 1) {
            nActiveForms--;
            printf("Forma eliminada! Total: %d/%d\n", nActiveForms, N);
        }
        glutPostRedisplay();
    }

    if (key == 'b' || key == 'B') {
        toggleBoundingBoxes();
        printf("Bounding boxes: %s\n", areBoundingBoxesVisible() ? "ON" : "OFF");
        glutPostRedisplay();
    }

    if (key == 'g' || key == 'G') {
        dropFormsDown(a, N, workHeight);
        printf("Formas cairam para baixo\n");
        glutPostRedisplay();
    }

    if (key == 'u' || key == 'U') {
        dropFormsUp(a, N, workHeight);
        printf("Formas subiram\n");
        glutPostRedisplay();
    }

    if (key == 'l' || key == 'L') {
        dropFormsLeft(a, N);
        printf("Formas foram para esquerda\n");
        glutPostRedisplay();
    }

    if (key == 'r' || key == 'R') {
        dropFormsRight(a, N, W);
        printf("Formas foram para direita\n");
        glutPostRedisplay();
    }

    if (key == 'f' || key == 'F') {
        int result = insertFormInLargestFreeSpace(a, N, W, workHeight, palleteForms, selectFormType, 0);
        if (result == 1) {
            nActiveForms++;
            printf("Forma inserida no maior espaco livre! Total: %d/%d\n", nActiveForms, N);
        }
        else {
            printf("Nao foi possivel encontrar espaco livre\n");
        }
        glutPostRedisplay();
    }

    if (key == 'q' || key == 'Q') {
        printf("A encerrar...\n");
        exit(0);
    }

    if (key == 'h' || key == 'H') {
        printf("\n=== AJUDA ===\n");
        printf("MOUSE:\n");
        printf("  Esquerdo: Criar forma (drag) ou mover forma existente\n");
        printf("  Direito: Apagar forma sob cursor\n");
        printf("\nTECLADO:\n");
        printf("  D: Apagar forma sob cursor\n");
        printf("  B: Toggle bounding boxes\n");
        printf("  G: Cair para baixo\n");
        printf("  U: Cair para cima\n");
        printf("  L: Cair para esquerda\n");
        printf("  R: Cair para direita\n");
        printf("  F: Inserir no maior espaco livre\n");
        printf("  H: Mostrar ajuda\n");
        printf("  Q: Sair\n");
        printf("=============\n\n");
    }
}

void reshape(int w, int h) {
    if (h == 0) h = 1;

    winW = w;
    winH = h;

    glViewport(0, 0, winW, winH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);
    glMatrixMode(GL_MODELVIEW);

    createPaletteForms(palleteForms);
    createFormsToSelect(selectFormType, W, H);

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    for (int i = 0; i < N; i++)
        a[i] = NULL;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutCreateWindow("Editor Grafico OpenGL");

    glViewport(0, 0, W, H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    createPaletteForms(palleteForms);
    createFormsToSelect(selectFormType, W, H);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouseDrag);
    glutPassiveMotionFunc(updateMousePosition);
    glutReshapeFunc(reshape);

    printf("\n=== Editor Grafico OpenGL ===\n");
    printf("Pressione 'H' para ver comandos.\n");
    printf("=============================\n\n");

    glutMainLoop();
    return 0;
}
