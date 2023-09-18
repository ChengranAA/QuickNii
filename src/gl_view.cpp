#include "quicknii.h"
#include <math.h>
#include <filesystem>

int INITIAL_X = 500;
int INITIAL_Y = 200;

int HEADER_WIN_W = 400;
int HEADER_WIN_H = 200;

GLuint textureID;
nifti_image *NII;
nifti_1_header NII_HEADER;

double rotationAngle = 0.0;
bool displaySliceIndex = false;
bool displayHeader = false;

bool headerWindowVisible = false;
int headerWindowID = -1;

void keyboard(unsigned char key, int x, int y);

void display_text_gl(std::string text, int x, int y)
{
    glRasterPos2i(x, y); // Position for text
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void renderHeader()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, HEADER_WIN_W, HEADER_WIN_H + glutGet(GLUT_WINDOW_HEIGHT), 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0, 0.8, 0);

    int yOffset = 20; // Starting y-coordinate for text
    int line_w = 20;
    int xOffset = 30;
    // Display each header field using display_text_gl
    std::__fs::filesystem::path filePath = FILE_PATH;
    display_text_gl("File Name: " + filePath.filename().string(), 10, yOffset);
    yOffset += line_w;

    display_text_gl("sizeof_hdr: " + std::to_string(NII_HEADER.sizeof_hdr), 10, yOffset);
    yOffset += line_w;

    display_text_gl("data_type: " + std::string(NII_HEADER.data_type), 10, yOffset);
    yOffset += line_w;

    // Add the rest of the fields in the same manner
    display_text_gl("extents: " + std::to_string(NII_HEADER.extents), 10, yOffset);
    yOffset += line_w;

    display_text_gl("session_error: " + std::to_string(NII_HEADER.session_error), 10, yOffset);
    yOffset += line_w;

    display_text_gl("regular: " + std::to_string(NII_HEADER.regular), 10, yOffset);
    yOffset += line_w;

    display_text_gl("dim_info: " + std::to_string(NII_HEADER.dim_info), 10, yOffset);
    yOffset += line_w;

    display_text_gl("dim: ", 10, yOffset);
    xOffset = 30;
    yOffset += line_w;
    for (int i = 0; i < 8; i++)
    {
        display_text_gl(std::to_string(NII_HEADER.dim[i]) + ",", xOffset, yOffset);
        if (i == 3)
        {
            yOffset += line_w,
            xOffset = 30;
        }
        else
        {
            xOffset += 50;
        }
    }
    yOffset += line_w;

    display_text_gl("intent_p1: " + std::to_string(NII_HEADER.intent_p1), 10, yOffset);
    yOffset += line_w;

    display_text_gl("intent_p2: " + std::to_string(NII_HEADER.intent_p2), 10, yOffset);
    yOffset += line_w;

    display_text_gl("intent_p3: " + std::to_string(NII_HEADER.intent_p3), 10, yOffset);
    yOffset += line_w;

    display_text_gl("intent_code: " + std::to_string(NII_HEADER.intent_code), 10, yOffset);
    yOffset += line_w;

    display_text_gl("datatype: " + std::to_string(NII_HEADER.datatype), 10, yOffset);
    yOffset += line_w;

    display_text_gl("bitpix: " + std::to_string(NII_HEADER.bitpix), 10, yOffset);
    yOffset += line_w;

    display_text_gl("slice_start: " + std::to_string(NII_HEADER.slice_start), 10, yOffset);
    yOffset += line_w;

    // Display pixdim
    display_text_gl("pixdim: ", 10, yOffset);
    yOffset += line_w;
    xOffset = 30;
    for (int i = 0; i < 8; i++)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << NII_HEADER.pixdim[i];
        std::string roundedPixdim = oss.str();
        display_text_gl(roundedPixdim + ", ", xOffset, yOffset);
        if (i == 3)
        {
            yOffset += line_w,
            xOffset = 30;
        }
        else
        {
            xOffset += 50;
        }
    }
    yOffset += line_w;

    display_text_gl("vox_offset: " + std::to_string(NII_HEADER.vox_offset), 10, yOffset);
    yOffset += line_w;

    display_text_gl("scl_slope: " + std::to_string(NII_HEADER.scl_slope), 10, yOffset);
    yOffset += line_w;

    display_text_gl("scl_inter: " + std::to_string(NII_HEADER.scl_inter), 10, yOffset);
    yOffset += line_w;

    // Continue adding the remaining fields in a similar manner
    // ...

    // Update yOffset for the last section of text
    yOffset += 20;

    // Display the srow_x, srow_y, and srow_z arrays
    display_text_gl("srow_x: ", 10, yOffset);
    for (int i = 0; i < 4; i++)
    {
        display_text_gl(std::to_string(NII_HEADER.srow_x[i]) + ", ", 60 + i * 70, yOffset);
    }
    yOffset += line_w;

    display_text_gl("srow_y: ", 10, yOffset);
    for (int i = 0; i < 4; i++)
    {
        display_text_gl(std::to_string(NII_HEADER.srow_y[i]) + ", ", 60 + i * 70, yOffset);
    }
    yOffset += line_w;

    display_text_gl("srow_z: ", 10, yOffset);
    for (int i = 0; i < 4; i++)
    {
        display_text_gl(std::to_string(NII_HEADER.srow_z[i]) + ", ", 60 + i * 70, yOffset);
    }
    yOffset += line_w;

    // Display the remaining fields as needed
    display_text_gl("intent_name: " + std::string(NII_HEADER.intent_name), 10, yOffset);
    yOffset += line_w;

    display_text_gl("magic: " + std::string(NII_HEADER.magic), 10, yOffset);
}

