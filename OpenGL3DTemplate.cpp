#include <cmath>
#include <iostream>
#include <glut.h>
#include<math.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <thread> 
#include <sstream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
// Global variables for human character position and orientation
bool isFerrisWheelRotating = false;
float ferrisWheelAngle = 1.0;
bool isSeesawMoving = false;
float torusRotationAngle = 0.0f;
bool isTorusRotating = false;
float iceCreamScale = 1.5f; // Initial scale of the ice cream
bool isTreeScaling = false;
float treeScale = 1.0f;
float scaleDirection = 0.01f; // Adjust for scaling speed
bool isumbrellaScaling = false;
float umbrellaScale = 1.0f;
bool isIceCreamVisible = true;
float humanPosX = 0.0f, humanPosY = 0.06f, humanPosZ = 0.0f;
float humanRotY = 0.0f; // Rotation around the Y-axis
float facePosition = 0.0f; // Rotation around the Y-axis



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
    gluPerspective(120, 1500.0 / 1000.0, 0.001, 190);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.look();
}

float parkSizex = 3.0;// Increase the size of the park
float parkSizez = 1.5;

float seesawAngle = 0.0f;  // Initial angle of the seesaw
float seesawSpeed = 1.0;  // Speed of seesaw movement
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
// Function to draw a single column


const float FERRIS_WHEEL_RADIUS = .8; // Adjust as needed
const int NUM_CABINS = 8; // Number of cabins on the wheel
const float SMALL_DISK_RADIUS = 0.1;
// Constants for the columns
const float COLUMN_HEIGHT = 1; // Adjust as needed
//const float COLUMN_RADIUS = 0.03; // Radius of the columns
const float COLUMN_ANGLE = 35.0; // Angle of tilt for the columns

//void drawColumn() {
//    glColor3f(0.5, 0.5, 0.5); // Grey color for the column
//    glPushMatrix();
//    glRotatef(COLUMN_ANGLE, 1, 0, 1); // Tilt the column
//    GLUquadric* quadric = gluNewQuadric();
//    gluCylinder(quadric, COLUMN_RADIUS, COLUMN_RADIUS, COLUMN_HEIGHT, 10, 10);
//    gluDeleteQuadric(quadric);
//    glPopMatrix();
//}
void drawColumn(float angle) {
    glPushMatrix();
    glColor3f(0.7, 0.7, 0.7); // Column color
    glRotatef(angle, 0, 0, 1); // Rotate column
    glTranslatef(0.0, COLUMN_HEIGHT / 2, -0.7); // Position column
    glRotatef(2 * -angle, 0, 0, 1); // Rotate column
    glScalef(0.05, COLUMN_HEIGHT, 0.05); // Scale to column size
    glutSolidCube(1.0);
    glPopMatrix();
}
float frame_HEIGHT = 0.5;
//void drawFrame() {
//     glPushMatrix();
//    glScalef(1, frame_HEIGHT, 1); // Apply scaling to the frame height
//    drawColumn(0); // First column
//    glTranslatef(0, COLUMN_HEIGHT / frame_HEIGHT, 1.4); // Adjust position for the second column
//    drawColumn(180); // Second column, facing the first
//    glPopMatrix();

//    // Draw the second pair of columns
//    //glPushMatrix();
//    //glTranslatef(2, 0, 0); // Adjust position for the third column
//    //drawColumn(0); // Third column
//    //glTranslatef(0, 0, 1.4); // Adjust position for the fourth column
//    //drawColumn(180); // Fourth column, facing the third
//    //glPopMatrix();

