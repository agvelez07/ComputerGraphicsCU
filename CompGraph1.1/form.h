#ifndef FORM_H
#define FORM_H

typedef struct form* Form;

Form newForm(float x, float y, float xSize, float ySize, int formType);
Form newFormC(float x, float y, float xSize, float ySize, int formType);
void deleteForm(Form f);

int formsOverlap(Form a, Form b);
int overlapsAny(Form f, Form list[], int n);
int overlapsAnyExcept(Form f, Form list[], int n, int ignoreIndex);
int getFormIndexAt(Form list[], int n, int mouseX, int mouseY);
int isPointInsideAnyForm(Form forms[], int n, int x, int y);

int deleteFormAt(Form forms[], int n, int x, int y);
int cleanForm(Form forms[], int pos, int n);
int isEmpty(Form forms[], int n);
void printfForm(Form f);
void moveForm(Form f, float newX, float newY);

float getFormX(Form f);
float getFormY(Form f);
float getFormXSize(Form f);
float getFormYSize(Form f);

void drawForm(Form f);
void drawPreviewShape(int x0, int y0, int x1, int y1);
void drawFormAtPosition(Form f, float x, float y);

void createPaletteForms(Form forms[]);
void drawPalette(Form forms[]);
int selectedColor(Form forms[], int x, int y);
int selectPaleteColor(Form palleteForms[], int i);

void createFormsToSelect(Form forms[], int w, int h);
int selectedShape(Form forms[], int x, int y);
int activeShape(Form forms[], int i);

int drawFormOnDrag(Form forms[], Form palleteForm[], Form selectFormType[],
    int n, int w, int h,
    int initialX, int initialY, int finalX, int finalY);

int willFormCollide(Form palleteForm[], Form selectFormType[], Form forms[],
    int n, int x0, int y0, int x1, int y1);

void toggleBoundingBoxes();
int areBoundingBoxesVisible();
void setBoundingBoxColor(Form f, float r, float g, float b);

void dropFormsDown(Form forms[], int n, int windowHeight);
void dropFormsUp(Form forms[], int n, int windowHeight);
void dropFormsLeft(Form forms[], int n);
void dropFormsRight(Form forms[], int n, int windowWidth);

int insertFormInLargestFreeSpace(Form forms[], int n, int w, int h,
    Form palleteForm[], Form selectFormType[], int formType);

#endif
