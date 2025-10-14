#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <gl/glut.h>


// this struct is encapsulated
struct form {
	float x, y;             // initial point of the form
	float xSize, ySize;     // sides 
	float r, g, b;         // color
	float rBorder, gBorder, bBorder;         // color

};

Form newForm(float x, float y, float xSize, float ySize) {
	Form f = (Form)malloc(sizeof(struct form));

	f->x = x;
	f->y = y;
	f->xSize = xSize;
	f->ySize = ySize;
	f->r = (float)rand() / ((float)RAND_MAX);
	f->g = (float)rand() / ((float)RAND_MAX);
	f->b = (float)rand() / ((float)RAND_MAX);

	f->rBorder = (float)rand() / ((float)RAND_MAX);
	f->gBorder = (float)rand() / ((float)RAND_MAX);
	f->bBorder = (float)rand() / ((float)RAND_MAX);
	

	return f;
}

Form newFormC(float x, float y, float xSize, float ySize) {
	return newForm(x - (xSize / 2), y - (ySize / 2), xSize, ySize);
}

void deleteForm(Form f) {
	free(f);
}

void updateForm(Form f, float dx, float dy) {
	f->x += dx;
	f->y += dy;
}


void printfForm(Form f) {
	printf("Graphical Form BBox: (%.2f,%.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\t",
		f->x, f->y,
		f->x, f->y + f->ySize,
		f->x + f->xSize, f->y + f->ySize,
		f->x + f->xSize, f->y);


	printf("Center(%.2f,%.2f) Size{%.2f,%.2f}\n",
		f->x + (f->xSize / 2), f->y + (f->ySize / 2),
		f->xSize, f->ySize);

}


void drawForm(Form f) {
	glBegin(GL_POLYGON);
	glColor3f(f->r, f->g, f->b);
	glVertex2f(f->x, f->y);
	glVertex2f(f->x, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y);
	glEnd();


	glBegin(GL_LINE_LOOP);
	glColor3f(f->rBorder, f->gBorder, f->bBorder);
	glLineWidth(2);
	glVertex2f(f->x, f->y);
	glVertex2f(f->x, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y);
	glEnd();

	glFlush();
}

void initRandomForms(Form forms[], int n) {
	srand(time(NULL));

	for (int i = 0; i < n; i++) {
		float x = ((float)rand() / RAND_MAX) * 2.0 - 1.0; // [-1,1]
		float y = ((float)rand() / RAND_MAX) * 2.0 - 1.0; // [-1,1]
		float w = ((float)rand() / RAND_MAX) * 2.0 - 1.0; // [0.1,0.6]
		float h = ((float)rand() / RAND_MAX) * 2.0 - 1.0; // [0.1,0.6]

		forms[i] = newForm(x, y, w, h);
	}
}