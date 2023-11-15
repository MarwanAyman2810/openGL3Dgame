#include <glut.h>

// Initialize OpenGL Graphics
void initGL() {
    glEnable(GL_LIGHTING);   // Enable lighting
    glEnable(GL_LIGHT0);     // Enable light #0
    glEnable(GL_NORMALIZE);  // Automatically normalize normals
    glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
    glEnable(GL_COLOR_MATERIAL);

    // Set the light parameters
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat lightDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightPosition[] = { 2.0, 5.0, 5.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glClearColor(1.0, 1.0, 1.0, 1.0); // Set background color to white
}
void drawShoe() {
    glColor3f(0.3, 0.3, 0.3); // Dark grey color for the shoe
    glPushMatrix();
    glTranslatef(-0.21, -1.0, 0.0); // Position the shoe at the bottom of the right leg

    // Shoe Base
    glPushMatrix();
    glScalef(0.15, 0.05, 0.25);
    glutSolidCube(1.0);
    glPopMatrix();

    // Shoe Toe Cap
    glColor3f(0.4, 0.4, 0.4); // Slightly lighter grey for the toe cap
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.125);
    glScalef(0.15, 0.05, 0.1);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    // Shoe Heel
    glColor3f(0.25, 0.25, 0.25); // Slightly darker grey for the heel
    glPushMatrix();
    glTranslatef(0.0, -0.025, -0.125);
    glScalef(0.15, 0.1, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Laces Area
    glColor3f(0.3, 0.3, 0.3); // Matching the shoe color
    glPushMatrix();
    glTranslatef(0.0, 0.025, 0.05);
    glScalef(0.15, 0.1, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();

    // Individual Laces
    for (int i = 0; i < 5; i++) {
        glColor3f(1.0, 1.0, 1.0); // White color for the laces
        glPushMatrix();
        glTranslatef(0.0, 0.03, 0.025 - i * 0.05);
        glScalef(0.12, 0.01, 0.01);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Side Details
    for (int i = 0; i < 3; i++) {
        glColor3f(0.4, 0.4, 0.4); // Slightly lighter grey for side details
        glPushMatrix();
        glTranslatef(0.075 - i * 0.075, 0.0, 0.075 - i * 0.075);
        glRotatef(45, 0, 1, 0);
        glScalef(0.01, 0.05, 0.15);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Shoe Tongue
    glColor3f(0.35, 0.35, 0.35); // A different shade of grey for contrast
    glPushMatrix();
    glTranslatef(0.0, 0.05, 0.125);
    glScalef(0.15, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    // Shoe Sole
    glColor3f(0.2, 0.2, 0.2); // Very dark grey for the sole
    glPushMatrix();
    glTranslatef(0.0, -0.05, 0.0);
    glScalef(0.15, 0.01, 0.25);
    glutSolidCube(1.0);
    glPopMatrix();

    // End of the shoe drawing
    glPopMatrix();
}
void drawHand() {
    // Set the color for the hand
    glColor3f(1.0, 0.85, 0.75); // Skin color for the hand

    // Draw the palm
    glPushMatrix();
    glTranslatef(-0.5, -1, 0.0); // Adjust position for the palm
    glScalef(0.2, 0.1, 0.0);
    glutSolidCube(1.0);
    glPopMatrix();

    //// Draw the thumb
    //glPushMatrix();
    //glTranslatef(-0.5, -.95, 0.0); // Adjust position for the thumb
    //glScalef(0.03, 0.1, 0.1);
    //glutSolidCube(1.0);
    //glPopMatrix();

    // Draw the four fingers
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(-0.579 + i * 0.05, -0.95, 0.0); // Adjust position for the fingers
        glScalef(0.03, 0.11, 0.1);
        glutSolidCube(1.0);
        glPopMatrix();
    }
}


// Function to draw the human figure
static void drawHuman() {
    glPushMatrix();

    // Scale down the entire figure
    glScalef(0.5, 0.5, 0.5);

    // Draw head
    glColor3f(1.0, 0.85, 0.75); // Skin color for the head
    glTranslatef(0.0, 1.5, 0.0);
    glutSolidSphere(0.25, 20, 20);

    // Draw body
    glColor3f(0.0, 0.0, 1.0); // Blue color for the body
    glTranslatef(0.0, -0.6, 0.0);
    glScalef(1.0, 1.5, 0.5);
    glutSolidCube(0.5);

    // Draw right side of shorts
    glColor3f(1.0, 0.0, 0.0); // Color for the shorts
    glPushMatrix();
    glTranslatef(-0.15, -0.45, 0.0); // Adjust position to the right side
    glRotatef(-15, 0, 0, 1); // Reverse tilt
    glScalef(0.23, 0.5, 0.3); // Increase thickness
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw left side of shorts
    glPushMatrix();
    glTranslatef(0.15, -0.45, 0.0); // Adjust position to the left side
    glRotatef(15, 0, 0, 1); // Reverse tilt
    glScalef(0.23, 0.5, 0.3); // Increase thickness
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw right arm
    glColor3f(1.0, 0.85, 0.75); // Skin color for arms
    glPushMatrix();
    glTranslatef(-0.25, 0.0, 0.0); // Adjust translation to be closer to body
    glRotatef(-30, 0.0, 0.0, 1.0); // Adjust rotation as needed
    glScalef(0.1, 0.9, 0.1); // Increased arm length
    glutSolidCube(1.0);
    glPopMatrix();


    // Adjusted left arm with increased length
    glPushMatrix();
    glTranslatef(0.25, 0.0, 0.0); // Adjust translation to be closer to body
    glRotatef(30, 0.0, 0.0, 1.0); // Adjust rotation as needed
    glScalef(0.1, 0.9, 0.1); // Increased arm length
    glutSolidCube(1.0);
    glPopMatrix();


    // Draw right leg
    glColor3f(0.0, 0.0, 0.0); // Black color for the legs
    glPushMatrix();
    glTranslatef(-0.21, -0.75, 0.0);
    glScalef(0.12, 0.5, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw left leg
    glPushMatrix();
    glTranslatef(0.21, -0.75, 0.0);
    glScalef(0.12, 0.5, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Drawing the right shoe
    glPushMatrix();
    glTranslatef(0.0, -0.1, 0.0); // Position for the right shoe
    drawShoe();
    glPopMatrix();

    // Drawing the left shoe
    glPushMatrix();
    glTranslatef(0.0, -0.1, 0.0); // Position for the left shoe
    glScalef(-1.0, 1.0, 1.0); // Mirror the shoe across the y-axis
    drawShoe();
    glPopMatrix();
    
    // Drawing the right hand
    //glPushMatrix();
    //
    //glTranslatef(-0.10, 0.6, 0.0); // Position for the right hand
    //drawHand();
    //glRotatef(-270, 0.0, 0.0, 1.0);
    //
    //glPopMatrix();

    // Drawing the left hand
    //glPushMatrix();
    //glTranslatef(0.3, 1.0, 0.0); // Position for the left hand
    //glScalef(-1.0, 1.0, 1.0); // Mirror the hand across the y-axis
    //drawHand();
    //glPopMatrix();

    
}

// Handler for window-repaint event
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
    glMatrixMode(GL_MODELVIEW);    // To operate on model-view matrix
    glLoadIdentity();              // Reset the model-view matrix

    drawHuman();

    glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);          // Initialize GLUT
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Enable depth, double buffering, and RGBA
    glutCreateWindow("3D Human Figure using GLUT"); // Create window
    glutInitWindowSize(800, 640);   // Set window size
    glutInitWindowPosition(50, 50); // Set window position
    initGL();                       // OpenGL initialization
    glutDisplayFunc(display);       // Register display callback
    glutMainLoop();                 // Enter the event-processing loop
    return 0;
}
