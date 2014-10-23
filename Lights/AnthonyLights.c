/* File: AnthonyLights.c
 * Program that simulates a worm and then lights it using three separate lights, and allows rotation.
 * Rotate around the worm with left click and zoom in and out by holding right click. Toggle different light
 * configurations using the 1, 2, 3, 4, 5 keys. 
 *
 * Command line arguments are number of slices and stacks to create the circles of the worm with
 *
 */


#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

GLfloat lastX, lastY;
int leftState, rightState;
int toggle0, toggle1, toggle2;
 
float xrot = 0.0f;
float yrot = 0.0f;
float zrot = 1.0f;
 
float xdiff = 0.0f;
float ydiff = 0.0f;
float zdiff = 0.0f;

int slices, stacks;
int height = 800;
int width = 800;


void setLights(void) {
    glEnable(GL_LIGHTING);
    if(toggle0 == 1) {
     /* Specify light properties */
	GLfloat light_ambient[]={0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[]={0.5, 0.9, 0.7, 1.0};
    GLfloat light_specular[]={0.8, 0.2, 0.4, 1.0};
	/* Position of Light 0: */
	GLfloat light0_pos[] = {5.0,6.0,7.0,1.0};

	/* Set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	
    glEnable(GL_LIGHT0);

    }

    if(toggle1 == 1) {
     /* Specify light properties */
	GLfloat light_ambient1[]={0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse1[]={0.5, 0.9, 0.2, 1.0};
    GLfloat light_specular1[]={0.8, 0.7, 0.0, 1.0};
	/* Position of Light 0: */
	GLfloat light1_pos[] = {1.0,-1.0,7.0,1.0};

	/* Set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);


    glEnable(GL_LIGHT1);
    }

    if(toggle2 == 1) {

        	/* Specify light properties */
	GLfloat light_ambient2[]={0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse2[]={0.5, 0.2, 1.0, 1.0};
    GLfloat light_specular2[]={0.8, 0.1, 1.0, 1.0};
	/* Position of Light 0: */
	GLfloat light2_pos[] = {0.0,0.0,-4.0,1.0};

	/* Set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);


    glEnable(GL_LIGHT2);
    }

    //Choose which lights are displayed based on toggles
    if(toggle0 == 0)
            glDisable(GL_LIGHT0);
    if(toggle1 == 0)
            glDisable(GL_LIGHT1);
    if(toggle2 == 0)
            glDisable(GL_LIGHT2);
}
/*
 *Standard display callback
 */
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 0.0f);

    setLights();
    	


    
    
    glutSolidSphere(2, slices, stacks);
    glTranslatef(-1.8, 0.0, 0.0);
    glutSolidSphere(2, slices, stacks);
    glTranslatef(-1.8, 0.0, 0.0);
    glutSolidSphere(2, slices, stacks);
    glTranslatef(-1.9, 0.0, 0.5);
    glColor3f(1.0, 0.2, 0.1);
    glutSolidSphere(1, slices, stacks);
    glTranslatef(0.0, 0.0, -1.0);
    glutSolidSphere(1, slices, stacks);
    
    glFlush();

    

}


/*
 * Mouse callback that adjusts xrotations, y rotations and z rotations
 */
void mouse(int button, int state, int x, int y)
{


    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        leftState = 1;
        xdiff = x - yrot;
        ydiff = -y + xrot;
    }
    else{
        
        leftState = 0;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
            rightState = 1;
            
            zdiff = -y + xrot;
    }
    else{
            rightState = 0;
    }
    

}

/**
 * Motion function that captures rotation
 */
void motion(int x, int y){

   
    
    if(leftState==1){
        yrot = x - xdiff;
        xrot = y + ydiff;

    }
    //Adjust zoom using right click
    if(rightState==1) {
        zrot = y - zdiff;
    }


    glutPostRedisplay();
    


}

/*
 * Standard reshape function
 */
void reshape(int w, int h )
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(w <= h)
        glOrtho(-10.0, 10.0, -10.0 * (GLfloat) h / (GLfloat) w,
                10.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-10.0 * (GLfloat) w / (GLfloat) h,
                10.0 * (GLfloat) w / (GLfloat) h, -10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    display();
}

void keyboard(unsigned char key, int x, int y) {
    switch((int) key){
            case 49:
                    toggle0=1;
                    toggle1=1;
                    toggle2=1;
                    break;
            case 50:
                    toggle0=0;
                    toggle1=1;
                    toggle2=1;
                    break;
            case 51:
                    toggle0=0;
                    toggle1=0;
                    toggle2=1;
                    break;
            case 52:
                    toggle0=1;
                    toggle1=0;
                    toggle2=1;
                    break;
            case 53:
                    toggle0=1;
                    toggle1=1;
                    toggle2=0;
                    break;
    }

    glutPostRedisplay();


}
/*
 *Initialize some standard values
 */
void init(void)
{

    /* Specify material properties */
    GLfloat mat_specular[]={1.0, 1.0, 1.0, 1.0};
    GLfloat mat_diffuse[]={1.0, 1.0, 1.0, 1.0};
    GLfloat mat_ambient[]={1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess={50.0};
    	
	

	/* Define material properties for front face of all polygons */
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glColor3f(1.0, 0, 0);
    leftState = 0;
    rightState = 0;
    toggle0 = 1;
    toggle1 = 1;
    toggle2 = 1;

}



int main(int argc, char **argv)
{
    if (argc != 3)
	{
		printf("\nUse AnthonyLights  slices[int]  stacks[int]\n\n");
		exit(0);
	}
    slices = atoi(argv[1]);				
	stacks = atoi(argv[2]);	

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Lights and Worms");
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    init();
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMainLoop();
}
