#ifndef __FORM_H__
#define __FORM_H__

typedef struct form* Form;

// create a new form by given the original point and the sides
Form newForm(float x, float y, float xSize, float ySize);

// create a new form by given the center point and the sides
Form newFormC(float x, float y, float xSize, float ySize, int formType);

// delete form
void deleteForm(Form f);

// update a form position - moving by (dx,dy)
void updateForm(Form f, float dx, float dy);

// print form info
void printfForm(Form f);

// draw form
void drawForm(Form f);

// initialize random forms within screen bounds
void initRandomForms(Form forms[], int n, int w, int h);

#endif