void displayH()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderHeader();
    glutSwapBuffers();
}

void toggleHeaderWindow()
{
    if (headerWindowVisible)
    {
        glutDestroyWindow(headerWindowID);
        headerWindowVisible = false;
    }
    else
    {
        // Get the position and size of the main window
        int mainWindowX = glutGet(GLUT_WINDOW_X);
        int mainWindowY = glutGet(GLUT_WINDOW_Y);
        int mainWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        // Calculate the position for the header window to the left of the main window
        int headerWindowX = mainWindowX - HEADER_WIN_W;
        int headerWindowY = mainWindowY;

        // Create a new window for the header
        glutInitWindowPosition(headerWindowX, headerWindowY);
        headerWindowID = glutCreateWindow("Header");
        glClearColor(0.3, 0.3, 0.3, 0);
        glutDisplayFunc(displayH);                     // Set the display function for the header window
        glutKeyboardFunc(keyboard);                    // Allow keyboard input for the header window
        glutReshapeWindow(HEADER_WIN_W, mainWindowHeight + HEADER_WIN_H); // Set the window size
        headerWindowVisible = true;

    }
}

void renderSliceIndex()
{
    // Save the current matrix mode
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Reset the transformation
    glLoadIdentity();

    // Display SLICE_INDEX as text on the top left corner
    glColor3f(0, 1.0, 0);
    glRasterPos2i(10, 20); // Position for text
    std::string indexText = std::to_string(SLICE_INDEX);
    for (char c : indexText)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void renderSlice()
{
    Eigen::MatrixXd slice = SLICE;
    double minValue = slice.minCoeff();
    double maxValue = slice.maxCoeff();
    slice = (slice.array() - minValue) / (maxValue - minValue);
    slice = slice.cwiseMin(1.0).cwiseMax(0.0);

    int mapX = slice.cols();
    int mapY = slice.rows();
    int mapS = 1; // Set your desired size for each cell.

    int new_win_width = mapX;
    int new_win_height = mapY;

    if (((int(rotationAngle) / 90) & 1) == 1)
    {
        new_win_width = mapY;
        new_win_height = mapX;
    }
    glutReshapeWindow(new_win_width, new_win_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, new_win_width, new_win_height, 0); // Match the orthographic projection to your image size

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calculate the center of the image
    double centerX = new_win_width / 2.0;
    double centerY = new_win_height / 2.0;

    // Apply the rotation
    glTranslatef(centerX, centerY, 0.0);
    glRotatef(-rotationAngle, 0.0, 0.0, 1.0);
    glTranslatef(-mapX / 2.0, -mapY / 2.0, 0.0);

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

    if (displayHeader)
        renderHeader();
    if (displaySliceIndex)
        renderSliceIndex();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'q' || key == 'Q' || key == 27 /* ESC key */)
    {
        glutDestroyWindow(glutGetWindow());
        nifti_image_free(NII);
        exit(0);
    }
    if (key == 'a')
    {
        SLICE_INDEX -= 1;
        loadAndSliceNifti();
    };
    if (key == 'A')
    {
        SLICE_INDEX -= 10;
        loadAndSliceNifti();
    }; // speed mode
    if (key == 'd')
    {
        SLICE_INDEX += 1;
        loadAndSliceNifti();
    };
    if (key == 'D')
    {
        SLICE_INDEX += 10;
        loadAndSliceNifti();
    }; // speed mode
    if (key == 'x' || key == 'X')
        displaySliceIndex = !displaySliceIndex;
    if (key == 'h' || key == 'H')
        toggleHeaderWindow();
    if (key == 'r')
    {
        // Rotate the displayed matrix 90 degrees counterclockwise
        rotationAngle += 90.0;
        if (rotationAngle == 360.0)
        {
            rotationAngle = 0;
        }
    }

    if (key == 'R')
    {
        // Rotate the displayed matrix 90 degrees clockwise
        rotationAngle -= 90.0;
        if (rotationAngle == -360.0)
        {
            rotationAngle = 0;
        }
    }
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderSlice();
    glutSwapBuffers();
}

void initializeGL(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(INITIAL_X, INITIAL_Y);
    glutCreateWindow("QuickNii");
    glClearColor(0.3, 0.3, 0.3, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}