//    //// Draw the connecting bar
//    //glPushMatrix();
//    //glColor3f(0.7, 0.7, 0.7); // Bar color
//    //glTranslatef(1, COLUMN_HEIGHT, 0.7); // Position at the top center
//    //glScalef(2, 0.05, 0.05); // Scale to the right size
//    //glutSolidCube(1.0);
//    //glPopMatrix();
//}
// Function to draw a Ferris wheel cabin
void drawCabin() {
    glColor3f(0.8, 0.1, 0.1); // Cabin color
    glPushMatrix();
    glScalef(0.2, 0.15, 0.15); // Scale to cabin size
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawFerrisWheel(float x, float y, float z) {
    glColor3f(0.5, 0, 1); // Color for the wheel
    glPushMatrix();
    glTranslatef(x, y, z);

    // Draw the wheel
    GLUquadric* quadric = gluNewQuadric();
    glPushMatrix();
    glRotatef(-90, 0, 0, 1);
    glRotatef(ferrisWheelAngle, 0, 0, 1);
    gluDisk(quadric, FERRIS_WHEEL_RADIUS - 0.1, FERRIS_WHEEL_RADIUS, 20, 20);
    glPopMatrix();
    //gluDeleteQuadric(quadric);
    // Draw the smaller disk at the center
    glColor3f(0.5, 0.5, 0.5); // Grey color for the smaller disk
    gluDisk(quadric, 0, SMALL_DISK_RADIUS, 20, 20);

    gluDeleteQuadric(quadric);
    // Draw cabin
    for (int i = 0; i < NUM_CABINS; ++i) {
        float angle = (360.0 / NUM_CABINS) * i;
        glPushMatrix();
        glRotatef(angle, 0, 0, 1);
        glRotatef(ferrisWheelAngle, 0, 0, 1);
        glTranslatef(0, FERRIS_WHEEL_RADIUS, 0);
        drawCabin();
        glPopMatrix();
    }


    // Draw radial lines
    int numLines = 30; // Adjust this for more or fewer lines
    glColor3f(0, 0, 0); // Line color
    for (int i = 0; i < numLines; ++i) {
        float angle = (360.0 / numLines) * i;
        float radianAngle = angle * M_PI / 180.0;

        // Calculate end point of line on the wheel circumference
        float endX = FERRIS_WHEEL_RADIUS * cos(radianAngle);
        float endY = FERRIS_WHEEL_RADIUS * sin(radianAngle);

        // Draw line from center to the wheel edge
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0); // Center of wheel
        glVertex3f(endX, endY, 0); // Edge of wheel
        glEnd();
    }

    glPopMatrix();


    glPopMatrix();
    drawColumn(COLUMN_ANGLE);
    drawColumn(-COLUMN_ANGLE);
}



void drawGrass() {
    glColor3f(0.1, 0.9, 0.1); // Green color for grass
    glBegin(GL_LINES);
    for (double i = -parkSizex / 2; i <= parkSizex / 2; i += parkSizex / 150) {
        for (double j = -parkSizez / 2; j <= parkSizez / 2; j += parkSizez / 150) {
            glVertex3f(i, 0.0, j);
            glVertex3f(i, 0.03, j); // Slightly above the ground
        }
    }
    glEnd();
}

// Function to draw trees
void drawTree(float x, float y, float z, float ScaleFactor) {
    // Drawing the trunk
    glColor3f(0.55, 0.27, 0.07); // Brown color for the trunk
    glPushMatrix();
    glScalef(ScaleFactor, ScaleFactor, ScaleFactor); // Apply scaling
    glTranslated(x, y, z);
    glRotated(-90, 1, 0, 0); // Rotate trunk to stand upright
    glutSolidCone(0.1, 0.5, 10, 2);
    glPopMatrix();

    // Drawing the lower foliage
    glColor3f(0.13, 0.55, 0.13); // Dark green for leaves
    glPushMatrix();
    glScalef(ScaleFactor, ScaleFactor, ScaleFactor); // Apply scaling
    glTranslated(x, y + 0.4, z);
    glutSolidSphere(0.25, 10, 10);
    glPopMatrix();

    // Drawing the upper foliage
    glColor3f(0.13, 0.55, 0.13); // Same color for leaves
    glPushMatrix();
    glScalef(ScaleFactor, ScaleFactor, ScaleFactor); // Apply scaling
    glTranslated(x, y + 0.65, z); // Adjust the y-coordinate for the position of the upper foliage
    glutSolidSphere(0.2, 10, 10); // Slightly smaller sphere for the top
    glPopMatrix();
}



