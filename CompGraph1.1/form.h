#ifndef __FORM_H__
#define __FORM_H__

/*
*  This interface provides clients with handles to form objects,
*  but does not give any information about the internal representation.
*  (The struct is specified only by its tag name.)
*/

typedef struct form* Form;

// create a new form by given the original point and the sides
Form newForm(float x, float y, float xSize, float ySize);

// create a new form by given the center point and the sides
Form newFormC(float x, float y, float xSize, float ySize);

// delete form
void deleteForm(Form f);

// update a form position - moving by (dx,dy)
void updateForm(Form f, float dx, float dy);

void printfForm(Form f);

void drawForm(Form f);

void initRandomForms(Form forms[], int n);
#endif

#pragma once
