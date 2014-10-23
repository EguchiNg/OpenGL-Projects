/* File: forest.c
 * 3d forest to walk through, using openGL. Has two trees, one
 *  with two cones and one with only one cone. Navigate through
 *  the forest using the left, right, up and down arrow keys.
 *
 */

#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159
#define N 10

GLfloat top1[N][3], bot1[N][3], cone[N][3], pole[3], outlineTop[N][3], outlineBot[N][3],
        outlineFoliage[N][3];

float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=5.0f;

GLfloat width, height;

float radius1, radius2, treeHeight;





GLfloat left, right, bottom, top, znear, zfar;

//Function that creates a "ground" with lines. Simple loop.
void ground(void)
{
    int i, j;

    glLineWidth(1);
    for(i = 0; i <= 100; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-50.0 + i, -1.0, 50.0);
        glVertex3f(-50.0 + i, -1.0, -50.0);
        glEnd();
    }
    for (j = 0; j <= 100; j++)
    {
        glBegin(GL_LINES);
        glVertex3f(-50.0, -1.0, 50.0 - j);
        glVertex3f(50.0, -1.0, 50.0 - j);
        glEnd();
    }
}


//Function that initializes and sets all of our vertices for use in drawing.
//Creates the top and bottom of tree trunks, the vertices for the foliage on top,
//and some vertices for outlining the trees.
void makeVertices(void)
{
    int i;
    float t;

    for(i=0; i < N; i++){
        t = i * 2 * PI / N;
        top1[i][0] = radius1 * cos(t);            
        top1[i][1] = treeHeight;
        top1[i][2] = radius1 * sin(t);
        bot1[i][0] = radius1 * cos(t);
        bot1[i][1] = -1.0;
        bot1[i][2] = radius1 * sin(t);
        
        cone[i][0] = 1.5*radius1 * cos(t);
        cone[i][1] =  0.0;
        cone[i][2] = 1.5 * radius1 * sin(t);
        
        outlineTop[i][0] = 1.01 * radius1 * cos(t);
        outlineTop[i][1] = treeHeight;
        outlineTop[i][2] = 1.01 * radius1 * sin(t);
        
        outlineBot[i][0] = 1.01 * radius1 * cos(t);
        outlineBot[i][1] = -1.0;
        outlineBot[i][2] = 1.01 * radius1 * sin(t);

        outlineFoliage[i][0] = 1.501 * radius1 * cos(t);
        outlineFoliage[i][1] = 0.0;
        outlineFoliage[i][2] = 1.501 * radius1 * sin(t);

        
    }
    pole[0] = 0.0;
    pole[1] = 1.0 + .8 * treeHeight;
    pole[2] = 0.0;

}

//Function that creates the tree stump and outlines it.
//Is called multiple times for a single tree.
//Also takes in original color so the color can be reset.
void polygon(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat v4[3],
               GLfloat outline1[3], GLfloat outline2[3], GLfloat outline3[3],
              GLfloat outline4[3], GLfloat original[3] ) {

    //A rectangle that represents just a slice of the tree
    glBegin(GL_POLYGON);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glVertex3fv(v4);
    glEnd();

    //Making lines that outline the tree.
    glColor3f(0, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex3fv(outline1);
    glVertex3fv(outline2);
    glEnd();

    
    
    glBegin(GL_LINES);
    glVertex3fv(outline2);
    glVertex3fv(outline3);
    glEnd();

    glBegin(GL_LINES);
    glVertex3fv(outline3);
    glVertex3fv(outline4);
    glEnd();

    glBegin(GL_LINES);
    glVertex3fv(outline4);
    glVertex3fv(outline1);
    glEnd();
    
    

    glColor3fv(original);
}


//Function that creates the top of the tree and outlines the bottom ring. Also takes in the original
//color so that the color can be reset.
void polygonCone(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], 
                GLfloat outline1[3], GLfloat outline2[3], GLfloat original[3]) {
        glBegin(GL_POLYGON);
        glVertex3fv(v1);
        glVertex3fv(v2);
        glVertex3fv(v3);
        glEnd();
        
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3fv(outline1);
        glVertex3fv(outline2);
        glEnd();

        glColor3fv(original);

}

//Function that calls the creation of the foliage by making many triangles.
//Also takes in the original color for resetting.
void drawCone(GLfloat originalColor[3]) {
        int i,k;

        glColor3fv(originalColor);
        for (i = 0; i < N; i++){
                k = i+1;
                if(k == N)
                        k = 0;
                polygonCone(cone[i], cone[k], pole, outlineFoliage[i], outlineFoliage[k], originalColor);
        }

        glColor3fv(originalColor);

}