// Function to draw an umbrella
void drawUmbrella(float x, float y, float z, float ScaleFactor) {
    // Umbrella pole
    glColor3f(0, 0, 0); // Black color for the pole
    glPushMatrix();
    glScalef(ScaleFactor, ScaleFactor, ScaleFactor); // Apply scaling
    glTranslated(x, y, z);
    glRotated(-90, 1, 0, 0);
    drawCylinder(0.02, 0.3, 10, 2);
    glPopMatrix();

    // Umbrella canopy
    glColor3f(1, 0, 0); // Red color for the canopy
    glPushMatrix();
    glScalef(ScaleFactor, ScaleFactor, ScaleFactor);
    glTranslated(x, y + 0.3, z);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(0.3, 0.1, 10, 2);
    glPopMatrix();

    // Umbrella handle (at the top of the pole)
    glColor3f(0.2, 0.2, 0.2); // Dark grey color for the handle
    glPushMatrix();
    glScalef(ScaleFactor, ScaleFactor, ScaleFactor);
    glTranslated(x, y + 0.3, z);
    glutSolidSphere(0.03, 10, 10); // Small sphere for handle
    glPopMatrix();

    // Tip of the canopy (smaller cone at the top)
    glColor3f(1, 0.2, 0.2); // Slightly different red color for variety
    glPushMatrix();
    glScalef(ScaleFactor, ScaleFactor, ScaleFactor);
    glTranslated(x, y + 0.4, z); // Position slightly above the canopy
    glRotated(-90, 1, 0, 0);
    glutSolidCone(0.1, 0.05, 10, 2); // Smaller cone for the tip
    glPopMatrix();
}


//// Function to draw moving bees
//void drawBee(float x, float y, float z) {
//    glColor3f(1, 1, 0); // Yellow color for bees
//    glPushMatrix();
//    glTranslated(x, y, z);
//    glutSolidSphere(0.05, 5, 5);
//    glPopMatrix();
//}
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

void drawCabin(float size) {
    glColor3f(1.0, 0.0, 0.0); // Red color for the cabin
    glPushMatrix();
    glutSolidCube(size);
    glPopMatrix();
}


void drawTorus(float innerRadius, float outerRadius, int sides, int rings) {
    glPushMatrix();
    glColor3f(0.5, 0.5, 1.0); // Color for the torus
    glutSolidTorus(innerRadius, outerRadius, sides, rings);
    glPopMatrix();
}
void drawRadialLines(float cylinderRadius, float cylinderHeight, float torusInnerRadius, float torusOuterRadius, int numLines, float lineLengthFactor) {
    glColor3f(1.0, 0.0, 0.0); // Color for radial lines
    float angleStep = 360.0 / numLines;

    for (int i = 0; i < numLines; i++) {
        float angle = DEG2RAD(i * angleStep);
        float x = cos(angle);
        float z = sin(angle);

        // Start point on the cylinder
        float startX = x * cylinderRadius;
        float startZ = z * cylinderRadius;
        float startY = 1.6;

        // End point closer to the cylinder
        float endX = startX + x * lineLengthFactor;
        float endZ = startZ + z * lineLengthFactor;
        float endY = startY - lineLengthFactor; // Adjust this to control the height of the end point

        glBegin(GL_LINES);
        glVertex3f(startX, startY, startZ);
        glVertex3f(endX, endY, endZ);
        glEnd();
    }
}

void drawCabinsOnTorus(float torusInnerRadius, float torusOuterRadius, int numCabins) {
    float angleStep = 360.0 / numCabins;
    float cabinSize = 0.1; // Adjust as needed

    for (int i = 0; i < numCabins; i++) {
        float angle = DEG2RAD(i * angleStep);
        float x = cos(angle) * (torusOuterRadius + torusInnerRadius) / 1.2f;
        float z = sin(angle) * (torusOuterRadius + torusInnerRadius) / 1.2f;
        float y = torusOuterRadius - torusInnerRadius; // Adjust Y-coordinate as needed

        glPushMatrix();
        glTranslatef(x, 1.3, z);
        glRotatef(angle * 180.0 / M_PI, 0, 1, 0); // Orient the cabin
        glRotatef(torusRotationAngle, 0, 0, 1); // Rotate around Y-axis
        drawCabin(cabinSize);
        glPopMatrix();
    }
}



