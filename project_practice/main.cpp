#include <stdio.h>
#include <math.h>
#include <glut.h>


float camX = 0.0f, camY = 3.0f, camZ = 15.0f;
float camYaw = 0.0f;
float camPitch = 0.0f;
float camSpeed = 0.5f;
float rotSpeed = 2.0f;
float sceneRotY = 0.0f;

float M_PI = 3.14159f;

void setCamera() {
    glRotatef(-camPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-camYaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camX, -camY, -camZ);
}

void keyboard(unsigned char key, int, int) {
    float lx = cos(camPitch * M_PI / 180.0f) * cos(camYaw * M_PI / 180.0f);
    float ly = sin(camPitch * M_PI / 180.0f);
    float lz = cos(camPitch * M_PI / 180.0f) * sin(camYaw * M_PI / 180.0f);

    switch (key) {
    case 27: exit(0); break;

    case 'a': camX += lx * camSpeed; camY += ly * camSpeed; camZ += lz * camSpeed; break;
    case 'd': camX -= lx * camSpeed; camY -= ly * camSpeed; camZ -= lz * camSpeed; break;
    case 'w': camX += lz * camSpeed; camZ -= lx * camSpeed; break;
    case 's': camX -= lz * camSpeed; camZ += lx * camSpeed; break;
    case 'q': camY += camSpeed; break;
    case 'e': camY -= camSpeed; break;
    case 'z': sceneRotY -= rotSpeed; break;
    case 'x': sceneRotY += rotSpeed; break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    switch (key) {
    case GLUT_KEY_LEFT:  camYaw -= rotSpeed; break;
    case GLUT_KEY_RIGHT: camYaw += rotSpeed; break;
    case GLUT_KEY_UP:    camPitch += rotSpeed; if (camPitch > 89) camPitch = 89; break;
    case GLUT_KEY_DOWN:  camPitch -= rotSpeed; if (camPitch < -89) camPitch = -89; break;
    }
    glutPostRedisplay();
}

void drawCone(float x, float y, float z, float height, float baseRadius, int slices = 32) {
    float angleStep = 2 * M_PI / slices;

    glPushMatrix();
    glTranslatef(x, y, z);

    // Set cone color (similar to building but slightly different)
    glColor3f(0.35f, 0.35f, 0.35f);

    // Draw cone sides
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < slices; i++) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;

        float x1 = cos(angle1) * baseRadius;
        float z1 = sin(angle1) * baseRadius;
        float x2 = cos(angle2) * baseRadius;
        float z2 = sin(angle2) * baseRadius;

        // Triangle from base edge to tip
        glVertex3f(x1, 0, z1);
        glVertex3f(x2, 0, z2);
        glVertex3f(0, height, 0);
    }
    glEnd();

    // Draw cone base
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= slices; i++) {
        float angle = i * angleStep;
        float px = cos(angle) * baseRadius;
        float pz = sin(angle) * baseRadius;
        glVertex3f(px, 0, pz);
    }
    glEnd();

    glPopMatrix();
}

