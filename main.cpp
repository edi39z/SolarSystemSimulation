
#include <vector>
#include <GL/glut.h>
#include <cmath>
#include <iostream>


struct OrbitTrail {
    std::vector<std::pair<float, float>> points;

    void addPoint(float x, float z) {
        points.push_back({ x, z });
        if (points.size() > 500) 
            points.erase(points.begin());
    }

    void drawTrail(float r, float g, float b) const {
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < points.size(); ++i) {
            glColor3f(r, g, b);
            glVertex3f(points[i].first, 0.0, points[i].second);
        }
        glEnd();
    }
};

struct Planet {
    float radius;
    float distance;
    float orbit;
    float orbitSpeed;
    float eccentricity;
    float targetEccentricity;
    float rotation;
    float rotationSpeed;
    float tilt; 
    float r, g, b;
    float morphState;
    bool isMorphing;
    mutable OrbitTrail trail;

    Planet(float rad, float dist, float o, float os, float rotSpeed, float tiltAngle, float red, float green, float blue)
        : radius(rad), distance(dist), orbit(o), orbitSpeed(os), eccentricity(0.0),
        targetEccentricity(0.0), rotation(0.0), rotationSpeed(rotSpeed), tilt(tiltAngle),
        r(red), g(green), b(blue), morphState(0.0), isMorphing(false) {}


    void draw() const {
        float x = distance * (1 + eccentricity) * cos(orbit * 3.14 / 180.0);
        float z = distance * (1 - eccentricity) * sin(orbit * 3.14 / 180.0);

        glPushMatrix();
        glTranslatef(x, 0.0, z);

       
        glRotatef(tilt, 0.0, 0.0, 0.0);
        glRotatef(rotation, 0.0, 1.0, 0.0);

       
        float lightIntensity = cos(orbit * 3.14 / 180.0);
        float planetColor = (lightIntensity > 0) ? 1.0f : 0.5f;
        glColor3f(r * planetColor, g * planetColor, b * planetColor);

      
        if (isMorphing) {
            float scaleY = 1.0 - morphState;
            float scaleXZ = 1.0 + morphState;
            glScalef(scaleXZ, scaleY, scaleXZ);
        }

        glutSolidSphere(radius, 50, 50);
        glPopMatrix();

        trail.addPoint(x, z);
    }

    void drawTrail() const {
        trail.drawTrail(r, g, b);
    }

    void update() {
        orbit += orbitSpeed;
        if (orbit > 360.0) orbit -= 360.0;

        rotation += rotationSpeed;
        if (rotation > 360.0) rotation -= 360.0;

        if (std::abs(targetEccentricity - eccentricity) > 0.001) {
            if (targetEccentricity > eccentricity)
                eccentricity += 0.005;
            else
                eccentricity -= 0.005;
        }

        if (isMorphing) {
            morphState += 0.01;
            if (morphState >= 1.0) {
                morphState = 1.0;
                isMorphing = false;
            }
        }
    }

    void startMorphing() {
        morphState = 0.0;
        isMorphing = true;
    }

    std::pair<float, float> getPosition() const {
        return { distance * (1 + eccentricity) * cos(orbit * 3.14 / 180.0),
                 distance * (1 - eccentricity) * sin(orbit * 3.14 / 180.0) };
    }
};

std::vector<Planet> planets = {

    Planet(0.2, 2.0, 0.0, 2.0, 0.5, 23.5, 0.5, 0.5, 0.5),  // Gray planet
    Planet(0.3, 3.0, 0.0, 1.5, 0.3, 3.1, 0.9, 0.6, 0.1),   // Orange planet
    Planet(0.4, 4.0, 0.0, 1.0, 0.2, 25.0, 0.2, 0.4, 1.0),  // Blue planet
    Planet(0.35, 5.0, 0.0, 0.8, 0.1, 0.0, 1.0, 0.2, 0.2)   // Red planet
};

Planet sun(1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);


float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraZoom = 20.0f;
float cameraPanX = 0.0f;
float cameraPanY = 0.0f;
int lastMouseX = -1;
int lastMouseY = -1;
bool isDragging = false;
bool isPaused = false;
void initRendering();
void handleResize(int w, int h);
void handleKeypress(unsigned char key, int x, int y);
void handleMouse(int button, int state, int x, int y);
void handleMouseMotion(int x, int y);
void update(int value);
void checkCollisions();
void drawScene();

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    
    float ambientLight[] = { 0.6, 0.6, 0.6, 1.0 };
    float diffuseLight[] = { 1.0, 1.0, 1.0, 1.0 };
    float lightPosition[] = { 0.0, 10.0, 10.0, 1.0 };  

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glClearColor(0.0, 0.0, 0.0, 1.0); 
}



void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'e': 
        planets[2].targetEccentricity = 0.3; 
        break;
    case 'c': 
        planets[2].targetEccentricity = 0.0; 
        break;
    case 'm': 
        for (auto& planet : planets) {
            planet.startMorphing();
        }
        break;
    case 'p':
        isPaused = true;
        break;
    case 'r':
        isPaused = false;
        break;
    case 27:
        exit(0);
    }
}

void handleMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        lastMouseX = x;
        lastMouseY = y;
        isDragging = true;
    }
    else if (state == GLUT_UP) {
        isDragging = false;
    }

    if (button == 3) {
        cameraZoom -= 1.0f;
        if (cameraZoom < 5.0f) cameraZoom = 5.0f;
    }
    else if (button == 4) {
        cameraZoom += 1.0f;
    }
}

void handleMouseMotion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        cameraAngleX += dx * 0.2f;
        cameraAngleY += dy * 0.2f;

        lastMouseX = x;
        lastMouseY = y;
    }
}


void update(int value) {
    if (!isPaused) {
        for (auto& planet : planets) {
            planet.update();
        }

        checkCollisions();
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void checkCollisions() {
    for (size_t i = 0; i < planets.size(); ++i) {
        for (size_t j = i + 1; j < planets.size(); ++j) {
            auto posA = planets[i].getPosition();
            auto posB = planets[j].getPosition();

            float dx = posA.first - posB.first;
            float dz = posA.second - posB.second;
            float distance = sqrt(dx * dx + dz * dz);

            if (distance < (planets[i].radius + planets[j].radius)) {
                std::cout << "Collision detected between planets " << i << " and " << j << "!" << std::endl;
                isPaused = true;
                return;
            }
        }
    }
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

        glTranslatef(cameraPanX, cameraPanY, -cameraZoom);
    glRotatef(cameraAngleY, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleX, 0.0f, 1.0f, 0.0f);
    float sunPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);  

    sun.draw();
    for (const auto& planet : planets) {
        planet.draw();
        planet.drawTrail();
    }

    glutSwapBuffers();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System Simulation");
    initRendering();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(handleMouse);
    glutMotionFunc(handleMouseMotion);
    glutTimerFunc(25, update, 0);
    glutMainLoop();
    return 0;
}