// Function to decorate the park
void parkDecorator() {
    drawGrass();  // Add grass

    // Adjusted tree positions
    //drawTree(parkSizex * 0.5, 0, parkSizez * 0.5);
    if (isTreeScaling) {
        // Scale the tree continuously between 1.0 (original size) and 1.5 (150% size)
        // Adjust the 5000.0 divisor to change the speed of scaling
        float scaleFactor = 1.0 + 0.1 * sin(glutGet(GLUT_ELAPSED_TIME) / 200.0);
        drawTree(-0.4 * parkSizex, 0, -parkSizez * 0.34, scaleFactor);
    }
    else {
        float  scaleFactor = 1.0; // No scaling when not active
        drawTree(-0.4 * parkSizex, 0, -parkSizez * 0.34, scaleFactor);
    }


    if (isumbrellaScaling) {
        // Scale the tree continuously between 1.0 (original size) and 1.5 (150% size)
        // Adjust the 5000.0 divisor to change the speed of scaling
        float scaleFactor = 1.0 + 0.1 * sin(glutGet(GLUT_ELAPSED_TIME) / 200.0);
        drawUmbrella(parkSizex * 0.35, 0, -parkSizez * 0.35, scaleFactor);
    }
    else {
        float  scaleFactor = 1.0; // No scaling when not active
        drawUmbrella(parkSizex * 0.35, 0, -parkSizez * 0.35, scaleFactor);
    }
    // Adjusted umbrella position


    // Add moving bees
    static float beeX = -0.7, beeZ = -.9;
    /* drawBee(beeX, 0.2, beeZ);*/
     // Update bee positions for next frame (simple movement logic)
    /* beeX += (rand() % 3 - 1) * 0.1;
     beeZ += (rand() % 3 - 1) * 0.1;*/
    if (isSeesawMoving) {
        // Update seesawAngle for animation
        seesawAngle += seesawSpeed;
        if (seesawAngle > 30 || seesawAngle < -30) { // Limit angle to a range
            seesawSpeed = -seesawSpeed; // Reverse direction
        }
    }

    drawSeesaw(parkSizex * 0.3, 0.07, parkSizez * 0.4); // Redraw seesaw with updated angle
    //drawFerrisWheel(0, 1, -1); // Adjust the position 

}
// Function to update the seesaw's animation



void drawSphereCloud() {
    float sphereSize = 0.1; // Size for the smaller spheres

    // Top sphere
    glColor3f(1, 1, 1); // Red color
    glPushMatrix();
    glTranslated(0, sphereSize - 0.15, 0);
    glutSolidSphere(sphereSize, 100, 100);
    glPopMatrix();

    // Bottom sphere

    glPushMatrix();
    glTranslated(0, -sphereSize - 0.05, 0);
    glutSolidSphere(sphereSize, 100, 100);
    glPopMatrix();

    // Left sphere 

    glPushMatrix();
    glTranslated(-sphereSize - 0.03, sphereSize - 0.21, 0);
    glutSolidSphere(sphereSize, 100, 100);
    glPopMatrix();

    // Right sphere 

    glPushMatrix();
    glTranslated(sphereSize + 0.03, sphereSize - 0.21, 0);
    glutSolidSphere(sphereSize, 100, 100);
    glPopMatrix();
}