void drawCylinder(float x, float y, float z, float height, float radius, int slices = 32) {
    float angleStep = 2 * M_PI / slices;
    float floorHeight = 2.5f; // Standard floor height
    int numFloors = (int)(height / floorHeight);

    glPushMatrix();
    glTranslatef(x, y, z);

    // Draw each floor separately
    for (int floor = 0; floor < numFloors; floor++) {
        float floorY = floor * floorHeight;
        float currentFloorHeight = floorHeight;

        // Adjust last floor height if needed
        if (floor == numFloors - 1) {
            currentFloorHeight = height - floorY;
        }

        // Alternate floor colors for depth
        if (floor % 2 == 0) {
            glColor3f(0.45f, 0.45f, 0.45f); // Darker gray
        }
        else {
            glColor3f(0.55f, 0.55f, 0.55f); // Lighter gray
        }

        // Draw main building wall with floor pattern
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            float angle = i * angleStep;
            float px = cos(angle) * radius;
            float pz = sin(angle) * radius;
            glVertex3f(px, floorY, pz);
            glVertex3f(px, floorY + currentFloorHeight, pz);
        }
        glEnd();

        // Draw windows on each floor
        glColor3f(0.2f, 0.3f, 0.5f); // Dark blue for windows
        int windowsPerFloor = slices / 4; // Number of windows per floor
        float windowWidth = angleStep * 0.6f;
        float windowHeight = currentFloorHeight * 0.4f;
        float windowY = floorY + currentFloorHeight * 0.3f;

        for (int w = 0; w < windowsPerFloor; w++) {
            float windowAngle = (w * 4) * angleStep; // Space windows around building
            float windowX = cos(windowAngle) * (radius + 0.01f); // Slightly outside wall
            float windowZ = sin(windowAngle) * (radius + 0.01f);

            // Draw small window rectangle
            glBegin(GL_QUADS);
            float wx1 = cos(windowAngle - windowWidth / 2) * (radius + 0.01f);
            float wz1 = sin(windowAngle - windowWidth / 2) * (radius + 0.01f);
            float wx2 = cos(windowAngle + windowWidth / 2) * (radius + 0.01f);
            float wz2 = sin(windowAngle + windowWidth / 2) * (radius + 0.01f);

            glVertex3f(wx1, windowY, wz1);
            glVertex3f(wx2, windowY, wz2);
            glVertex3f(wx2, windowY + windowHeight, wz2);
            glVertex3f(wx1, windowY + windowHeight, wz1);
            glEnd();
        }

        // Draw floor separator lines
        glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for floor lines
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < slices; i++) {
            float angle = i * angleStep;
            float px = cos(angle) * (radius + 0.005f);
            float pz = sin(angle) * (radius + 0.005f);
            glVertex3f(px, floorY + currentFloorHeight, pz);
        }
        glEnd();
    }

    // Draw the roof (darker shade)
    glColor3f(0.25f, 0.25f, 0.25f); // Very dark gray for roof
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, height, 0);
    for (int i = 0; i <= slices; i++) {
        float angle = i * angleStep;
        float px = cos(angle) * radius;
        float pz = sin(angle) * radius;
        glVertex3f(px, height, pz);
    }
    glEnd();

    // Draw the ground base
    glColor3f(0.6f, 0.6f, 0.6f); // Light gray for base
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= slices; i++) {
        float angle = i * angleStep;
        float px = cos(angle) * radius;
        float pz = sin(angle) * radius;
        glVertex3f(px, 0, pz);
    }
    glEnd();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    setCamera();

    // Apply scene rotation around Y-axis
    glRotatef(sceneRotY, 0.0f, 1.0f, 0.0f);

    int count = 9;  // More segments for taller building

    // Burj Khalifa-inspired asymmetric design with much greater height
    float radiusValues[] = { 4.0f, 3.5f, 3.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.7f, 0.5f }; // Reordered for tapering effect
    float heightValues[] = { 10.0f, 12.0f, 15.0f, 18.0f, 20.0f, 22.0f, 25.0f, 28.0f, 30.0f }; // Heights for each segment

    float currentY = 0.0f;  // Track position for stacking along Y-axis

    for (int i = 0; i < count; i++) {
        float radius = radiusValues[i];
        float height = heightValues[i];

        glPushMatrix();
        glTranslatef(0, currentY, 0); // Position along the Y-axis

        drawCylinder(0, 0, 0, height, radius);

        glPopMatrix();

        currentY += height; // Accumulate the height for the next cylinder
    }

    // Add cone on top of the final cylinder
    float coneHeight = 35.0f;
    float coneRadius = 0.2f;

    glPushMatrix();
    glTranslatef(0, currentY, 0); // Place cone on top of the total height
    drawCone(0, 0, 0, coneHeight, coneRadius);
    glPopMatrix();

    glutSwapBuffers();
}





void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 4.0 / 3.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0f * w / h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ratio, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("9 Touching Cylinders on X-axis");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}