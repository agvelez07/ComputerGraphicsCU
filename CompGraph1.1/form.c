#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float hex_center_x = 0.0f;
float hex_center_y = 0.0f;
float hex_radius = 0.6f;   /* radius in world coordinates */
float hex_rotation = 0.0f;

int formTypeN = 7;

typedef enum formType {
    RECTANGLE,
    SQUARE,
    TRIANGLE,
    LINE,
    EQUILATERAL_TRIANGLE,
    HEXAGON,
    CIRCLE
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

void createRetangle(Form f) {
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

// Sentido contrário ao relógio
void createSquare(Form f) {
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

void createIsocelesTriangle(Form f) {
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + (f->ySize * 2 ));
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
}

void createLine(Form f) {
    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glBegin(GL_LINES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glEnd();
}


void createEquilateralTriangle(Form f) {
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
}

// Hexágono regular
void createHexagon(Form f) {
    float cx = f->x + f->xSize / 2.0f;
    float cy = f->y + f->ySize / 2.0f;

    /* width = 2*r, height = sqrt(3)*r -> r deve caber em ambos */
    float r1 = f->xSize / 2.0f;
    float r2 = f->ySize / sqrtf(3.0f);
    float r = (r1 < r2) ? r1 : r2;

    /* começar em PI/6 para ter “flat top” (topo plano). Para pointy-top use start = 0.0f */
    float start = M_PI / 6.0f;

    /* Fill */
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        float angle = start + (2.0f * M_PI * i) / 6.0f;
        glVertex2f(cx + r * cosf(angle), cy + r * sinf(angle));
    }
    glEnd();

    /* Border */
    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 6; ++i) {
        float angle = start + (2.0f * M_PI * i) / 6.0f;
        glVertex2f(cx + r * cosf(angle), cy + r * sinf(angle));
    }
    glEnd();

    glFlush();
}

// Círculo
void createCircle(Form f) {
    float cx = f->x + f->xSize / 2;
    float cy = f->y + f->ySize / 2;
    float r = f->xSize / 2;

    glColor3f(f->r, f->g, f->b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // centro

    for (int i = 0; i <= 50; i++) { // 50 segmentos
        float angle = 2.0 * M_PI * i / 50;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();

    // Contorno
    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 50; i++) {
        float angle = 2.0 * M_PI * i / 50;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
    glFlush();
}

void drawForm(Form f) {

    switch (f->formType) {
        case 0:
			createRetangle(f);
            break;
        case 1:
			createSquare(f);
            break;
        case 2:
            createIsocelesTriangle(f);
            break;
        case 3:
			createLine(f);
            break;
        case 4:
            createEquilateralTriangle(f); 
            break;
        case 5:
            createHexagon(f); 
            break;
        case 6:
            createCircle(f);
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
			case 4://Equilateral Triangle
			case 5://Hexagon
			case 6://Circle
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


void cleanForm(Form forms[], int pos, int n) {
    if(pos >= 0 && pos < n){

        deleteForm(forms[pos]);
        forms[pos] = NULL;
	    
        for(int i = 0; i < n - 1; i++){
            if (!forms[i] && forms[i + 1]) {
                forms[i] = forms[i + 1];
				forms[i + 1] = NULL;
            }
        }
    }
}

int isEmpty(Form forms[], int n) {
    for (int i = 0; i < n; i++) {
        if (forms[i] != NULL)
            return 0;
    }
    return 1; 
}
