#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159
#endif

float r = 0, g = 0, b = 0;

int formTypeN = 7;
int palleteFormN = 3;

int hasActiveColor = 0;
int hasActiveShape = 0;
int activeShapeType = -1;

static int showBoundingBoxes = 0;

typedef enum formType {
    RECTANGLE,
    SQUARE,
    ISOSCELES_TRIANGLE,
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
    f->x = x;
    f->y = y;
    f->xSize = xSize;
    f->ySize = ySize;

    if (hasActiveColor) {
        f->r = r; f->g = g; f->b = b;
    }
    else {
        f->r = (float)rand() / RAND_MAX;
        f->g = (float)rand() / RAND_MAX;
        f->b = (float)rand() / RAND_MAX;
    }

    f->rBorder = 1;
    f->gBorder = 1;
    f->bBorder = 1;

    if (hasActiveShape)
        f->formType = activeShapeType;
    else
        f->formType = formType;

    return f;
}

Form newFormC(float x, float y, float xSize, float ySize, int formType) {
    return newForm(x - (xSize / 2), y - (ySize / 2), xSize, ySize, formType);
}

int circlesOverlap(Form a, Form b) {
    float ax = a->x + a->xSize / 2;
    float ay = a->y + a->ySize / 2;
    float ar = a->xSize / 2;
    float bx = b->x + b->xSize / 2;
    float by = b->y + b->ySize / 2;
    float br = b->xSize / 2;
    float dx = ax - bx;
    float dy = ay - by;
    float dist = sqrt(dx * dx + dy * dy);
    return dist < (ar + br);
}

int formsOverlap(Form a, Form b) {
    if (!a || !b) return 0;
    if (a->formType == CIRCLE && b->formType == CIRCLE) {
        return circlesOverlap(a, b);
    }

    float aHeight = a->ySize, bHeight = b->ySize;
    
    if (a->formType == ISOSCELES_TRIANGLE) {
        aHeight = a->ySize * 2;
    }
    
    else if (a->formType == EQUILATERAL_TRIANGLE) {
        aHeight = (sqrt(3.0) / 2.0) * a->xSize;
    }
    
    if (b->formType == ISOSCELES_TRIANGLE) {
        bHeight = b->ySize * 2;
    }

    else if (b->formType == EQUILATERAL_TRIANGLE) {
        bHeight = (sqrt(3.0) / 2.0) * b->xSize;
    }

    return (
        a->x < b->x + b->xSize &&
        a->x + a->xSize > b->x &&
        a->y < b->y + bHeight &&
        a->y + aHeight > b->y
        );
}

int overlapsAny(Form f, Form list[], int n) {
    for (int i = 0; i < n; i++) {
        if (list[i] && formsOverlap(f, list[i]))
            return 1;
    }
    return 0;
}

int overlapsAnyExcept(Form f, Form list[], int n, int ignoreIndex) {
    for (int i = 0; i < n; i++) {
        if (i == ignoreIndex) continue;
        if (list[i] && formsOverlap(f, list[i]))
            return 1;
    }
    return 0;
}

int getFormIndexAt(Form list[], int n, int mouseX, int mouseY) {
    if (!list) {
        return -1;
    }
    
    for (int i = 0; i < n; i++) {

        if (!list[i]) {
            continue;
        }

        Form f = list[i];
        
        float height = f->ySize;
        
        if (f->formType == ISOSCELES_TRIANGLE) {
            height = f->ySize * 2;
        }
        else if (f->formType == EQUILATERAL_TRIANGLE) {
            height = (sqrt(3.0) / 2.0) * f->xSize;
        }
        if (mouseX >= f->x && mouseX <= f->x + f->xSize &&
            mouseY >= f->y && mouseY <= f->y + height) {
            return i;
        }
    }
    return -1;
}

