/*
circles.c

This simple program demonstrates one of the ways to display a circle
in OpenGL, with the use of GL_TRIANGLE_FAN.
Other new features are the use of trigonometry that requires <math.h>
and the randomization of the seed for the random number generator,
which is achieved via current time, and requires <time.h>

Many comments are carried over from the glintro1.c program.

LP: 02/12/14 (Demo for cs350s14)
*/

#include <GL\glut.h>					// include GL Utility Toolkit
#include <stdlib.h>						// for rand()
#include <math.h>						// for sin(), cos()
#include <time.h>						// for srand()

#define N 20							// number of circles
#define W 800				
#define H 800
#define MAXRADIUS 360					// radius of the largest circle
#define NSEGMENTS 100						// number of circle segments
#define PI 3.14159

/* The initialization function sets up things that need to be done
regardless of display function */
void init(void)
{
	/* Set up the clearing color to black; clearing color means
	   background color. Note that the fourth element of color
	   is transparency */
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* The four following statements set up the viewing rectangle */
	glMatrixMode(GL_PROJECTION);		// use projection matrix
	glLoadIdentity();					// load identity matrix
	gluOrtho2D(0, W, 0, H);	
	glMatrixMode(GL_MODELVIEW);			// back to modelview matrix
}

/* The display callback function. It will be called ("automatically")
   when there is a need to display the window */
void ourDisplay(void)
{
	GLfloat r, g, b;					// needed for random colors
	GLfloat centerx = W/2, centery = H/2;
	GLint radius[N];					// radii of circles
	GLfloat phi;

	/* Clear the color buffer, i.e., fill it with clearing color */
	glClear(GL_COLOR_BUFFER_BIT);

	/* randomize the seed to have a different color combination each
	   time the program is running. */
	srand(time(NULL));

	/* Display many concentric circles in random colors */
	for (int i = 0; i < N; i++)
	{
		r = (rand()%100)/100.0;			// generate random color
		g = (rand()%100)/100.0;
		b = (rand()%100)/100.0;
		/* Compute the radius of the i-th circle */
		radius[i] = MAXRADIUS - i*MAXRADIUS/N;
		glColor3f(r, g, b);
		/* Build the circle made of circular segments */
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(centerx, centery);
		for (int j = 0; j <= NSEGMENTS; j++)
		{
			phi = j * 2* PI/NSEGMENTS;
			GLfloat x1 = centerx + radius[i]*cos(phi);
			GLfloat y1 = centery + radius[i]*sin(phi);
			glVertex2f(x1, y1);
		}
		glEnd();
	}	

	/* Flush the picture to the screen */
	glFlush();
}

/* The main function */
int main(int argc, char *argv[])
{
	/* Establish communication between OpenGL and the windowing
	   system */
	glutInit(&argc, argv);

	/* Set up display mode; in this case it is single buffering
	   (display goes directly to the screen) and the RGB color 
	   mode. */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(700, 700);		// obvious

	glutInitWindowPosition(0, 0);		// obvious

	glutCreateWindow("circles");		// window title

	/* The following is the callback handler */
	glutDisplayFunc(ourDisplay);

	/* Call the initialization function */
	init();

	/* Start the main loop that waits for events to happen and
	   then to process them */
	glutMainLoop();
}
