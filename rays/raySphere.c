/*
 * File: raySphere.c
 * Program for showing rudimentary ray casting a sphere which is centered at (0,0,0), with input from the
 * user for the radius of the sphere.
 *
 */


#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
float radius;
//Location of light and its rgb values
float xL, yL, zL, rL, gL, bL;



float ortho = 1;
float kd = 0.2;
#define WIDTH 700
#define HEIGHT 700


/*
 * Standard init
 */
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(-ortho, ortho, -ortho, ortho);


}





//Equation of our sphere will be x^2 + y^2 + z^2 = R^2.
//The sphere is centered at (0, 0, 0) so any point (x, y, z) of the sphere
//must fit with the equation above.
//Viewing plane is z = 1.
//Eyepoint is at 0, 0, 2.
//The equation for the straight line through EV is
//x = xv * t
//y = yv * t
//z = d - d*t
//Where d = -1, and xv and yv are the coordinate of the pixel on the screen
//We want to iterate through every pixel on the screen
//and check to see if a ray through that pixel will intersect with the object.
//Substituting into the line of the equation we get
//t^2(xv^2 + yv^2 + 1) + t(-4) + (4-R^2)
//From this we calculate the discriminant,
//B^2 - 4AC
//which is 16 - 4(xv^2 + yv^2)(4-R^2)
//

/**
 * Function that determines if the ray goes through the polygon.
 * Params:
 * xv: The x coordinate in the plane that we are testing
 * xy: the y coordinate in the plane that we are testing
 */
int intercept(float xv, float yv) {
    float discriminant;
    float B = -4;
    float A = xv * xv + yv * yv + 1;
    //printf("A = %f\n", A);
    float C = 4 - radius * radius;
    //printf("C = %f\n", C);
    discriminant = (B * B) - (4 * A * C);
    
    
    if(discriminant > 0){
        
        //Solve for t if this happens, where t = (4 + sqrt(discriminant)) / 2 * A
        //and (4 - sqrt(discriminant)) / 2 * A
        float t1 = ((4 + sqrt(discriminant)) / (2 * A));
        float t2 = ((4 - sqrt(discriminant)) / (2 * A));
        if(t2 > t1)
            t1 = t2;
        float x, y, z, normalX, normalY, normalZ;
        //Calculate points from equation of the line
        x = 0 + t1*xv;
        y = 0 + t1*yv;
        z = 2 + t1*-1;

        float normalMagnitude = sqrt(x * x + y * y + z * z);

        //Normalize the points
        normalX = x / normalMagnitude;
        normalY = y / normalMagnitude;
        normalZ = z / normalMagnitude;
        
        //Get magnitude of the light source's vector
        float lightMag = sqrt(xL * xL + yL * yL + zL * zL);
        //Normalize the lights
        float normalxL, normalyL, normalzL;
        normalxL = xL / lightMag;
        normalyL = yL / lightMag;
        normalzL = zL / lightMag;
        
        //Distance calculation from point to light source. Calculates the sqr of distance for attenuation
        float distance = sqrt((normalX - normalxL) * (normalX - normalxL) +
                        (normalY - normalyL) * (normalY - normalyL) +
                        (normalZ - normalzL) * (normalZ - normalzL));


        //Calculate dot product of light and normal vector
        float dot = normalX*normalxL + normalY*normalyL + normalZ*normalzL;

        float red, green, blue;
        red = rL * kd * lightMag * dot / (1.0/distance);
        green = gL * kd * lightMag * dot / (1.0/distance);
        blue = bL * kd * lightMag * dot / (1.0/distance);
        
        glColor3f(red, green, blue);
        glBegin(GL_POINTS);
        glVertex2f(xv,yv);
        glEnd();

        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Display function.
 */
void display(void) {

    //Iterate through every pixel in the window
    for(int i = 0; i < WIDTH; i++){
        float xv = (float)i/WIDTH * 2 * ortho - ortho;
        for(int j = 0; j < HEIGHT; j++){
            
            float yv = (float)j/HEIGHT * 2 * ortho - ortho;

            intercept(xv, yv);

        }
    }

    glFlush();



}

/*
 * Simple keyboard function for exiting
 */
void keyboard(unsigned char key, int x, int y) {

        if(key == 27)
        {
                exit(0);
        }


}

int main(int argc, char **argv) {

    if(argc != 8)
    {
        printf("Program requires 7 arguments:\n Radius \n");
        printf("position of light xL, yL, zL \n Light color rL, gL, bL\n");

    }

    else {
        //Assign all the values according to user input.
        radius = atof(argv[1]);
        if(radius > 1){
            printf("Radius too big. Please give a radius <= 1.\n");
            exit(0);
        }
        //Light Location
        xL = atof(argv[2]);
        yL = atof(argv[3]);
        zL = atof(argv[4]);
        //Light colors
        rL = atof(argv[5]);
        gL = atof(argv[6]);
        bL = atof(argv[7]);
    }

    

    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    
    glutCreateWindow("hw9");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);




    glutMainLoop();



    return 0;
}





