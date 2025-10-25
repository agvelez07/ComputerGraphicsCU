#ifndef __FORM_H__
#define __FORM_H__

typedef struct form* Form;

Form newForm(float x, float y, float xSize, float ySize, int formType);
Form newFormC(float x, float y, float xSize, float ySize, int formType);

void deleteForm(Form f);
int deleteFormAt(Form forms[], int n, int x, int y);
void updateForm(Form f, float dx, float dy);
void printfForm(Form f);
void drawForm(Form f);

 
void initRandomForms(Form forms[], int n, int w, int h);
int addRandomForm(Form forms[], Form palleteForm[], Form selectFormType[], int n, int w, int h);
int addFormAT(Form forms[], Form palleteForm[], Form selectFormType[], int n, int w, int h, int x, int y);
int cleanForm(Form forms[], int pos, int n);
int isEmpty(Form forms[], int n);
int formExistsAt(Form forms[], int n, int x, int y);
int formsOverlap(Form a, Form b);
int overlapsAny(Form newForm, Form forms[], int n);

 
void createPaletteForms(Form forms[]);
void drawPalette(Form forms[]);
int selectPaleteColor(Form palleteForms[], int i);
int selectedColor(Form palleteForms[], int x, int y);
int activeColor(Form forms[], int i);

 
void createFormsToSelect(Form forms[], int w, int h);
int selectedShape(Form forms[], int x, int y);
int activeShape(Form forms[], int i);

#endif