//Function that calls the creation of the stumps by creating many rectangles.
//Also takes in the original color for resetting.
void drawCylinder(GLfloat originalColor[3])
{
    glColor3fv(originalColor);
    int i,k;
    for( i = 0; i < N; i++){
        k = i+1;
        if(k == N)
            k = 0;
        polygon(bot1[i], bot1[k], top1[k], top1[i], outlineBot[i], outlineBot[k], outlineTop[k], outlineTop[i], originalColor);
    }

}
//Function that calls the two drawCylinder and drawCone functions
//to create a tree at the current location. Also takes in original color
//and gives the color of green to the drawCone function. Only has one cone on top.
void drawTree(GLfloat originalColor[3]) {
    drawCylinder(originalColor);
    glPushMatrix();
    glTranslatef(0.0, treeHeight, 0.0);
    GLfloat green[3] = {.4,.5,0.2};
    drawCone(green);
    glPopMatrix();

}


//Function that calls the two drawCylinder and drawCone functions
//to create a tree at the current location. Also takes in original color
//and gives the color of green to the drawCone function. Consists of a stump
//and two cones on top.
void drawTree2(GLfloat originalColor[3]) {
    drawCylinder(originalColor);
    glPushMatrix();
    glTranslatef(0.0, treeHeight, 0.0);
    GLfloat green[3] = {.4,.5,0.2};
    drawCone(green);
    glTranslatef(0.0, .5 * treeHeight, 0.0);
    drawCone(green);
    glPopMatrix();
    glPopMatrix();

}

//Display function called after every redisplay
void display(void){
    

    //Sky blue
    glClearColor(0.670, 0.804, 0.847, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    

    glLoadIdentity();

    //Green
    glColor3f(.4,.5,0.2);
    
    //Adjust where the view is  
    gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);

    //Draw the ground
    ground();
    glColor3f(.204, .106, .047);
    glBegin(GL_POLYGON);
    glVertex3f(-50, -1.01, -50);
    glVertex3f(50, -1.01, -50);
    glVertex3f(50, -1.01, 50);
    glVertex3f(-50, -1.01, 50);
    glEnd(); 

    //Create the brown color, translate to the left corner and then create the forest iteratively.
    GLfloat brown[3] = {.204, .106, .047};
    
    glTranslatef( -40.0, 0.0, -40.0);
    for(int i = 0; i < 10; i++)
    {   
        
        for(int j = 0; j < 20; j++){
            
            drawTree(brown);
            glTranslatef(5.0, 0.0, 0.0);
            drawTree2(brown);
            glTranslatef(5.0, 0.0, 0.0);
        }
        glTranslatef(-5.0 * 20 * 2, 0.0, 5.0);
        

        

    }
    

    glutSwapBuffers();
}

//Reshape function for adjusting window 
void reshape(int w, int h)
{
    GLfloat ratio = (GLfloat)w / (GLfloat)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
		glFrustum(left, right, bottom * ratio, 
			top * ratio, znear, zfar);
	else
		glFrustum(left / ratio, right / ratio, 
			bottom, top, znear, zfar);

	glMatrixMode(GL_MODELVIEW);
}

//Keyboard that just allows for exit through escape key
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:             // ESCAPE key
      
	  exit (0);
	  break;
  }
}


//Adjusts the viewing angle using the left, right, up and down arrow keys.
void myKeyboardSpecial(int key, int xx, int yy) {

	float fraction = 0.25f;

    
	switch (key) {
		case GLUT_KEY_LEFT :
			angle -= 0.15f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.15f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;
	}

    glutPostRedisplay();
}

//Initialization stuff
void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(left, right, bottom, top, znear, zfar);
    glMatrixMode(GL_MODELVIEW);
}
//Standard main
int main(int argc, char **argv)
{

    
    //Set the variables for the viewing camera
    left = -1.0;
    right = 1.0;
    bottom = -1.0;
    top = 1.0;
    znear = 1.5;
    zfar = 20.0;

    //Set radius for trees
    radius1=0.8;
    
    //Set tree heights
    treeHeight = 2.0;
    makeVertices();
    

    width = 800;
    height = 800;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Forest");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(myKeyboardSpecial);
    
    glEnable(GL_DEPTH_TEST);
    init();
    glutMainLoop();


    return 0;


}


