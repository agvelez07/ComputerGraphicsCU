#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float hex_center_x = 0;
float hex_center_y = 0;
float hex_radius = 0;   
float hex_rotation = 0;

float r = 0;
float g = 0;
float b = 0;

int formTypeN = 7;
int palleteFormN = 3;

int hasActiveColor = 0;
int hasActiveShape = 0;
int activeShapeType = -1;


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
    float x, y;              
    float xSize, ySize;         
    float r, g, b;              
    float rBorder, gBorder, bBorder; 
    FormType formType;
};

Form newForm(float x, float y, float xSize, float ySize, int formType) {
    Form f = (Form)malloc(sizeof(struct form));

    printf("\n\n\n\nR: %.2f G: %.2f B: %.2f\n\n\n\n", r, g, b);

    f->x = x;
    f->y = y;
    f->xSize = xSize;
    f->ySize = ySize;
     
    if(hasActiveColor){
        f->r = r;
        f->g = g;
        f->b = b;
    }
    else {
        f->r = (float)rand() / RAND_MAX;
        f->g = (float)rand() / RAND_MAX;
        f->b = (float)rand() / RAND_MAX;
    }

    f->rBorder = (float)rand() / RAND_MAX;
    f->gBorder = (float)rand() / RAND_MAX;
    f->bBorder = (float)rand() / RAND_MAX;

    if (hasActiveShape && activeShapeType >= 0) {
        f->formType = activeShapeType;

        if (hasActiveShape && activeShapeType >= 0) {
            f->formType = activeShapeType;

            if (activeShapeType == RECTANGLE) {
                f->xSize *= 1.8;
            }
        }
        else {
            f->formType = formType;
        }


    }
    return f;
}

Form newFormC(float x, float y, float xSize, float ySize, int formType) {
    return newForm(x - (xSize / 2), y - (ySize / 2), xSize, ySize, formType);
}

int formsOverlap(Form a, Form b) {
    if (!a || !b) return 0;

    return (
        a->x < b->x + b->xSize + 1 &&
        a->x + a->xSize + 1 > b->x &&
        a->y < b->y + b->ySize + 1 &&
        a->y + a->ySize + 1 > b->y
        );
}

int overlapsAny(Form newForm, Form forms[], int n) {
    for (int i = 0; i < n; i++) {
        if (forms[i] && formsOverlap(newForm, forms[i])) {
            return 1;
        }
    }
    return 0;
}

void deleteForm(Form f) {
    free(f);
}

int deleteFormAt(Form forms[], int n, int x, int y) {
    for (int i = 0; i < n; i++) {
        if (forms[i] != NULL && forms[i]->x == x && forms[i]->y == y) {
            deleteForm(forms[i]);
            forms[i] = NULL; 
            return 1;
        }
    }
    return -1;
}

void updateForm(Form f, float dx, float dy) {
    f->x += dx;
    f->y += dy;
}

void printfForm(Form f) {
    if (f) {
        printf("Form: Pos(%.2f, %.2f) Size(%.2f, %.2f)\n",
            f->x, f->y, f->xSize, f->ySize);
    }
}

void createRetangle(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

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

void createSquare(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_QUADS);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

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
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + (f->ySize * 2));
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + (f->ySize * 2));
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glFlush();
}

void createLine(Form f) {
    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glBegin(GL_LINES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glEnd();
    glFlush();
}

void createEquilateralTriangle(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glFlush();
}

void createHexagon(Form f) {
    float cx = f->x + f->xSize / 2.0;
    float cy = f->y + f->ySize / 2.0;
    float r1 = f->xSize / 2.0;
    float r2 = f->ySize / sqrtf(3.0);
    float r = (r1 < r2) ? r1 : r2;
    float start = M_PI / 6.0;

    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        float angle = start + (2.0 * M_PI * i) / 6.0;
        glVertex2f(cx + r * cosf(angle), cy + r * sinf(angle));
    }
    glEnd();

    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 6; ++i) {
        float angle = start + (2.0 * M_PI * i) / 6.0;
        glVertex2f(cx + r * cosf(angle), cy + r * sinf(angle));
    }
    glEnd();

    glFlush();
}

