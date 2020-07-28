// The code in this file is based on code provided by Lisa Dion
// in a 3D graphics tutorial.


#include "graphics.h"
#include "gameManager.h"

GLdouble width, height;
int wd;
GameManager manager;
// Mouse variables
int prevMouseX, prevMouseY;

void init()
{
    width = 800;
    height = 500;
    prevMouseX = width/2;
    prevMouseY = height/2;
}

/* Initialize OpenGL Graphics */
void initGL()
{
    // Enable alpha transparency
    // Code from https://www.opengl.org/archives/resources/faq/technical/transparency.htm
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set "clearing" or background color
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Point camLoc = manager.getCameraLocation();
    Point camLook = manager.getCameraLookingAt();
    Point camUp = manager.getCameraUp();
    gluLookAt(camLoc.x, camLoc.y, camLoc.z,  // eye position
              camLook.x, camLook.y, camLook.z,  // center position (not gaze direction)
              camUp.x, camUp.y, camUp.z); // up vector
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display()
{
    glLineWidth(3.0);

    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width/height, 1, 4096);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glEnable(GL_DEPTH);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);

    // Update where the camera is
    Point camLoc = manager.getCameraLocation();
    Point camLook = manager.getCameraLookingAt();
    Point camUp = manager.getCameraUp();
    gluLookAt(camLoc.x, camLoc.y, camLoc.z,  // eye position
              camLook.x, camLook.y, camLook.z,  // center position (not gaze direction)
              camUp.x, camUp.y, camUp.z); // up vector

    // Draw in 3d
    manager.draw();


    // Switch to 2d mode
    // Code from https://www.youtube.com/watch?v=i1mp4zflkYo
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0,width,0.0,height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw UI things
    manager.drawUI();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y)
{
    // escape
    if (key == 27)
    {
        glutDestroyWindow(wd);
        exit(0);
    }

    switch(key)
    {
        case 'w': manager.setWKey(true);
            break;
        case 'a': manager.setAKey(true);
            break;
        case 's': manager.setSKey(true);
            break;
        case 'd': manager.setDKey(true);
            break;
        case 'r': manager.setRKey(true);
            break;
        case 'c': manager.setCKey(true);
            break;
        case 'v': manager.printPlayerBuildingDebug();
            break;
    }

    glutPostRedisplay();
}


void kbu(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w': manager.setWKey(false);
            break;
        case 'a': manager.setAKey(false);
            break;
        case 's': manager.setSKey(false);
            break;
        case 'd': manager.setDKey(false);
            break;
        case 'r': manager.setRKey(false);
            break;
        case 'c': manager.setCKey(false);
            break;
        case 'p' : manager.togglePaused();
            break;
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y)
{
    glutPostRedisplay();
}

void cursor(int x, int y)
{
    double theta = atan2(y - prevMouseY, x - prevMouseX);
    manager.reactToMouseMovement(x, y, theta);
    prevMouseX = x;
    prevMouseY = y;

    // If the cursor gets close to the edge during the game, put it back in the middle.
    if(manager.getCurrentStatus() == Playing && (x < 80 || x > width - 80 || y < 80 || y > height - 80))
    {
        glutWarpPointer(width/2,height/2);
    }
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_UP)
    {
        manager.reactToMouseClick(x, y);
        if(manager.getCurrentStatus() == Playing)
        {
            // Make the cursor invisible
            glutSetCursor(GLUT_CURSOR_NONE);
        }
        else // Keep arrow when not playing
        {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
            // Check for if the quit button was hit
            if(manager.getCloseWindow())
            {
                glutDestroyWindow(wd);
                exit(0);
            }
        }
    }
    glutPostRedisplay();
}

void timer(int dummy)
{
    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
    manager.tick();
    if(manager.getShowMouse()) // If the user hasn't clicked, the cursor won't show
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv)
{

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Prime City" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // Make the cursor invisible
    //glutSetCursor(GLUT_CURSOR_NONE);

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbu);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}