double wallLength = parkSizex;  // Length of the walls
double wallThickness = 0.01;  // Hardcoded thickness
//double ihatez = parkSizex / 2;
void drawWall(double lengthx, double lengthz) {

    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(lengthx, wallThickness, lengthz); // Scale wall according to specified length
    glutSolidCube(1);
    glPopMatrix();
}
// Function to draw a triangular prism
void drawTriangularPrism(float height, float base, float length) {
    float halfBase = base / 2.0f;

    glBegin(GL_TRIANGLES); // Begin drawing the triangular faces

    // Front face
    glVertex3f(0, 0, 0);
    glVertex3f(halfBase, height, 0);
    glVertex3f(-halfBase, height, 0);

    // Back face
    glVertex3f(0, 0, length);
    glVertex3f(halfBase, height, length);
    glVertex3f(-halfBase, height, length);
    glEnd();

    glBegin(GL_QUADS); // Begin drawing the rectangular faces
    // Bottom face
    glVertex3f(-halfBase, height, 0);
    glVertex3f(halfBase, height, 0);
    glVertex3f(halfBase, height, length);
    glVertex3f(-halfBase, height, length);

    // Left face
    glVertex3f(0, 0, 0);
    glVertex3f(-halfBase, height, 0);
    glVertex3f(-halfBase, height, length);
    glVertex3f(0, 0, length);

    // Right face
    glVertex3f(0, 0, 0);
    glVertex3f(halfBase, height, 0);
    glVertex3f(halfBase, height, length);
    glVertex3f(0, 0, length);
    glEnd();
}
float longy = 4;
void drawPark() {
    // Draw the ground
    glColor3f(0.0, 1, 0.0); // Green color for the ground
    glPushMatrix();
    drawWall(parkSizex, parkSizez);
    glPopMatrix();

    glPushMatrix();
    parkDecorator();
    glPopMatrix();

    // Calculate elapsed time
    float elapsedTime = glutGet(GLUT_ELAPSED_TIME) / 200; // Time in seconds

    // Interpolate between the colors every 2 seconds
    float interpolationFactor = (sin(elapsedTime / 2.0f * M_PI) + 1) / 0.5f; // Oscillates between 0 and 1 every 2 seconds

    // Interpolate green and blue components between 1 and 0.9
    float greenComponent = 0.9f + 0.1f * interpolationFactor;
    float blueComponent = 0.9f + 0.1f * interpolationFactor;

    // Set wall color with the interpolated components
    glColor3f(0.9f, greenComponent, blueComponent); // Interpolated color between the two shades of blue
    //backwall
    //glColor3f(.7, 1, 1); // Brown color for the back wall
    glPushMatrix();
    glTranslated(0, 2, -0.5 * parkSizez); // Reposition the back wall
    glScaled(parkSizex, longy, 0);
    glRotated(180, 0, 1, 0); // Rotate 180 degrees to face inward
    glRotated(90, 1, 0, 0); // Rotate 90 degrees vertically
    glutSolidCube(1);
    glPopMatrix();
    //// Draw the left wall
    //glColor3f(0.7, 1, 1); // Brown color for the left wall
    glPushMatrix();
    glTranslated(-0.5 * parkSizex, 0.5, 0); // Reposition the left wall
    glScaled(0.01, 1, parkSizez); // Scale wall according to specified length
    glRotated(-90, 0, 1, 0); // Rotate -90 degrees to face left
    glRotated(90, 1, 0, 0); // Rotate 90 degrees vertically
    glutSolidCube(1);
    glPopMatrix();

    //// Draw the right wall
    //glColor3f(0.7, 1, 1); // blue color for the left wall
    glPushMatrix();
    glTranslated(0.5 * parkSizex, 0.5, 0); // Reposition the left wall
    glScaled(0.01, 1, parkSizez); // Scale wall according to specified length
    glRotated(-90, 0, 1, 0); // Rotate -90 degrees to face left
    glRotated(90, 1, 0, 0); // Rotate 90 degrees vertically
    glutSolidCube(1);
    glPopMatrix();

    // Cloud of spheres on the back wall with different colors
    glPushMatrix();
    glTranslated(-1.2, longy - 0.1, -0.5 * parkSizez + 0.01); //cloud#1
    drawSphereCloud();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.7, longy - 0.5, -0.5 * parkSizez + 0.01); //cloud#2
    drawSphereCloud();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.8, longy - 0.5, -0.5 * parkSizez + 0.01); //cloud2and3
    drawSphereCloud();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.5, longy - 0.3, -0.5 * parkSizez + 0.01); //cloud#4
    drawSphereCloud();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1.2, longy - 0.5, -0.5 * parkSizez + 0.01); //cloud#5
    drawSphereCloud();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.25, longy - 0.3, -0.5 * parkSizez + 0.01); //cloud#6
    drawSphereCloud();
    glPopMatrix();


    glColor3f(1, 1, 0); // Yellow color
    glPushMatrix();
    glTranslated(0, 3.8, -0.55 * parkSizez); //sun
    glutSolidSphere(.2, 100, 100);
    glPopMatrix();



    // Fence properties
    float fenceHeight = 0.15; // Height of the fence cylinders
    float fenceRadius = 0.01; // Radius of the fence cylinders
    int slices = 16; // Number of slices for the cylinder
    int stacks = 16; // Number of stacks for the cylinder
    float fenceSpacing = 0.1; // Space between each cylinder
    int numFencePosts = static_cast<int>(parkSizez / fenceSpacing); // Number of fence posts

    // Triangular prism properties
    float prismBase = 0.03; // Base of the triangular prism
    float prismHeight = 0.03; // Height of the triangular prism
    float prismLength = 0.02; // Length of the triangular prism

    // Drawing fences on both sides
    for (int i = 0; i < numFencePosts; ++i) {
        float zPos = i * fenceSpacing - (parkSizez / 2) + fenceSpacing / 2;

        // Draw fence on the right side
        glColor3f(0.7, 0.4, 0.0); // Brown color for the fences
        glPushMatrix();
        glTranslated(parkSizex / 2.1, 0, zPos); // Position each cylinder along the right wall
        glRotated(-90, 1, 0, 0); // Rotate the cylinder to stand upright on the X-Z plane
        drawCylinder(fenceRadius, fenceHeight, slices, stacks);
        glPopMatrix();

        // Draw the triangular prism on top of the right fence
        glColor3f(0.7, 0.4, 0.0); // Brown color for the fences
        glPushMatrix();
        glTranslated(parkSizex / 2.1, fenceHeight + (fenceHeight / 6), zPos + (prismLength / 2)); // Position the prism on top of the cylinder
        glRotatef(180, 1, 0, 0);
        drawTriangularPrism(prismHeight, prismBase, prismLength);
        glPopMatrix();

        // Draw fence on the left side
        glColor3f(0.7, 0.4, 0.0); // Brown color for the fences
        glPushMatrix();
        glTranslated(-parkSizex / 2.1, 0, zPos); // Position each cylinder along the left wall
        glRotated(-90, 1, 0, 0); // Rotate the cylinder to stand upright on the X-Z plane
        drawCylinder(fenceRadius, fenceHeight, slices, stacks);
        glPopMatrix();

        // Draw the triangular prism on top of the left fence
        glColor3f(0.7, 0.4, 0.0); // Brown color for the fences
        glPushMatrix();
        glTranslated(-parkSizex / 2.1, fenceHeight + (fenceHeight / 6), zPos + (prismLength / 2));
        glRotatef(180, 1, 0, 0);
        drawTriangularPrism(prismHeight, prismBase, prismLength);
        glPopMatrix();
    }
    // Draw connecting ropes between fences

    glColor3f(0., 0., 0.); // Black color for the ropes
    int numRopes = 8; // Number of ropes between each pair of fence posts
    float ropeSpacing = 0.02; // Vertical spacing between ropes

    for (int i = 0; i < numFencePosts - 1; ++i) {
        float zPos1 = i * fenceSpacing - (parkSizez / 2) + fenceSpacing / 2;
        float zPos2 = (i + 1) * fenceSpacing - (parkSizez / 2) + fenceSpacing / 2;

        for (int j = 0; j < numRopes; ++j) {
            float currentRopeHeight = fenceHeight - (j * ropeSpacing);

            // Draw rope on the right side
            glBegin(GL_LINES);
            glVertex3f(parkSizex / 2.1, currentRopeHeight, zPos1);
            glVertex3f(parkSizex / 2.1, currentRopeHeight, zPos2);
            glEnd();

            // Draw rope on the left side
            glBegin(GL_LINES);
            glVertex3f(-parkSizex / 2.1, currentRopeHeight, zPos1);
            glVertex3f(-parkSizex / 2.1, currentRopeHeight, zPos2);
            glEnd();
        }
    }
}

 void drawHuman() {

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
 bool iceCreamVisible = true; // Initially, the ice cream is visible
 float iceCreamPosX = 0.7f, iceCreamPosY = 0.2f, iceCreamPosZ = 0.01f;
void drawCone(float base, float height, int slices, int stacks) {
    GLUquadric* quadric = gluNewQuadric();
    glPushMatrix();
    glRotatef(90, 1, 0, 0); // Rotate the cone to stand upright
    gluCylinder(quadric, base, 0.0f, height, slices, stacks); // Draw a cone
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void drawSphere(float radius, int slices, int stacks) {
    glutSolidSphere(radius, slices, stacks); // Draw a sphere
}
bool checkIceCreamCollision() {
    float distance = sqrt(pow(humanPosX - iceCreamPosX, 2) +
        pow(humanPosY - iceCreamPosY, 2) +
        pow(humanPosZ - iceCreamPosZ, 2));
    return distance < 0.2f; // Collision threshold, adjust as necessary
}

void drawIceCream(float coneHeight, float scoopRadius) {
    if (!iceCreamVisible) return;
    float iceCreamScale = 1.0f + 0.02f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.008f); // Change 0.001f to adjust speed
    // Draw the cone
    glColor3f(0.82f, 0.70f, 0.55f); // Brown color for the cone
    glPushMatrix();
    glScalef(iceCreamScale, iceCreamScale, iceCreamScale); // Apply dynamic scaling
    glTranslatef(0.7, 0.2, 0.01f); // Position the cone at the origin
    drawCone(scoopRadius, coneHeight, 20, 20); // Adjust size as needed
    glPopMatrix();

    // Draw the first scoop
    glColor3f(0.6, 0.4, 0.1); // Light color for the scoop (e.g., vanilla)
    glPushMatrix();
    glScalef(iceCreamScale, iceCreamScale, iceCreamScale); // Apply dynamic scaling
    glTranslatef(0.7, 0.2, 0.01f); // Position on top of the cone
    drawSphere(scoopRadius, 20, 20);
    glPopMatrix();

    // Draw the second scoop
    glColor3f(0.9, 0.8f, 0.4f); // Red color for the scoop (e.g., strawberry)
    glPushMatrix();
    glScalef(iceCreamScale, iceCreamScale, iceCreamScale); // Apply dynamic scaling
    glTranslatef(0.7, 0.25, 0.01f); // Position above the first scoop
    drawSphere(scoopRadius, 20, 20);
    glPopMatrix();

    // Draw the third scoop
    glColor3f(1.0f, 0.5f, 0.5f); // Green color for the scoop (e.g., mint)
    glPushMatrix();
    glScalef(iceCreamScale, iceCreamScale, iceCreamScale); // Apply dynamic scaling
    glTranslatef(0.7, 0.3, 0.01f); // Position above the second scoop
    drawSphere(scoopRadius, 20, 20);
    glPopMatrix();
}
bool gameActive = true; // Global variable to control game state


void setFrontView() {
    camera.eye = Vector3f(0.0f, 0.5f, 1.5f); // Closer along Z-axis
    camera.center = Vector3f(0.0f, 0.5f, 0.0f);
    camera.up = Vector3f(0.0f, 1.0f, 0.0f);
}


void setSideView() {
    camera.eye = Vector3f(1.5f, 0.5f, 0.0f); // Closer along X-axis
    camera.center = Vector3f(0.0f, 0.5f, 0.0f);
    camera.up = Vector3f(0.0f, 1.0f, 0.0f);
}


void setTopView() {
    camera.eye = Vector3f(0.0f, 1.5f, 0.0f); // Closer along Y-axis
    camera.center = Vector3f(0.0f, 0.0f, 0.0f);
    camera.up = Vector3f(0.0f, 0.0f, -1.0f);
}



void Keyboard(unsigned char key, int x, int y) {
    float d = 0.08;
    float moveSpeed = 0.1f;
    float boundaryLimitx = parkSizex / 2;
    float boundaryLimitz = parkSizez / 1.95;
    switch (key) {
    case 'f':
        isFerrisWheelRotating = !isFerrisWheelRotating;
        isSeesawMoving = !isSeesawMoving;
        isTorusRotating = !isTorusRotating;
        isTreeScaling = !isTreeScaling; // Toggle tree scaling
        isumbrellaScaling = !isumbrellaScaling;
        break;
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

    case '1':
        setFrontView();
        break;
    case '2':
        setSideView();
        break;
    case '3':
        setTopView();
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




const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 1000;
float remainingTime = 20.0; 
//bool gameActive = true;

void drawText(const char* text, int length, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    double* matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 1500, 0, 1000, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}
void updateTimer(int value) {
    remainingTime -= 1.0; // Decrease time by 1000 milliseconds (1 second)
    if (remainingTime < 0) {
        remainingTime = 0;
        // Add code here to handle what happens when time runs out
    }
    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(1000, updateTimer, 0); // Recall this function after 1 second
}

void displayTimer() {
    glColor3f(0.0, 0.0, 0.0); // Set text color to black

    std::stringstream ss;
    ss << "Time Remaining: " << remainingTime << " seconds";
    std::string timeStr = ss.str();
    drawText(timeStr.data(), timeStr.size(), 10, WINDOW_HEIGHT - 20);
}

void timer(int value) {
    if (gameActive) {
        remainingTime -= 1.0;
        if (remainingTime <= 0) {
            gameActive = false;
            remainingTime = 0;
        }
        glutPostRedisplay();
        glutTimerFunc(1000, timer, 0); // Call timer every second
    }
}
void drawYouLose() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set background color to white
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up an orthogonal projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Set text color to black
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2i(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2);

    std::string winText = "nope, you lose!";
    for (char c : winText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Restore the previous projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glutSwapBuffers();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    exit(0);
}
void drawWinScreen() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set background color to white
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up an orthogonal projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Set text color to black
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2i(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2);

    std::string winText = "Congratulations, you win!";
    for (char c : winText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Restore the previous projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glutSwapBuffers();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    exit(0);
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (!gameActive) {
        if (iceCreamVisible) {
            drawYouLose(); ;
        }
        else
        {
            drawWinScreen();
        }
        
        return; // Skip the rest of the rendering
    }
    setupCamera();

    

    drawPark();
    
    drawIceCream(0.2, 0.05);
    if (isFerrisWheelRotating) {
        ferrisWheelAngle += 1; // Adjust the speed as needed
        //if (ferrisWheelAngle >= 360.0f) ferrisWheelAngle -= 360.0f;
    }
    drawFerrisWheel(0, 0.8, -0.66);

    drawHuman();
    if (checkIceCreamCollision()) {
        iceCreamVisible = false; // Hide ice cream if collision detected
    }
     drawIceCream(0.2, 0.05); // Draw ice cream only if visible


    if (isTorusRotating) {
        torusRotationAngle += 1.05; // Adjust rotation speed as needed
        if (torusRotationAngle >= 360.0f) {
            torusRotationAngle -= 360.0f;
        }
    }

    // Draw the cylinder
    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(-1, 1.2, 0.5);
    glRotatef(90, 1, 0, 0);
    drawCylinder(0.03f, 1.2, 20, 20);
    glPopMatrix();

    // Draw the torus
    glPushMatrix();
    glTranslatef(-1, 0.8f, 0.5f);
    glRotatef(90, 1, 0, 0);
    //glRotatef(torusRotationAngle, 0, 0, 1); // Rotate around Y-axis
    drawTorus(0.03f, 0.3f, 20, 20);
    glPopMatrix();

    // Draw radial lines
    glTranslatef(-1, -0.48, 0.5);
    float lineLengthFactor = 0.3f; // Adjust this value to control the length of the lines
    drawRadialLines(0.03f, 1.5f, 0.05f, 0.3f, 20, lineLengthFactor);

    drawCabinsOnTorus(0.05f, 0.3f, 6); // Adjust the number of cabins as needed

    //drawIceCream(0.2, 0.05);
    //drawFrame();
    displayTimer();
    
    glutPostRedisplay();


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
    glutTimerFunc(1000, timer, 0); // 30000 milliseconds = 30 seconds 
    glutMainLoop();
    return 0;
}