#include "form.h"
#include <stdlib.h>
#include <GL/glut.h>

#define N 10

// Array of pointers to form objects (Recall, Form is a pointer!)
Form a[N];

drawFormCount = 0;

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < N; i++) {
		if (a[i] != NULL) {
			drawForm(a[i]);
		}
		drawFormCount++;
	}
	printf("Draw Count: %d\n", drawFormCount);
}

void main(int argc, char** argv) {
	int i;

	// Simple example: create one form, print it and delete it at the end
	/*Form f = newForm(0, 0, 5, 5);
	printfForm(f);
	deleteForm(f);
	*/
	// initialize array of forms (pointers to forms) to NULL
	for (i = 0; i < N; i++)
		a[i] = NULL;

	initRandomForms(a, N);

	// print the content of the array
	for (i = 0; i < N; i++)
		if (a[i] != NULL)
			printfForm(a[i]);

	glutInit(&argc, argv);   
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutCreateWindow("simple");
	glutDisplayFunc(myDisplay);
	glutMainLoop();

}