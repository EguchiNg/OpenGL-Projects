/* File: ballPhysicsAnthony.c
 * Author: Anthony Nguyen
 * Date: 2/24/2014
 *
 *
 * Description: Program that displays from 1-10 circles, depending on 
 * user input, which have random speeds and random colors. Shows elastic collisi
 * on with other balls and
 * the wall.
 */

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define RADIUS 25
#define NSEGMENTS 100
#define PI 3.1415926535
int numCircles;

float colorArray[30];
float centerArray[20];
float vectorArray[20];
float waitTime;

void display(void);
void drawCircle(float, float, float);


/*Function: init
 * init function that sets up all of our values before beginning animation.
 */
void init(void){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    waitTime = 25;
    
    
    //Setting initial centers of circles.
    //CenterArray has x and y coordinates next to each other
    //for each circle.
    for (int i = 0; i < numCircles*2; i+=2){
        colorArray[i] = (double)rand() / (double)RAND_MAX;
        colorArray[i+1] = (double)rand() / (double)RAND_MAX;
        colorArray[i+2] = (double)rand() / (double)RAND_MAX;
        if (i <= 8) {
            centerArray[i] = .25*WIDTH;
            centerArray[i+1] = .7 * HEIGHT - ( RADIUS * i);
        }
        else{
            centerArray[i] = (.45)*WIDTH;
            centerArray[i+1] = .7 * HEIGHT - (RADIUS * (i%10) );
        }
        
    }


    //Set up the vector array using random numbers.

    for (int i = 0; i < numCircles*2; i+= 2){
        vectorArray[i] = (double)rand() / (double)RAND_MAX*2;
        vectorArray[i+1] = (double)rand() / (double)RAND_MAX*2;

        

    }
    
    glutPostRedisplay();
}

/* Function: display
 * display callback that draws our enclosure and the circles.
 */
void display(void) {

    //Clear screen and then draw our rectangular enclosure.
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5, 0.3, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(.2*WIDTH, .8*HEIGHT);
    glVertex2f(.8*WIDTH, .8*HEIGHT);
    glVertex2f(.8*WIDTH, .2*HEIGHT);
    glVertex2f(.2*WIDTH, .2*HEIGHT);
    glEnd();

    //Set random colors for each circle and draw them.
    for (int i = 0; i < numCircles*2; i+=2){
        glColor3f(colorArray[i], colorArray[i+1], colorArray[i+2]);
        drawCircle(centerArray[i], centerArray[i+1], RADIUS);
    }

    
    glFlush();

}

/*function: drawCircle
 * function that is called to draw a circle on openGL. 
 * Helps to keep the code clean.
 * float x : x position
 * float y : y position
 * int radius : radius of the ball
 */
void drawCircle(float x, float y, float radius) {
    GLfloat phi, x1, y1;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for(int j = 0; j <= NSEGMENTS; j++){
        phi = j * 2 * PI/NSEGMENTS;
        x1 = x + radius * cos(phi);
        y1 = y + radius * sin(phi);
        glVertex2f(x1, y1);
    }
    glEnd();


}
/* function: mouse
 * mouse handling callback that speeds up on right click
 * and slows down on left click.
 * butt - which button is pressed
 * state - state of the mouse
 * x - x coordinate of click
 * y - y coordinate of click 
 */
void mouse(int butt, int state, int x, int y){

    //Increase speed by 2
    if (state == GLUT_DOWN && butt == GLUT_LEFT_BUTTON)
		waitTime*=2;

	if (state == GLUT_DOWN && butt == GLUT_RIGHT_BUTTON)
		waitTime/=2;

    

}

/* Function: timerFunc
 * timer that recalculates changes in the ball's position
 * and handles collision.
 * value - unused
 */
void timerFunc(int value){
    for(int i = 0; i < numCircles*2; i+=2){
        float x1, x2, y1, y2;
        x1 = centerArray[i];
        y1 = centerArray[i+1];
        
        

        //MATH STUFF computing collisions between the current ball with every other ball and adjusting accordingly

        for(int j = i+2; j < numCircles*2; j+=2){
            //Compute the vector between the center of two balls
            x2 = centerArray[j];
            y2 = centerArray[j+1];
            float vectorx = x1 - x2;
            float vectory = y1 - y2;
            
            
            
            //Compute the square of the vector's magnitude
            float dSq = (vectorx * vectorx) + (vectory * vectory);
            float radius = RADIUS;
            float check = radius*radius*4;
            
            if(dSq <= check){
                
                //Collision, compute vector of difference of velocities
                float dVx = vectorArray[j] - vectorArray[i];
                float dVy = vectorArray[j+1] - vectorArray[i+1];
                //Compute dot product of vector between centers and dv
                float dP = (vectorx * dVx) + (vectory * dVy);

                if(dP > 0)
                {
                   
                    //Balls moving toward each other.
                    float factor = dP / dSq;
                    float xChange = vectorx * factor;
                    float yChange = vectory * factor;
                    //Modify the vectors based on collision.
                    vectorArray[i] = vectorArray[i] + xChange;
                    vectorArray[i+1] = vectorArray[i+1] + yChange;
                    vectorArray[j] = vectorArray[j] - xChange;
                    vectorArray[j+1] = vectorArray[j+1] - yChange;

                }
            }
            
        }
        //Changing x and checking for collision with the wall.
        if(x1 > (.8 * WIDTH - RADIUS) || x1 < (.2 * WIDTH + RADIUS))
            vectorArray[i] = -vectorArray[i];
        centerArray[i] = x1 + vectorArray[i];
        //Changing y and checking for collision with the wall
        if(y1 > (.8 * HEIGHT - RADIUS) || y1 < .2*HEIGHT + RADIUS)
            vectorArray[i+1] = -vectorArray[i+1];
        centerArray[i+1] = y1 + vectorArray[i+1];
        
    }
    glutPostRedisplay();
    //Prevent from going too high or too low
    if(waitTime>1)
        glutTimerFunc(waitTime, timerFunc, 1);
    else if(waitTime > 3200)
        glutTimerFunc(3200, timerFunc, 1);
    else
        glutTimerFunc(1, timerFunc, 1);
}
/*Function: keyboard
 * Simple keyboard function that takes in keyboard input
 * and closes the program if the key is esc.
 * key - key that is pressed
 * int x, y - unused
 * */
void keyboard(char key, int x, int y) {

    if((int)key == 27)
            exit(0);
}

int main(int argc, char *argv[]) {
    //Get user input for amount of balls and set up all our callbacks/windows
    if(argc > 1)
        numCircles = atoi(argv[1]);
    else
        numCircles = 2;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Bouncing Balls");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(waitTime, timerFunc, 1);

    init();

    glutMainLoop();
}