int isPointInsideAnyForm(Form forms[], int n, int x, int y) {

    for (int i = 0; i < n; i++) 
    {
        if (!forms[i]) {
            continue;
        }
        Form f = forms[i];

        if (f->formType == CIRCLE) {
            float cx = f->x + f->xSize / 2;
            float cy = f->y + f->ySize / 2;
            float radius = f->xSize / 2;
            float dx = x - cx;
            float dy = y - cy;
            float dist = sqrt(dx * dx + dy * dy);
            if (dist <= radius) return 1;
        }
        else if (f->formType == ISOSCELES_TRIANGLE) {
            float realHeight = f->ySize * 2;
            if (x >= f->x && x <= f->x + f->xSize &&
                y >= f->y && y <= f->y + realHeight) {
                return 1;
            }
        }
        else if (f->formType == EQUILATERAL_TRIANGLE) {
            
            float realHeight = (sqrt(3.0) / 2.0) * f->xSize;
            
            if (x >= f->x && x <= f->x + f->xSize &&
                y >= f->y && y <= f->y + realHeight) {
                return 1;
            }
        }
        else {
            if (x >= f->x && x <= f->x + f->xSize &&
                y >= f->y && y <= f->y + f->ySize) {
                return 1;
            }
        }
    }
    return 0;
}

void deleteForm(Form f) {
    free(f);
}

int deleteFormAt(Form forms[], int n, int x, int y) {
    for (int i = 0; i < n; i++) {

        if (!forms[i]) continue;
        
        Form f = forms[i];
        
        float height = f->ySize;
        
        if (f->formType == ISOSCELES_TRIANGLE) {
            height = f->ySize * 2;
        }
        else if (f->formType == EQUILATERAL_TRIANGLE) {
            height = (sqrt(3.0f) / 2.0f) * f->xSize;
        }

        if (x >= f->x && x <= f->x + f->xSize &&
            y >= f->y && y <= f->y + height) {
            deleteForm(forms[i]);
            forms[i] = NULL;
            return 1;
        }
    }
    return -1;
}

void printfForm(Form f) {
    if (f) {
        printf("Form: Pos(%.2f, %.2f) Size(%.2f, %.2f)\n",
            f->x, f->y, f->xSize, f->ySize);
    }
}

int cleanForm(Form forms[], int pos, int n) {
    if (pos >= 0 && pos < n && forms[pos]) {
        deleteForm(forms[pos]);
        forms[pos] = NULL;
        return 1;
    }
    return -1;
}

int isEmpty(Form forms[], int n) {
    for (int i = 0; i < n; i++)
        if (forms[i] != NULL) return 0;
    return 1;
}

void moveForm(Form f, float newX, float newY) {
    if (f) {
        f->x = newX;
        f->y = newY;
    }
}

void drawFormAtPosition(Form f, float x, float y) {
    if (!f) {
        return;
    }
    
    float originalX = f->x;
    float originalY = f->y;
 
    f->x = x;
    f->y = y;
    
    float originalR = f->r;
    float originalG = f->g;
    float originalB = f->b;
    
    f->r *= 0.5;
    f->g *= 0.5;
    f->b *= 0.5;
    
    drawForm(f);
    f->x = originalX;
    f->y = originalY;
    f->r = originalR;
    f->g = originalG;
    f->b = originalB;
}

void toggleBoundingBoxes() {
    showBoundingBoxes = !showBoundingBoxes;
}

int areBoundingBoxesVisible() {
    return showBoundingBoxes;
}

void setBoundingBoxColor(Form f, float r2, float g2, float b2) {
    if (f) {
        f->rBorder = r2;
        f->gBorder = g2;
        f->bBorder = b2;
    }
}

float getFormX(Form f) {
    return f ? f->x : 0; 
}

float getFormY(Form f) {
    return f ? f->y : 0;
}

float getFormXSize(Form f) {
    return f ? f->xSize : 0;
}

float getFormYSize(Form f) { 
    return f ? f->ySize : 0;
}

