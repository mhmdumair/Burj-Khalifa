#include <stdio.h>
#include <math.h>
#include <glut.h>

// Camera and scene variables
float camX = 0.0f, camY = 1000.0f, camZ = 1500.0f;
float camYaw = 0.0f;
float camPitch = 0.0f;
float camSpeed = 10.0f;
float rotSpeed = 2.0f;
float sceneRotY = 0.0f;

// Define PI
float M_PI = 3.14159f;

// Sets up the camera view
void setCamera() {
    glRotatef(-camPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-camYaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camX, -camY, -camZ);
}

// Handles keyboard input for camera movement and scene rotation
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

// Handles special key input for camera rotation
void specialKeys(int key, int, int) {
    switch (key) {
    case GLUT_KEY_LEFT:  camYaw -= rotSpeed; break;
    case GLUT_KEY_RIGHT: camYaw += rotSpeed; break;
    case GLUT_KEY_UP:    camPitch += rotSpeed; if (camPitch > 89) camPitch = 89; break;
    case GLUT_KEY_DOWN:  camPitch -= rotSpeed; if (camPitch < -89) camPitch = -89; break;
    }
    glutPostRedisplay();
}

// Draws a cone primitive for the spire
void drawCone(float x, float y, float z, float height, float baseRadius, int slices = 32) {
    float angleStep = 2 * M_PI / slices;

    glPushMatrix();
    glTranslatef(x, y, z);

    // Set cone color
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

// Draws a rectangular prism with a floor effect, removing the window effect
void drawRectangularPrism(float x, float y, float z, float length, float width, float height, int numFloors) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Define vertices for a simple rectangular base centered at (0,0,0)
    float halfLength = length / 2.0f;
    float halfWidth = width / 2.0f;

    float floorThickness = 2.0f; // Thickness of the floor lines
    float floorSpacing = height / numFloors;

    for (int floor = 0; floor < numFloors; floor++) {
        float floorY = floor * floorSpacing;
        float currentFloorHeight = floorSpacing;

        // Draw main wall section
        if (floor % 2 == 0) {
            glColor3f(0.45f, 0.45f, 0.45f); // Darker gray
        }
        else {
            glColor3f(0.55f, 0.55f, 0.55f); // Lighter gray
        }

        glBegin(GL_QUADS);
        // Front face
        glVertex3f(-halfLength, floorY, halfWidth);
        glVertex3f(halfLength, floorY, halfWidth);
        glVertex3f(halfLength, floorY + currentFloorHeight, halfWidth);
        glVertex3f(-halfLength, floorY + currentFloorHeight, halfWidth);
        // Back face
        glVertex3f(halfLength, floorY, -halfWidth);
        glVertex3f(-halfLength, floorY, -halfWidth);
        glVertex3f(-halfLength, floorY + currentFloorHeight, -halfWidth);
        glVertex3f(halfLength, floorY + currentFloorHeight, -halfWidth);
        // Right face
        glVertex3f(halfLength, floorY, halfWidth);
        glVertex3f(halfLength, floorY, -halfWidth);
        glVertex3f(halfLength, floorY + currentFloorHeight, -halfWidth);
        glVertex3f(halfLength, floorY + currentFloorHeight, halfWidth);
        // Left face
        glVertex3f(-halfLength, floorY, -halfWidth);
        glVertex3f(-halfLength, floorY, halfWidth);
        glVertex3f(-halfLength, floorY + currentFloorHeight, halfWidth);
        glVertex3f(-halfLength, floorY + currentFloorHeight, -halfWidth);
        glEnd();
    }

    // Draw floor lines
    glColor3f(0.3f, 0.3f, 0.3f); // Dark lines for floors
    for (int floor = 0; floor <= numFloors; floor++) {
        float floorY = floor * floorSpacing;
        glBegin(GL_QUADS);
        // Top and bottom faces of the floor line
        glVertex3f(-halfLength, floorY - floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY - floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY - floorThickness / 2, -halfWidth);
        glVertex3f(-halfLength, floorY - floorThickness / 2, -halfWidth);

        glVertex3f(-halfLength, floorY + floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY + floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY + floorThickness / 2, -halfWidth);
        glVertex3f(-halfLength, floorY + floorThickness / 2, -halfWidth);

        // Sides of the floor line
        glVertex3f(-halfLength, floorY - floorThickness / 2, halfWidth);
        glVertex3f(-halfLength, floorY + floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY + floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY - floorThickness / 2, halfWidth);

        glVertex3f(-halfLength, floorY - floorThickness / 2, -halfWidth);
        glVertex3f(-halfLength, floorY + floorThickness / 2, -halfWidth);
        glVertex3f(halfLength, floorY + floorThickness / 2, -halfWidth);
        glVertex3f(halfLength, floorY - floorThickness / 2, -halfWidth);

        glVertex3f(halfLength, floorY - floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY + floorThickness / 2, halfWidth);
        glVertex3f(halfLength, floorY + floorThickness / 2, -halfWidth);
        glVertex3f(halfLength, floorY - floorThickness / 2, -halfWidth);

        glVertex3f(-halfLength, floorY - floorThickness / 2, -halfWidth);
        glVertex3f(-halfLength, floorY + floorThickness / 2, -halfWidth);
        glVertex3f(-halfLength, floorY + floorThickness / 2, halfWidth);
        glVertex3f(-halfLength, floorY - floorThickness / 2, halfWidth);

        glEnd();
    }

    // Draw top face
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(-halfLength, height, halfWidth);
    glVertex3f(halfLength, height, halfWidth);
    glVertex3f(halfLength, height, -halfWidth);
    glVertex3f(-halfLength, height, -halfWidth);
    glEnd();

    // Draw bottom face
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    glVertex3f(-halfLength, 0, halfWidth);
    glVertex3f(halfLength, 0, halfWidth);
    glVertex3f(halfLength, 0, -halfWidth);
    glVertex3f(-halfLength, 0, -halfWidth);
    glEnd();

    glPopMatrix();
}

