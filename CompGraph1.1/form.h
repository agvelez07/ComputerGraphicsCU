#ifndef __FORM_H__
#define __FORM_H__

typedef struct form* Form;

Form newForm(float x, float y, float xSize, float ySize);
Form newFormC(float x, float y, float xSize, float ySize, int formType);

void deleteForm(Form f);
int deleteFormAt(Form forms[], int n, int x, int y);

void updateForm(Form f, float dx, float dy);
void printfForm(Form f);
void drawForm(Form f);

void initRandomForms(Form forms[], int n, int w, int h);

int addRandomForm(Form forms[], Form paletteForm[], int n, int w, int h);
int addFormAT(Form forms[], Form paletteForm[], int n, int w, int h, int x, int y);

int cleanForm(Form forms[], int pos, int n);
int isEmpty(Form forms[], int n);
int formExistsAt(Form forms[], int n, int x, int y);

void createPaletteForms(Form forms[]);
void drawPalette(Form forms[]);

int overlapsAnyPalette(Form newForm, Form paletteForms[], int n);

int paletteColorSelected(Form paletteForms[], int x, int y);
int selectPaletteColor(Form paletteForms[], int i);

#endif