void createRetangle(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
    if (showBoundingBoxes) {
        glColor3f(f->rBorder, f->gBorder, f->bBorder);
        glBegin(GL_LINE_LOOP);
        glVertex2f(f->x, f->y);
        glVertex2f(f->x, f->y + f->ySize);
        glVertex2f(f->x + f->xSize, f->y + f->ySize);
        glVertex2f(f->x + f->xSize, f->y);
        glEnd();
    }
}

void createSquare(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_QUADS);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
    if (showBoundingBoxes) {
        glColor3f(f->rBorder, f->gBorder, f->bBorder);
        glBegin(GL_LINE_LOOP);
        glVertex2f(f->x, f->y);
        glVertex2f(f->x, f->y + f->ySize);
        glVertex2f(f->x + f->xSize, f->y + f->ySize);
        glVertex2f(f->x + f->xSize, f->y);
        glEnd();
    }
}

void createIsocelesTriangle(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + f->ySize * 2);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
    if (showBoundingBoxes) {
        glColor3f(f->rBorder, f->gBorder, f->bBorder);
        glBegin(GL_LINE_LOOP);
        glVertex2f(f->x, f->y);
        glVertex2f(f->x + f->xSize / 2, f->y + f->ySize * 2);
        glVertex2f(f->x + f->xSize, f->y);
        glEnd();
    }
}

void createLine(Form f) {
    glColor3f(f->rBorder, f->gBorder, f->bBorder);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glEnd();
    glLineWidth(1.0f);
}

void createEquilateralTriangle(Form f) {
    float base = f->xSize;
    float height = (sqrt(3.0) / 2.0) * base;
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + base / 2, f->y + height);
    glVertex2f(f->x + base, f->y);
    glEnd();
    if (showBoundingBoxes) {
        glColor3f(f->rBorder, f->gBorder, f->bBorder);
        glBegin(GL_LINE_LOOP);
        glVertex2f(f->x, f->y);
        glVertex2f(f->x + base / 2, f->y + height);
        glVertex2f(f->x + base, f->y);
        glEnd();
    }
}

void createHexagon(Form f) {
    float cx = f->x + f->xSize / 2.0;
    float cy = f->y + f->ySize / 2.0;
    float radius = fmin(f->xSize / 2.0, f->ySize / 2.0);
    float start = M_PI / 6.0;
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        float angle = start + (2.0 * M_PI * i) / 6.0;
        glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
    }
    glEnd();
    if (showBoundingBoxes) {
        glColor3f(f->rBorder, f->gBorder, f->bBorder);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 6; ++i) {
            float angle = start + (2.0 * M_PI * i) / 6.0;
            glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
        }
        glEnd();
    }
}

void createCircle(Form f) {
    float cx = f->x + f->xSize / 2;
    float cy = f->y + f->ySize / 2;
    float radius = f->xSize / 2;
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 50; i++) {
        float angle = 2 * M_PI * i / 50;
        glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
    }
    glEnd();
    if (showBoundingBoxes) {
        glColor3f(f->rBorder, f->gBorder, f->bBorder);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 50; i++) {
            float angle = 2 * M_PI * i / 50;
            glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
        }
        glEnd();
    }
}

void drawForm(Form f) {
    switch (f->formType) {
    case RECTANGLE: createRetangle(f); break;
    case SQUARE: createSquare(f); break;
    case ISOSCELES_TRIANGLE: createIsocelesTriangle(f); break;
    case LINE: createLine(f); break;
    case EQUILATERAL_TRIANGLE: createEquilateralTriangle(f); break;
    case HEXAGON: createHexagon(f); break;
    case CIRCLE: createCircle(f); break;
    }
}

