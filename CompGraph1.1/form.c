#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/glut.h>

int formTypeN = 4;

// internal representation
typedef enum formType {
    RECTANGLE,
    SQUARE,
    TRIANGLE,
    LINE
} FormType;

struct form {
    float x, y;                 // initial point
    float xSize, ySize;         // dimensions
    float r, g, b;              // fill color
    float rBorder, gBorder, bBorder; // border color
    FormType formType;
};

// --- constructors ---

Form newForm(float x, float y, float xSize, float ySize, int formType) {
    Form f = (Form)malloc(sizeof(struct form));

    f->x = x;
    f->y = y;
    f->xSize = xSize;
    f->ySize = ySize;

    // random colors
    f->r = (float)rand() / RAND_MAX;
    f->g = (float)rand() / RAND_MAX;
    f->b = (float)rand() / RAND_MAX;

    f->rBorder = (float)rand() / RAND_MAX;
    f->gBorder = (float)rand() / RAND_MAX;
    f->bBorder = (float)rand() / RAND_MAX;

    f->formType = formType; //Selecionar forma aleatoria

    return f;
}

Form newFormC(float x, float y, float xSize, float ySize, int formType) {
    return newForm(x - (xSize / 2), y - (ySize / 2), xSize, ySize, formType);
}

void deleteForm(Form f) {
    free(f);
}

void updateForm(Form f, float dx, float dy) {
    f->x += dx;
    f->y += dy;
}

void printfForm(Form f) {
    printf("Form: Pos(%.2f, %.2f) Size(%.2f, %.2f)\n",
        f->x, f->y, f->xSize, f->ySize);
}

//---- Draw Type Form -----
void createRetangle(Form f){
    // Fill
    glBegin(GL_POLYGON);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    // Border
    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glFlush();
}

//Sentido contrario ao relogio
void createSquare(Form f){
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_QUADS);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    // Contorno
    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glFlush();
}

void createTriangle(Form f){
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

}

void createLine(Form f){
    glColor3f(f->rBorder, f->gBorder, f->bBorder);  
    glBegin(GL_LINES);
    glVertex2f(f->x, f->y);                      
    glVertex2f(f->x + f->xSize, f->y + f->ySize); 
    glEnd();
}
// --- drawing ---

void drawForm(Form f) {

    switch (f->formType) {
        case 0:
			createRetangle(f);
            break;
        case 1:
			createSquare(f);
            break;
        case 2:
			createTriangle(f);
            break;
        case 3:
			createLine(f);
            break;
    }
}

// --- random initialization ---

void initRandomForms(Form forms[], int n, int w, int h) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++) {

        int formType = rand() % formTypeN;

        switch (formType)
        {
            case 0://Retangle
            {
                float xSize = ((float)rand() / RAND_MAX) * 150.0 + 50.0; // 150 a 50px
                float ySize = ((float)rand() / RAND_MAX) * 150.0 + 50.0;

                float x = ((float)rand() / RAND_MAX) * (w - xSize);
                float y = ((float)rand() / RAND_MAX) * (h - ySize);

                forms[i] = newForm(x, y, xSize, ySize, formType);

                break;
            }
            case 1://Square
			case 2 ://Trinagle
            case 3://Line
            {
                float xSize = ((float)rand() / RAND_MAX) * 150.0 + 50.0;
                float ySize = xSize;

                float x = ((float)rand() / RAND_MAX) * (w - xSize);
                float y = ((float)rand() / RAND_MAX) * (h - ySize);

                forms[i] = newForm(x, y, xSize, ySize, formType);

                break;
			}
            default:
                break;
        }
    }
}
