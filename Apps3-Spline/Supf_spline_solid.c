/*
 *  Supf_spline_solid.c
 *  Afiseaza o suprafata spline 
 *   folosind biblioteca GLUT
 */
#include "glos.h"

#include "gl.h"
#include "glu.h"
#include "glaux.h"

void myinit(void);
void init_surface(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

GLfloat ctlpoints[4][4][3];

GLUnurbsObj *theNurb;

/*
 *  Initializeaza punctele de control pentru o suprafata
 *  de forma unei adancituri simetrice
 *  Domeniul punctelor de control intre -3 si +3 in x, y, si z
 */
void init_surface(void)
{
    int u, v;
    for (u = 0; u < 4; u++) {
    for (v = 0; v < 4; v++) {
        ctlpoints[u][v][0] = 2.0*((GLfloat)u - 1.5);
        ctlpoints[u][v][1] = 2.0*((GLfloat)v - 1.5);

        if ( (u == 1 || u == 2) && (v == 1 || v == 2))
        ctlpoints[u][v][2] = 3.0;
        else
        ctlpoints[u][v][2] = -3.0;
    }
    }               
}               
            
/*  Initializarea buffer-ului de adancime si a atributelor 
    materialului si sursei de lumina
 */
void myinit(void)
{
    GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glClearColor (0.0, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    init_surface();

    theNurb = gluNewNurbsRenderer(); // crearea obiectului suprafata spline
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0); 
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

void CALLBACK display(void)
{
	// punctele de control
    GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(330.0, 1.,0.,0.);
    glScalef (0.5, 0.5, 0.5);

    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb, 
        8, knots,
        8, knots,
        4 * 3,
        3,
        &ctlpoints[0][0][0], 
        4, 4,
        GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);
        
    glPopMatrix();
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);
}

/*  Main Loop 
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Suprafata B-Spline");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