void createPaletteForms(Form forms[]) {
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);

    float toolbarH = winH * 0.15;
    float size = toolbarH * 0.6;
    float margin = size * 0.25;

    float y = winH - toolbarH + (toolbarH - size) / 2.0;

    for (int i = 0; i < palleteFormN; i++) {

        forms[i] = (Form)malloc(sizeof(struct form));
        
        float x = margin + i * (size + margin);
        
        forms[i]->x = x;
        forms[i]->y = y;
        forms[i]->xSize = size;
        forms[i]->ySize = size;
        
        if (i == 0) {
            forms[i]->r = 1; forms[i]->g = 0; forms[i]->b = 0;
        }
        
        if (i == 1) {
            forms[i]->r = 0; forms[i]->g = 1; forms[i]->b = 0; 
        }
        
        if (i == 2) {
            forms[i]->r = 0; forms[i]->g = 0; forms[i]->b = 1;   
        }
        forms[i]->rBorder = 1;
        forms[i]->gBorder = 1;
        forms[i]->bBorder = 1;
        forms[i]->formType = SQUARE;
    }
}

void drawPalette(Form forms[]) {
    for (int i = 0; i < palleteFormN; i++)
        if (forms[i]) createSquare(forms[i]);
}

int selectedColor(Form forms[], int x, int y) {

    for (int i = 0; i < palleteFormN; i++) {
    
        Form f = forms[i];
        if (!f) {
            continue;
        }
        if (x >= f->x && x <= f->x + f->xSize &&
            y >= f->y && y <= f->y + f->ySize) {
            return i + 1;
        }
    }
    return 0;
}

int selectPaleteColor(Form palleteForms[], int i) {
    if (i <= 0) {
        return 0;
    }

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
            palleteForms[j]->rBorder = 1;
            palleteForms[j]->gBorder = 1;
            palleteForms[j]->bBorder = 1;
        }
    }
    return 1;
}

void createFormsToSelect(Form forms[], int w, int h) {
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);

    float toolbarH = winH * 0.15;
    float size = toolbarH * 0.5;
    float spacing = size * 0.6;
    float y = winH - toolbarH + (toolbarH - size) / 2.0;
    float margin = size * 0.5;
    float startX = (margin + 3 * (size + margin)) + size * 0.5;


    FormType shapeOrder[7] = {
        CIRCLE,
        HEXAGON,
        EQUILATERAL_TRIANGLE,
        ISOSCELES_TRIANGLE,
        LINE,
        SQUARE,
        RECTANGLE
    };

    for (int i = 0; i < formTypeN; i++) {

        forms[i] = (Form)malloc(sizeof(struct form));

        float x = startX + i * (size + spacing);

        forms[i]->x = x;
        forms[i]->y = y;
        forms[i]->xSize = size;
        forms[i]->ySize = size;

        if (shapeOrder[i] == ISOSCELES_TRIANGLE) {
            forms[i]->ySize = size * 0.6;  
            forms[i]->xSize = size * 0.6;
        }
        forms[i]->r = 0.85;
        forms[i]->g = 0.85;
        forms[i]->b = 0.85;
        forms[i]->rBorder = 1.0;
        forms[i]->gBorder = 1.0;
        forms[i]->bBorder = 1.0;
        forms[i]->formType = shapeOrder[i];

        if (shapeOrder[i] == RECTANGLE) {
            forms[i]->xSize = size * 2.0;
            forms[i]->ySize = size * 0.9;
        }
    }
}

int selectedShape(Form forms[], int x, int y) {
    for (int i = 0; i < formTypeN; i++) {
        Form f = forms[i];
        if (!f) {
            continue;
        }
        if (x >= f->x && x <= f->x + f->xSize &&
            y >= f->y && y <= f->y + f->ySize) {
            return i + 1;
        }
    }
    return 0;
}

int activeShape(Form forms[], int i) {
    if (i <= 0) {
        return 0;
    }

    Form f = forms[i - 1];
    
    f->rBorder = 0;
    f->gBorder = 1;
    f->bBorder = 0;
    
    hasActiveShape = 1;
    activeShapeType = f->formType;
    
    for (int j = 0; j < formTypeN; j++) {
        if (j != i - 1) {
            forms[j]->rBorder = 1;
            forms[j]->gBorder = 1;
            forms[j]->bBorder = 1;
        }
    }
    return 1;
}

