/* File:rotatingshape
 * Modeling a 3d object, a 3d I. Mouse clicks switch between 2x2 to 4x4 
 * remodeling of the object.
 */


#include <stdlib.h>
#include <GL/glut.h>

static GLfloat theta[] = {0.0, 0.0, 0.};
static GLint axis = 2;
int scaling;

GLfloat width, height;

//Top of our shape
GLfloat vertices[][3] = {{-1.5,0.5,-1.0},{1.5,0.5,-1.0},
		{1.5,1.0,-1.0}, {-1.5,1.0,-1.0}, {-1.5,0.5,1.0}, 
		{1.5,0.5,1.0}, {1.5,1.0,1.0}, {-1.5,1.0,1.0}};

//middle

GLfloat vertices2[][3] = {{-.75, -0.5, -1.0}, {.75, -0.5, -1.0},
        {.75, 0.5, -1.0}, {-.75, 0.5, -1.0}, {-.75, -0.5, 1.0},
        {.75, -0.5, 1.0}, {.75, 0.5, 1.0}, {-.75, 0.5, 1.0}};

//Bottom
GLfloat vertices3[][3] = {{-1.5,-1.0,-1.0},{1.5,-1.0,-1.0},
		{1.5,-0.5,-1.0}, {-1.5,-0.5,-1.0}, {-1.5,-1.0,1.0}, 
		{1.5,-1.0,1.0}, {1.5,-0.5,1.0}, {-1.5,-0.5,1.0}};


GLfloat colors[][3] = {{0.0, 0.0, 0.0}, {1.0, -1.0, -1.0},
    {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0},
    {1.0, -1.0, 1.0}, {0.25, 0.5, 0.75}, {-1.0, 1.0, 1.0}};


/* Function that takes in 4 ints, the vertices, and draws the corresponding
 * vertices in the vertex matrices
 *
 */
void face(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    glColor3fv(colors[b]);
    glVertex3fv(vertices[a]);

    glVertex3fv(vertices[b]);

    glVertex3fv(vertices[c]);
   
    glVertex3fv(vertices[d]);
    glEnd();

    //Makin da middle
    glBegin(GL_POLYGON);
    
    glVertex3fv(vertices2[a]);

    glVertex3fv(vertices2[b]);

    glVertex3fv(vertices2[c]);

    glVertex3fv(vertices2[d]);
    glEnd();

    //Makin da bottom
    glBegin(GL_POLYGON);
    glVertex3fv(vertices3[a]);

    glVertex3fv(vertices3[b]);

    glVertex3fv(vertices3[c]);

    glVertex3fv(vertices3[d]);
    glEnd();

}

/* Function that calls face with the proper vertices to create our I.
 *
 */
void shape(void)
{
    
    //back
    face(0,3,2,1);
    //top
    face(7,6,2,3);
    //left side
    face(0,4,7,3);
    //right side
	face(1,2,6,5);
    //front
	face(4,5,6,7);
    //bottom
	face(0,1,5,4);

    

}

/* Reshape function that is called whenever the window needs to be resized. Also changes the width and height global variables
 *
 *
 */
void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Sets the orthographic coordinates of the window, depending whether the width is less than height
    //or the height is less than the width.
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
            2.0 * (GLfloat) h / (GLfloat) w, -2.0, 2.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
            2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
}


/* Idle function that modifies the angle and redisplays the shape with the new angles.
 *
 */
void idle()
{
    theta[axis] += .05;
    if(theta[axis] > 360.0 ) theta[axis] -= 360.0;
    glutPostRedisplay();
}

/* Mouse function that reduces scaling to a minimum of 2 and increases
 * scaling by a maximum of 4. Allows for 2x2,3x3 or 4x4.
 */
void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        if(scaling > 2) scaling -=1;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        if(scaling < 4) scaling +=1;
    glutPostRedisplay();
}

/* Display function that handles redrawing of every shape everytime the redisplay is called.
 * 
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    float scale = .3/(scaling*scaling);
    int widthInterval = width/scaling;
    int heightInterval = height/scaling;
    int interval = 0;

    //First for loop that draws a shape for each column
    for (int i = 0; i < scaling; i ++)
    {
        
    
        //Second for loop that draws every shape in a column, row by row
        for(int j = 0; j < scaling; j++) {
            int theta0 = (theta[0] + 10*(i+j));
            int theta1 = (theta[1] + 10*(i+j));
            int theta2 = (theta[2] + 10*(i+j));
            

            //Rotate around each axis by the degree
            glRotatef(theta0, 1.0, 0.0, 0.0);
            glRotatef(theta1, 0.0, 1.0, 0.0);
            glRotatef(theta2, 0.0, 0.0, 1.0);

            //Scales the shape
            glScalef(1.0f / (1.0 + scale * interval),1.0f / (1.0 + scale * interval),1.0f / (1.0 + scale * interval));
            //Changes the viewport to the appropriately sized window
            glViewport( widthInterval * i,  heightInterval*j, widthInterval, heightInterval);
            //Call shape function
            shape();
            interval++;
        }


        
        
        
    }
    glutSwapBuffers();
}

int main(int argc, char **argv)
{

    width = 800;
    height = 800;
    scaling = 2;
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Program");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}