void createCircle(Form f) {
    float cx = f->x + f->xSize / 2;
    float cy = f->y + f->ySize / 2;
    float r = f->xSize / 2;

    glColor3f(f->r, f->g, f->b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 50; i++) {
        float angle = 2.0 * M_PI * i / 50;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();

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

int addRandomForm(Form forms[], Form palleteForm[], Form selectFormType[], int n, int w, int h) {
    for (int i = 0; i < n; i++) {
        if (forms[i] == NULL) {
            int formType = rand() % formTypeN;
            float size = ((float)rand() / RAND_MAX) * 100.0 + 30.0;
            float x, y;
            int attempts = 0;
            Form temp = NULL;

            do {
                x = ((float)rand() / RAND_MAX) * (w - size);
                y = ((float)rand() / RAND_MAX) * (h - size);

                if (temp) deleteForm(temp);
                temp = newForm(x, y, size, size, formType);
                attempts++;

            } while ((overlapsAny(temp, forms, n) ||
                overlapsAny(temp, palleteForm, palleteFormN) ||
                overlapsAny(temp, selectFormType, formTypeN)) &&
                attempts < 100);

            if (attempts >= 100) {
                printf("Nao encontrei espaco livre para nova figura.\n");
                deleteForm(temp);
                return -1;
            }

            forms[i] = temp;
            printf("Figura criada sem colisoes.\n");
            return 1;
        }
    }
    return -1;
}

int addFormAT(Form forms[], Form palleteForm[], Form selectFormType[], int n, int w, int h, int x, int y) {
    for (int i = 0; i < n; i++) {
        if (forms[i] == NULL) {
            int formType = rand() % formTypeN;
            float size = ((float)rand() / RAND_MAX) * 100.0 + 30.0;

            Form temp = newForm(x, y, size, size, formType);

            if (overlapsAny(temp, palleteForm, palleteFormN) ||
                overlapsAny(temp, selectFormType, formTypeN) ||
                overlapsAny(temp, forms, n)) {
                deleteForm(temp);
                return -1;
            }

            forms[i] = temp;
            printfForm(forms[i]);
            return 1;
        }
    }
    return -1;
}


int cleanForm(Form forms[], int pos, int n) {
    if(pos >= 0 && pos < n && forms[pos]) {

        deleteForm(forms[pos]);
        forms[pos] = NULL;
	    
        for(int i = 0; i < n - 1; i++){
            if (!forms[i] && forms[i + 1]) {
                forms[i] = forms[i + 1];
				forms[i + 1] = NULL;
            }
        }
        return 1;
    }
    return -1;
}

int isEmpty(Form forms[], int n) {
    for (int i = 0; i < n; i++) {
        if (forms[i] != NULL)
            return 0;
    }
    return 1; 
}

int formExistsAt(Form forms[], int n, int x, int y) {
    for (int i = 0; i < n; i++) {
        if (forms[i]->x == x && forms[i]->y == y) {
			return 1;
        }
    }
    return 0;
}

//+-------------------------------------------------------------------------------------------------------------------------------
//|  Pallete   
//|
//+-------------------------------------------------------------------------------------------------------------------------------

void createPaletteForms(Form forms[]) {
    float size = 80;
    float y = 10;

    for (int i = 0; i < palleteFormN; i++) {
        forms[i] = (Form)malloc(sizeof(struct form));
        float x = 10 + i * (size + 10.0);

        forms[i]->x = x;
        forms[i]->y = y;
        forms[i]->xSize = size;
        forms[i]->ySize = size;

        switch (i) {
        case 0:
            forms[i]->r = 1; forms[i]->g = 0; forms[i]->b = 0; // vermelho
            break;
        case 1:
            forms[i]->r = 0; forms[i]->g = 1; forms[i]->b = 0; // verde
            break;
        case 2:
            forms[i]->r = 0; forms[i]->g = 0; forms[i]->b = 1; // azul
            break;
        }

        forms[i]->rBorder = 1;
        forms[i]->gBorder = 1;
        forms[i]->bBorder = 1;
        forms[i]->formType = SQUARE;
    }
}

void drawPalette(Form forms[]) {
    for (int i = 0; i < palleteFormN; i++) {
        if (forms[i]) createSquare(forms[i]);
    }
}

int selectPaleteColor(Form palleteForms[], int i) {
    if (i != 0) {
        Form f = palleteForms[i - 1];
        f->rBorder = 0;
        f->gBorder = 1;
        f->bBorder = 0;
        
        hasActiveColor = 1;

        r = f->r;
		g = f->g;
		b = f->b;

        for (int j = 0; j < palleteFormN; j++) {
            if (j != i - 1) {
                Form other = palleteForms[j];
                other->rBorder = 1;
                other->gBorder = 1;
                other->bBorder = 1;
            }
		}
        return 1;
    }
    return 0;
}

int selectedColor(Form palleteForms[], int x, int y) {
    for (int i = 0; i < palleteFormN; i++) {
        Form f = palleteForms[i];
        if (f) {
            if (x >= f->x && x <= f->x + f->xSize && y >= f->y && y <= f->y + f->ySize) {
                return i + 1;
            }
        }
    }
    return 0;
}

int activeColor(Form forms[], int i) {
    if (i != 0) {
        Form f = forms[i - 1];
        f->rBorder = 0;
        f->gBorder = 1;
        f->bBorder = 0;

        hasActiveColor = 1;

        r = f->r;
        g = f->g;
        b = f->b;



        for (int j = 0; j < palleteFormN; j++) {
            if (j != i - 1) {
                Form other = forms[j];
                other->rBorder = 1;
                other->gBorder = 1;
                other->bBorder = 1;
            }
        }
        return 1;
    }
    return 0;
}

//+-------------------------------------------------------------------------------------------------------------------------------
//|  Forms 
//|
//+-------------------------------------------------------------------------------------------------------------------------------

void createFormsToSelect(Form forms[], int w, int h) {
    float size = 40;
    float marginRight = 60;
    float spacing = 25;
    float extraSpacing = 20;
    float totalHeight = formTypeN * size + (formTypeN - 1) * spacing + extraSpacing;
    float startY = (h - totalHeight) / 2;
    float xCenter = w - marginRight - size / 2;

    for (int i = 0; i < formTypeN; i++) {
        forms[i] = (Form)malloc(sizeof(struct form));

        float offsetY = (i > 3) ? extraSpacing : 0;
        float y = startY + i * (size + spacing) + offsetY;

        if (i == 3) {
            y += 10;
        }

        forms[i]->xSize = size;
        forms[i]->ySize = size;

      
        forms[i]->r = 0.83;
        forms[i]->g = 0.83;
        forms[i]->b = 0.83;
        forms[i]->rBorder = 1;
        forms[i]->gBorder = 1;
        forms[i]->bBorder = 1;

  
        switch (i) {
        case RECTANGLE:
            forms[i]->formType = RECTANGLE;
            forms[i]->xSize = size * 2.5;
            forms[i]->ySize = size;
            forms[i]->x = xCenter - (forms[i]->xSize / 2);
            break;
        default:
            forms[i]->formType = i;
            forms[i]->x = xCenter - (forms[i]->xSize / 2);
            break;
        }

        forms[i]->y = y;
    }
}

int selectedShape(Form forms[], int x, int y) {
    for (int i = 0; i < formTypeN; i++) {
        Form f = forms[i];
        if (f) {
            if (x >= f->x && x <= f->x + f->xSize && y >= f->y && y <= f->y + f->ySize) {
                return i + 1;
            }
        }
    }
    return 0;
}

int activeShape(Form forms[], int i) {
    if (i != 0) {
        Form f = forms[i - 1];
        f->rBorder = 0;
        f->gBorder = 1;
        f->bBorder = 0;

        hasActiveShape = 1;
        activeShapeType = f->formType;

        for (int j = 0; j < formTypeN; j++) {
            if (j != i - 1) {
                Form other = forms[j];
                other->rBorder = 1;
                other->gBorder = 1;
                other->bBorder = 1;
            }
        }
        return 1;
    }
    return 0;
}