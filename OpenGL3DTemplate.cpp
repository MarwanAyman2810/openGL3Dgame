#include <cmath>
#include <iostream>
#include <glut.h>
#include<math.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
// Global variables for human character position and orientation


void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

class Vector3f {
public:
    float x, y, z;

    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f operator+(const Vector3f& v) const {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    Vector3f operator-(const Vector3f& v) const {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }

    Vector3f operator*(float n) const {
        return Vector3f(x * n, y * n, z * n);
    }

    Vector3f operator/(float n) const {
        return Vector3f(x / n, y / n, z / n);
    }

    Vector3f unit() const {
        float length = sqrt(x * x + y * y + z * z);
        if (length != 0.0f)
            return *this / length;
        else
            return *this;
    }

    Vector3f cross(const Vector3f& v) const {
        return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

class Camera {
public:
    Vector3f eye, center, up;

    Camera(const Vector3f& eyePos = Vector3f(0.0f, .9f, 1.5f),
        const Vector3f& centerPos = Vector3f(0.0f, .9f, 0.0f),
        const Vector3f& upVec = Vector3f(0.0f, 1.0f, 0.0f)) {
        eye = eyePos;
        center = centerPos;
        up = upVec;
    }

    void moveX(float d) {
        Vector3f right = up.cross(center - eye).unit();
        eye = eye + right * d;
        center = center + right * d;
    }

    void moveY(float d) {
        eye = eye + up.unit() * d;
        center = center + up.unit() * d;
    }

    void moveZ(float d) {
        Vector3f view = (center - eye).unit();
        eye = eye + view * d;
        center = center + view * d;
    }

    void rotateX(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
        up = view.cross(right);
        center = eye + view;
    }

    void rotateY(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
        right = view.cross(up);
        center = eye + view;
    }

    void look() const {
        gluLookAt(
            eye.x, eye.y, eye.z,
            center.x, center.y, center.z,
            up.x, up.y, up.z
        );
    }
};

Camera camera;

 void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(110, 1500.0 / 1000.0, 0.001, 190);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.look();
}
 
float parkSizex = 3.0;// Increase the size of the park
float parkSizez = 1.5;

float seesawAngle = 0.0f;  // Initial angle of the seesaw
float seesawSpeed = 0.5f;  // Speed of seesaw movement
float oscillation = 0.0f;
void drawCylinder(float radius, float height, int slices, int stacks) {
    GLUquadric* quadric = gluNewQuadric();

    // Draw the cylinder
    gluCylinder(quadric, radius, radius, height, slices, stacks);

    // Draw the top disk
    glPushMatrix();
    glTranslatef(0.0, 0.0, height);
    gluDisk(quadric, 0, radius, slices, 1);
    glPopMatrix();

    // Draw the bottom disk
    gluDisk(quadric, 0, radius, slices, 1);

    gluDeleteQuadric(quadric);
}


void drawGrass() {
    glColor3f(0.1, 0.9, 0.1); // Green color for grass
    glBegin(GL_LINES);
    for (double i = -parkSizex/2; i <= parkSizex/2; i += parkSizex / 150) {
        for (double j = -parkSizez/2; j <= parkSizez/2 ; j += parkSizez / 150) {
            glVertex3f(i, 0.0, j);
            glVertex3f(i, 0.03, j); // Slightly above the ground
        }
    }
    glEnd();
}

// Function to draw trees
void drawTree(float x, float y, float z) {
    // Drawing the trunk
    glColor3f(0.55, 0.27, 0.07); // Brown color for the trunk
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(-90, 1, 0, 0); // Rotate trunk to stand upright
    glutSolidCone(0.1, 0.5, 10, 2);
    glPopMatrix();

    // Drawing the foliage
    glColor3f(0.13, 0.55, 0.13); // Dark green for leaves
    glPushMatrix();
    glTranslated(x, y + 0.4, z);
    glutSolidSphere(0.25, 10, 10);
    glPopMatrix();
}

// Function to draw an umbrella
void drawUmbrella(float x, float y, float z) {
    // Umbrella pole
    glColor3f(0, 0, 0); // Black color for the pole
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(-90, 1, 0, 0);
    drawCylinder(0.02, 0.3, 10, 2);
    glPopMatrix();

    // Umbrella canopy
    glColor3f(1, 0, 0); // Red color for the canopy
    glPushMatrix();
    glTranslated(x, y + 0.3, z);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(0.3, 0.1, 10, 2);
    glPopMatrix();
}

// Function to draw moving bees
void drawBee(float x, float y, float z) {
    glColor3f(1, 1, 0); // Yellow color for bees
    glPushMatrix();
    glTranslated(x, y, z);
    glutSolidSphere(0.05, 5, 5);
    glPopMatrix();
}
void drawSupport(float x, float y, float z, float scale) {
    // Drawing triangular wooden support
    glColor3f(0.6, 0.3, 0); // Brown color for wood
    glBegin(GL_TRIANGLES);

    // Adjust these coordinates to properly connect the support to the pivot
    glVertex3f(x, y + 0.05 * scale, z); // Top vertex (attached to the pivot)
    glVertex3f(x - 0.1 * scale, y - 0.1 * scale, z); // Bottom left
    glVertex3f(x + 0.1 * scale, y - 0.1 * scale, z); // Bottom right

    glEnd();
}


// Function to draw the seesaw
void drawSeesaw(float x, float y, float z) {
    // Scale down the seesaw
    const float scale = 0.3; // Adjust this value to scale the seesaw

    // Seesaw base (pivot)
    glColor3f(0.3, 0.3, 0.3); // Dark grey color
    glPushMatrix();
    glTranslated(x, y, z);
    glScalef(scale, scale, scale); // Apply scale transformation
    drawCylinder(0.05, 0.05, 10, 10);
    glPopMatrix();

    // Draw wooden supports
    drawSupport(x, y - 0.1 * scale, z, scale);
    // Apply rotation for the seesaw plank and handles
    glPushMatrix();
    glTranslated(x, y + 0.05 * scale, z); // Adjust y position based on scale
    glRotated(seesawAngle, 0, 0, 1); // Rotate based on animation angle
    glScalef(scale, scale, scale); // Apply scale transformation

    // Seesaw plank
    glColor3f(1, 0.5, 0.5); // Light red color
    glPushMatrix();
    glScaled(1.0, 0.05, 0.1); // Scale for the plank
    glutSolidCube(1);
    glPopMatrix();

    // Handles at the ends of the seesaw
    glColor3f(0.5, 0.5, 0.5); // Grey color for handles
    for (int i = -1; i <= 1; i += 2) { // Two handles at each end
        glPushMatrix();
        glTranslated(i * 0.4, 0.05, 0);
        drawCylinder(0.02, 0.1, 10, 10);
        glPopMatrix();
    }

    glPopMatrix();
}



// Function to decorate the park
 void parkDecorator() {
    drawGrass();  // Add grass

    // Adjusted tree positions
    //drawTree(parkSizex * 0.5, 0, parkSizez * 0.5);
    drawTree(-parkSizex * 0.5, 0, -parkSizez * 0.5);

    // Adjusted umbrella position
    drawUmbrella(parkSizex * 0.5, 0, -parkSizez * 0.3);

    // Add moving bees
    static float beeX = -0.7, beeZ =-.9 ;
    drawBee(beeX, 0.2, beeZ);
    // Update bee positions for next frame (simple movement logic)
    beeX += (rand() % 3 - 1) * 0.1;
    beeZ += (rand() % 3 - 1) * 0.1;
    drawSeesaw(parkSizex * 0.3, 0.2, parkSizez * 0.4); // Position the seesaw within the park

}
 // Function to update the seesaw's animation
 void updateSeesawAnimation() {
     seesawAngle += seesawSpeed;
     if (seesawAngle > 30 || seesawAngle < -30) {
         seesawSpeed = -seesawSpeed;  // Reverse the direction
     }
 }


double wallLength = parkSizex;  // Length of the walls
//double ihatez = parkSizex / 2;
 void drawWall(double lengthx,double lengthz) {
    double wallThickness = 0.01;  // Hardcoded thickness
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(lengthx, wallThickness, lengthz); // Scale wall according to specified length
    glutSolidCube(1);
    glPopMatrix();
}
 void drawPark() {
    

    // Draw the ground
    glColor3f(0.0, 1, 0.0); // Gray color for the ground
    glPushMatrix();
    drawWall(parkSizex,parkSizez);
    glPopMatrix();
    glPushMatrix();
    parkDecorator();
    glPopMatrix();

   
}
float humanPosX = 0.0f, humanPosY = 0.06f, humanPosZ = 0.0f;
float humanRotY = 0.0f; // Rotation around the Y-axis
float facePosition = 0.0f; // Rotation around the Y-axis
static void drawHuman() {
    
    glPushMatrix();
    // First, translate to the human's current position
    glTranslatef(humanPosX, humanPosY, humanPosZ);

    // Then, rotate the human to face the direction of movement
    glRotatef(facePosition, 0.0f, 1.0f, 0.0f);

    // Apply scaling to the human model
    glScalef(0.09, 0.09, 0.09);
    // Draw head
    glColor3f(1.0, 0.85, 0.75); // Skin color for the head
    glTranslatef(0.0, 1.5, 0.0);
    glutSolidSphere(0.25, 20, 20);

    // Draw body
    glColor3f(0.0, 0.0, 1.0); // Blue color for the body
    glTranslatef(0.0, -0.6, 0.0);
    glScalef(1.0, 1.5, 0.5);
    glutSolidCube(0.5);
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


    glPopMatrix();
}




void Keyboard(unsigned char key, int x, int y) {
    float d = 0.08;
    float moveSpeed = 0.3f;
    float boundaryLimitx = parkSizex/2;
    float boundaryLimitz = parkSizez/2;
    switch (key) {
       
    case 'w':
        // Move forward
        if (humanPosZ - moveSpeed * cos(DEG2RAD(humanRotY)) > -boundaryLimitz) {
            humanPosX += moveSpeed * sin(DEG2RAD(humanRotY));
            humanPosZ -= moveSpeed * cos(DEG2RAD(humanRotY));
        }
        facePosition = 180;
        break;
    case 's':
        // Move backward
        if (humanPosZ + moveSpeed * cos(DEG2RAD(humanRotY)) < boundaryLimitz) {
            humanPosX -= moveSpeed * sin(DEG2RAD(humanRotY));
            humanPosZ += moveSpeed * cos(DEG2RAD(humanRotY));
        }
        facePosition = -180;
        break;
    case 'a':
        // Move left
        if (humanPosX - moveSpeed * cos(DEG2RAD(humanRotY)) > -boundaryLimitx) {
            humanPosX -= moveSpeed * cos(DEG2RAD(humanRotY));
            humanPosZ -= moveSpeed * sin(DEG2RAD(humanRotY));
        }
        facePosition = -90;
        break;
    case 'd':
        // Move right
        if (humanPosX + moveSpeed * cos(DEG2RAD(humanRotY)) < boundaryLimitx) {
            humanPosX += moveSpeed * cos(DEG2RAD(humanRotY));
            humanPosZ += moveSpeed * sin(DEG2RAD(humanRotY));
        }
        facePosition = 90;
        break;
        break;
    case 'i': 
        camera.moveY(d);
        break;
    case 'k': 
        camera.moveY(-d);
        break;
    case 'j': 
        camera.moveX(d);
        break;
    case 'l': 
        camera.moveX(-d);
        break;
    case 'q': 
        camera.moveZ(d);
        break;
    case 'e': 
        camera.moveZ(-d);
        break;

    case GLUT_KEY_ESCAPE:
        exit(EXIT_SUCCESS);
    }
    
   /* if (humanRotY >= 360.0f || humanRotY <= -360.0f) {
        humanRotY = fmod(humanRotY, 360.0f);
    }*/
    glutPostRedisplay();
}

void Special(int key, int x, int y) {
    float a = 3.0;

    switch (key) {
    case GLUT_KEY_UP:
        camera.rotateX(a);
        break;
    case GLUT_KEY_DOWN:
        camera.rotateX(-a);
        break;
    case GLUT_KEY_LEFT:
        camera.rotateY(a);
        break;
    case GLUT_KEY_RIGHT:
        camera.rotateY(-a);
        break;

    }

    glutPostRedisplay();
}




void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setupCamera();
    
    drawPark();
    updateSeesawAnimation();
    drawHuman();
    
    

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1500, 1000);
    glutCreateWindow("3D Player Example");



    glutDisplayFunc(display);
    init();
    
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);
    glutMainLoop();
    return 0;
}