void previewRectangle(float x, float y, float w, float h) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void previewSquare(float x, float y, float w, float h) {
    float s = (w < h ? w : h);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + s, y);
    glVertex2f(x + s, y + s);
    glVertex2f(x, y + s);
    glEnd();
}

void previewLine(float x0, float y0, float x1, float y1) {
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(x1, y1);
    glEnd();
    glLineWidth(1.0f);
}

void previewEquilateralTriangle(float x, float y, float w) {
    float h = (sqrt(3.0f) / 2.0) * w;
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w / 2, y + h);
    glEnd();
}

void previewHexagon(float x, float y, float w, float h) {
    float cx = x + w / 2;
    float cy = y + h / 2;
    float r2 = fmin(w, h) / 2;
    float start = M_PI / 6.0;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 6; i++) {
        float angle = start + (2 * M_PI * i) / 6;
        glVertex2f(cx + r2 * cos(angle), cy + r2 * sin(angle));
    }
    glEnd();
}

void previewCircle(float x, float y, float w, float h) {
    float cx = x + w / 2;
    float cy = y + h / 2;
    float r2 = fmin(w, h) / 2;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 60; i++) {
        float angle = 2 * M_PI * i / 60;
        glVertex2f(cx + r2 * cos(angle), cy + r2 * sin(angle));
    }
    glEnd();
}

void previewIsocelesTriangle(float x, float y, float w, float h) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w / 2, y + h * 2);
    glEnd();
}

void drawPreviewShape(int x0, int y0, int x1, int y1) {
    float x = (x1 < x0 ? x1 : x0);
    float y = (y1 < y0 ? y1 : y0);
    float w = fabs(x1 - x0);
    float h = fabs(y1 - y0);

    int t = hasActiveShape ? activeShapeType : RECTANGLE;

    switch (t) {
    case RECTANGLE:
        previewRectangle(x, y, w, h);
        break;
    case SQUARE:
        previewSquare(x, y, w, h);
        break;
    case ISOSCELES_TRIANGLE:
        previewIsocelesTriangle(x, y, w, h);
        break;
    case LINE:
        previewLine(x0, y0, x1, y1);
        break;
    case EQUILATERAL_TRIANGLE:
        previewEquilateralTriangle(x, y, w);
        break;
    case HEXAGON:
        previewHexagon(x, y, w, h);
        break;
    case CIRCLE:
        previewCircle(x, y, w, h);
        break;
    }
}

int willFormCollide(Form palleteForm[], Form selectFormType[], Form forms[],
    int n, int x0, int y0, int x1, int y1)
{
    float x = (x1 < x0 ? x1 : x0);
    float y = (y1 < y0 ? y1 : y0);
    float xSize = fabs(x1 - x0);
    float ySize = fabs(y1 - y0);

    if (xSize < 20 || ySize < 20) {
        return 1;
    }

    int typeR = rand() % formTypeN;
    
    int ft = hasActiveShape ? activeShapeType : typeR;

    Form temp = newForm(x, y, xSize, ySize, ft);
    
    int collides = overlapsAny(temp, palleteForm, palleteFormN) ||
        overlapsAny(temp, selectFormType, formTypeN) ||
        overlapsAny(temp, forms, n);

    deleteForm(temp);
    return collides;
}

int drawFormOnDrag(Form forms[], Form palleteForm[], Form selectFormType[],
    int n, int w, int h,
    int initialX, int initialY, int finalX, int finalY)
{
    float x = (finalX < initialX ? finalX : initialX);
    float y = (finalY < initialY ? finalY : initialY);
    float xSize = fabs(finalX - initialX);
    float ySize = fabs(finalY - initialY);
    if (xSize < 20 || ySize < 20)
        return -1;
    if (y + ySize > h)
        return -1;

    for (int i = 0; i < n; i++) {
        if (forms[i] == NULL) {

            int typeR = rand() % formTypeN;
            int ft = hasActiveShape ? activeShapeType : typeR;

            Form temp = newForm(x, y, xSize, ySize, ft);

            if (overlapsAny(temp, palleteForm, palleteFormN) ||
                overlapsAny(temp, selectFormType, formTypeN) ||
                overlapsAny(temp, forms, n))
            {
                deleteForm(temp);
                return -1;
            }

            forms[i] = temp;
            return 1;
        }
    }
    return -1;
}

