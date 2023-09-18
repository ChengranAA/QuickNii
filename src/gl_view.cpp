#include "quicknii.h"

GLuint textureID;

void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q' || key == 27 /* ESC key */) {
        // Close the GLUT window and exit the program
        glutDestroyWindow(glutGetWindow());
        exit(0);
    }
    if (key == 'a') SLICE_INDEX -= 1;
    if (key == 'A') SLICE_INDEX -= 10; // speed mode
    if (key == 'd') SLICE_INDEX += 1;
    if (key == 'D') SLICE_INDEX += 10; // speed mode
    glutPostRedisplay();
}

void renderSlice()
{
    Eigen::MatrixXd slice = loadAndSliceNifti();
    double minValue = slice.minCoeff();
    double maxValue = slice.maxCoeff();
    slice = (slice.array() - minValue) / (maxValue - minValue);
    slice = slice.cwiseMin(1.0).cwiseMax(0.0);

    int mapX = slice.cols();
    int mapY = slice.rows();
    int mapS = 1; // Set your desired size for each cell.

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, mapX, mapY, 0); // Match the orthographic projection to your image size

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);
    for (int x = 0; x < mapX; x++)
    {
        for (int y = 0; y < mapY; y++)
        {
            glColor3f(slice(y, x), slice(y, x), slice(y, x));
            glVertex2i(x, y);
            glVertex2i(x, y + mapS);
            glVertex2i(x + mapS, y + mapS);
            glVertex2i(x + mapS, y);
        }
    }
    glEnd();
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderSlice();
    glutSwapBuffers();
}

void initializeGL(int argc, char **argv, win_config this_win_config)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(500, 200);
    glutInitWindowSize(this_win_config.windowWidth, this_win_config.windowHeight);
    glutCreateWindow("QuickNii");
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, this_win_config.windowWidth, this_win_config.windowHeight, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}