/*
 *  Programul utilizeaza biblioteca GLUT
 *  pentru redarea unei curbe spline
 */
#include "glos.h"

#include "gl.h"
#include "glu.h"
#include "glaux.h"

GLUnurbsObj* theNurb; // curba este un obiect de tipul GLUnurbsObj

void myinit()
{
	glShadeModel(GL_FLAT);
	theNurb = gluNewNurbsRenderer(); // obiectul de tip GLUnurbsObj
	// este creat dinamic in memoria libera prin apelul functiei gluNewNurbsRenderer()
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
	// eroarea de esantionare a curbei la redare
}


void CALLBACK display()
{	// 4 puncte de control
	GLfloat ctlpoints[4][3] = {
		{-.75, -.75, 0.0},
		{-.5, .75, 0.0},
		{.5, .75, 0.0},
		{.75, -.75, 0.0}
	};

	// 8 noduri
	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	//   GLfloat knots[8] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0); // culoarea curenta de desenare
	// incepe corpul de redare al curbei Spline
	gluBeginCurve(theNurb);
	gluNurbsCurve(theNurb,	// pointer obiect NURBS
		8, knots,			// numar noduri, tablou noduri
		sizeof(ctlpoints[0]) / sizeof(GLfloat), // intervalul de valori dintre doua puncte de control consecutive
		&ctlpoints[0][0],	// vector puncte de control
		4,					// ordinul curbei 
		GL_MAP1_VERTEX_3);	// tip evaluator
	gluEndCurve(theNurb);

	glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) {
		gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
	}
	else {
		gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_SINGLE | AUX_RGB);
	auxInitPosition(0, 0, 500, 500);
	auxInitWindow("Curba B-spline");
	myinit();
	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return 0;
}