void dropFormsDown(Form forms[], int n, int windowHeight) {
    int moved;
    do {

        moved = 0;
        
        for (int i = 0; i < n; i++) {
            if (!forms[i]) continue;
        
            Form f = forms[i];
            float originalY = f->y;
            
            f->y -= 1;
            
            if (f->y < 0 || overlapsAnyExcept(f, forms, n, i)) {
                f->y = originalY;
            }
            else {
                moved = 1;
            }
        }
    } while (moved);
}

void dropFormsUp(Form forms[], int n, int windowHeight) {
    int moved;

    do {
        moved = 0;
        for (int i = 0; i < n; i++) {

            if (!forms[i]) {
                continue;
            }
            
            Form f = forms[i];
            float height = f->ySize;
            
            if (f->formType == ISOSCELES_TRIANGLE) height = f->ySize * 2;
            else if (f->formType == EQUILATERAL_TRIANGLE) height = (sqrt(3.0f) / 2.0f) * f->xSize;
            float originalY = f->y;
            f->y += 1;
            if (f->y + height > windowHeight || overlapsAnyExcept(f, forms, n, i)) {
                f->y = originalY;
            }
            else {
                moved = 1;
            }
        }
    } while (moved);
}

void dropFormsLeft(Form forms[], int n) {
    int moved;
    do {
        moved = 0;
        for (int i = 0; i < n; i++) {
            if (!forms[i]) continue;
            Form f = forms[i];
            float originalX = f->x;
            f->x -= 1;
            if (f->x < 0 || overlapsAnyExcept(f, forms, n, i)) {
                f->x = originalX;
            }
            else {
                moved = 1;
            }
        }
    } while (moved);
}

void dropFormsRight(Form forms[], int n, int windowWidth) {
    int moved;
    do {
        moved = 0;
        for (int i = 0; i < n; i++) {
            if (!forms[i]) continue;
            Form f = forms[i];
            float originalX = f->x;
            f->x += 1;
            if (f->x + f->xSize > windowWidth || overlapsAnyExcept(f, forms, n, i)) {
                f->x = originalX;
            }
            else {
                moved = 1;
            }
        }
    } while (moved);
}

int insertFormInLargestFreeSpace(Form forms[], int n, int w, int h,

    Form palleteForm[], Form selectFormType[], int formType) {

    float maxSize = 0;
    float bestX = -1, bestY = -1;
    float bestWidth = 0, bestHeight = 0;
    
    int step = 20;
    
    for (int y = 100; y < h - 100; y += step) {
        for (int x = 50; x < w - 150; x += step) {
            for (int size = 50; size < 200; size += 20) {
    
                Form temp = newForm(x, y, size, size, formType);
                
                if (!overlapsAny(temp, forms, n) &&
                    !overlapsAny(temp, palleteForm, palleteFormN) &&
                    !overlapsAny(temp, selectFormType, formTypeN)) {
                
                    float area = size * size;
                    
                    if (area > maxSize) {
                        maxSize = area;
                        bestX = x;
                        bestY = y;
                        bestWidth = size;
                        bestHeight = size;
                    }
                }
                deleteForm(temp);
            }
        }
    }
    if (bestX >= 0) {
        for (int i = 0; i < n; i++) {
            if (!forms[i]) {
                forms[i] = newForm(bestX, bestY, bestWidth, bestHeight, formType);
                return 1;
            }
        }
    }
    return -1;
}