// Draws a large, round platform for the city model
void drawPlatform(float radius, float height) {
    glPushMatrix();
    glTranslatef(0.0f, -height, 0.0f); // Position the top of the platform at y=0

    // Draw top face
    glColor3f(0.7f, 0.7f, 0.75f); // Metallic silver
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, height, 0);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex3f(cos(angle) * radius, height, sin(angle) * radius);
    }
    glEnd();

    // Draw side face
    glColor3f(0.5f, 0.5f, 0.55f); // Darker gray for the side
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex3f(cos(angle) * radius, height, sin(angle) * radius);
        glVertex3f(cos(angle) * radius, 0, sin(angle) * radius);
    }
    glEnd();

    // Draw bottom face
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex3f(cos(angle) * radius, 0, sin(angle) * radius);
    }
    glEnd();

    glPopMatrix();
}

// Draws the moon as a sphere
void drawMoon() {
    glPushMatrix();
    glTranslatef(5000.0f, 5000.0f, -10000.0f);
    glColor3f(0.9f, 0.9f, 0.95f);
    GLUquadric* quadric = gluNewQuadric();
    gluSphere(quadric, 400.0f, 32, 32);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

// Main display function to render the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    setCamera();

    // Moonlight setup
    GLfloat light_position[] = { 10000.0, 10000.0, -10000.0, 0.0 };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.25f, 1.0f };
    GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.6f, 1.0f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.6f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glRotatef(sceneRotY, 0.0f, 1.0f, 0.0f);

    drawMoon();
    drawPlatform(1000.0f, 20.0f);

    // Draw central Burj Khalifa-like structure
    int numLevels = 15;
    float wingLengths[] = { 250, 235, 220, 205, 190, 175, 160, 145, 130, 115, 100, 85, 70, 55, 40 };
    float wingWidths[] = { 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10 };
    float levelHeights[] = { 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340 };
    int floorsPerWing[] = { 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 15, 15, 15 };

    float overallRotation = 0.0f;
    float rotationIncrement = 3.0f;
    float currentY = 0.0f;

    for (int i = 0; i < numLevels; i++) {
        float length = wingLengths[i];
        float width = wingWidths[i];
        float height = levelHeights[i];
        int numFloors = floorsPerWing[i];

        glPushMatrix();
        glTranslatef(0, currentY, 0);
        glRotatef(overallRotation, 0.0f, 1.0f, 0.0f);
        for (int j = 0; j < 3; j++) {
            glPushMatrix();
            glRotatef(j * 120.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(length * 0.3f, 0, 0);
            drawRectangularPrism(0, 0, 0, length, width, height, numFloors);
            glPopMatrix();
        }
        glPopMatrix();
        currentY += height;
        overallRotation += rotationIncrement;
    }

    float spireBaseRadius = wingWidths[numLevels - 1] * 0.5f;
    float spireHeight = 800.0f;
    glPushMatrix();
    glTranslatef(0, currentY, 0);
    drawCone(0, 0, 0, spireHeight, spireBaseRadius, 32);
    glPopMatrix();

    // Draw smaller buildings on the platform with varying floor counts and dimensions
    struct Building { float x, z, length, width, height; int floors; };
    Building buildings[] = {
        {600, 300, 100, 50, 400, 12},
        {-400, 500, 80, 80, 600, 15},
        {500, -500, 120, 60, 800, 10},
        {-700, -200, 90, 90, 500, 14},
        {100, -800, 70, 70, 300, 11},
        {-200, 200, 60, 100, 450, 13},
        {800, 100, 50, 110, 350, 10},
        {-500, -600, 75, 55, 550, 13},
        {200, 700, 60, 80, 480, 14},
        {-800, 300, 110, 40, 700, 15},
        {750, -400, 90, 120, 420, 11},
        {-150, -650, 110, 60, 580, 14},
        {350, 600, 150, 80, 500, 12},
        {-600, 150, 85, 140, 750, 15},
        {400, -250, 130, 95, 650, 14},
        {-250, -700, 115, 100, 470, 11},
        {700, 500, 90, 130, 380, 10}
    };
    int numBuildings = sizeof(buildings) / sizeof(buildings[0]);

    for (int i = 0; i < numBuildings; i++) {
        glPushMatrix();
        glTranslatef(buildings[i].x, 0, buildings[i].z);
        drawRectangularPrism(0, 0, 0, buildings[i].length, buildings[i].width, buildings[i].height, buildings[i].floors);
        glPopMatrix();
    }

    glutSwapBuffers();
}

// Initializes OpenGL settings
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 4.0 / 3.0, 1.0, 20000.0);
    glMatrixMode(GL_MODELVIEW);

    camY = 1000.0f;
    camZ = 1500.0f;
}

// Handles window reshaping
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0f * w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ratio, 1.0, 20000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D City Model");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
