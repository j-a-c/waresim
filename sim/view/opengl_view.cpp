// In order to get function prototypes from glext.h, 
// define GL_GLEXT_PROTOTYPES before including glext.h
#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <GL/glfw.h>
#include <cmath>


#include "glext.h"
#include "opengl_view.h"
#include "../util/util.h"

// Delete after debug.
#include <iostream>

        // Cube.
        //    v6----- v5
        //   /|      /|
        //  v1------v0|
        //  | |     | |
        //  | |v7---|-|v4
        //  |/      |/
        //  v2------v3 

        // Dimensions are defined in terms of UNITs.
        const float UNIT = 1.0f;

        // Vertex array for a cube.
        GLfloat cube_vertices[]  = { UNIT/2, UNIT/2, UNIT/2,  -UNIT/2, UNIT/2, UNIT/2,  -UNIT/2,-UNIT/2, UNIT/2,      // v0-v1-v2 (front)
                               -UNIT/2,-UNIT/2, UNIT/2,   UNIT/2,-UNIT/2, UNIT/2,   UNIT/2, UNIT/2, UNIT/2,      // v2-v3-v0

                                UNIT/2, UNIT/2, UNIT/2,   UNIT/2,-UNIT/2, UNIT/2,   UNIT/2,-UNIT/2,-UNIT/2,      // v0-v3-v4 (right)
                                UNIT/2,-UNIT/2,-UNIT/2,   UNIT/2, UNIT/2,-UNIT/2,   UNIT/2, UNIT/2, UNIT/2,      // v4-v5-v0

                                UNIT/2, UNIT/2, UNIT/2,   UNIT/2, UNIT/2,-UNIT/2,  -UNIT/2, UNIT/2,-UNIT/2,      // v0-v5-v6 (top)
                               -UNIT/2, UNIT/2,-UNIT/2,  -UNIT/2, UNIT/2, UNIT/2,   UNIT/2, UNIT/2, UNIT/2,      // v6-v1-v0

                               -UNIT/2, UNIT/2, UNIT/2,  -UNIT/2, UNIT/2,-UNIT/2,  -UNIT/2,-UNIT/2,-UNIT/2,      // v1-v6-v7 (left)
                               -UNIT/2,-UNIT/2,-UNIT/2,  -UNIT/2,-UNIT/2, UNIT/2,  -UNIT/2, UNIT/2, UNIT/2,      // v7-v2-v1

                               -UNIT/2,-UNIT/2,-UNIT/2,   UNIT/2,-UNIT/2,-UNIT/2,   UNIT/2,-UNIT/2, UNIT/2,      // v7-v4-v3 (bottom)
                                UNIT/2,-UNIT/2, UNIT/2,  -UNIT/2,-UNIT/2, UNIT/2,  -UNIT/2,-UNIT/2,-UNIT/2,      // v3-v2-v7

                                UNIT/2,-UNIT/2,-UNIT/2,  -UNIT/2,-UNIT/2,-UNIT/2,  -UNIT/2, UNIT/2,-UNIT/2,      // v4-v7-v6 (back)
                               -UNIT/2, UNIT/2,-UNIT/2,   UNIT/2, UNIT/2,-UNIT/2,   UNIT/2,-UNIT/2,-UNIT/2 };    // v6-v5-v4

        // Normal array for a cube.
        GLfloat cube_normals[]   = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
                                0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

                                0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
                                0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

                               -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
                               -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

                                0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
                                0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

                                0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
                                0, 0,-1,   0, 0,-1,   0, 0,-1 };    // v6-v5-v4

        // Color array for a worker's cube.
        GLfloat worker_colors[]    = { 1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v1-v2 (front)
                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v2-v3-v0

                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v5-v6 (top)
                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v6-v1-v0

                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v1-v6-v7 (left)
                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v7-v2-v1

                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v7-v4-v3 (bottom)
                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v3-v2-v7

                                1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v7-v6 (back)
                                1, 0, 0,   1, 0, 0,   1, 0, 0 };    // v6-v5-v4


/**
 * Constructor.
 */
OpenGLView::OpenGLView()
{
    init();
    enable();
}

/**
 * Destructor.
 */
OpenGLView::~OpenGLView()
{
    // We need to delete the VBO.
    glDeleteBuffersARB(1, &workerVBO);
    glDeleteBuffersARB(1, &factoryVBO);

}

/**
 * Initialize GLFW parameters.
 */
void OpenGLView::init()
{
    if (glfwInit() != GL_TRUE)
        shut_down();

    // Desktop parameters
    GLFWvidmode desktop;
    glfwGetDesktopMode(&desktop);

    // Create a full-screen window.
    //if (glfwOpenWindow(desktop.Width, desktop.Height, 8, 8, 8, 8, 24, 8, GLFW_FULLSCREEN) != GL_TRUE)

    // Create a window half of the screen size.
    desktop.Width /= 2;
    desktop.Height /= 2;
    if (glfwOpenWindow(desktop.Width, desktop.Height, 8, 8, 8, 8, 24, 8, GLFW_WINDOW) != GL_TRUE)
        shut_down();

    // Initialize GLEW
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0"))
    {
        // TODO
    }

    glViewport(0, 0, (GLsizei)desktop.Width, (GLsizei)desktop.Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective (angle of sight, perspective, depth)
    gluPerspective(60, (GLfloat)desktop.Width/ (GLfloat)desktop.Height, 0.1, 100.0); 
    glMatrixMode(GL_MODELVIEW);

    // TODO Initialize position

    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
}

/**
 * Enable OpenGL options.
 */
void OpenGLView::enable()
{
    // Space vertex winding is counter-clockwise.
    glFrontFace(GL_CCW);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glEnable(GL_DEPTH_TEST);

    // Keep current colors.
	glEnable(GL_COLOR_MATERIAL);
    
    // White diffuse light.
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

    // Enable a single OpenGL light.
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT0);

    // Automatically normalize normals.
    //glEnable(GL_NORMALIZE);

    // Back face culling
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glCullFace(GL_FRONT);

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    
	// Allows z-buffering. Can chew up extra CPU time and graphics memory for 
    // very complicated scenes.
    //glDepthMask(GL_TRUE);

}

/**
 * Shut down the GLFW window.
 */
void OpenGLView::shut_down()
{
    glfwTerminate();
    glfwCloseWindow();
}

/**
 * Rendering function
 */
void OpenGLView::render()
{
    
    // Clean screen to black.
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Re-load the orientation.
    glLoadIdentity();

    // Vertical rotation.
    glRotatef(v_angle, 1.0, 0.0, 0.0);
    // Horizontal rotation
    glRotatef(h_angle, 0.0, 1.0, 0.0);

    // Translate away from the camera.
    glTranslated(-x_pos, -y_pos, -z_pos);

    // TODO This is where we would render the simulation.

    // Bind VBOs with IDs and set the buffer offsets of the bound VBOs
    // When buffer object is bound with its ID, all pointers in gl*Pointer()
    // are treated as offset instead of real pointer.
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, workerVBO);

    // Enable vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Before draw, specify vertex and index arrays with their offsets
    glNormalPointer(GL_FLOAT, 0, (void*)sizeof(cube_vertices));
    glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(cube_vertices)+sizeof(cube_normals)));
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // Render the workers.
    for (auto& pos : factory->get_worker_locs())
    {
        // Translate the worker's position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, factory->get_width());
 
        // Translate the 'y' coordinate to OpenGL's z coordinate.
        z -= factory->get_height();
        
        // Move to the worker's location and draw the worker.
        glPushMatrix();
        glTranslated(-x,0,-z);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glPopMatrix();
    }

    // Disable vertex arrays.
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // Release VBOs after use.
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    // Swap back and front buffers
    glfwSwapBuffers();
}

/**
 * Look around.
 */
void OpenGLView::look_around()
{
    /*
    // Capture mouse position.
    int mouse_x, mouse_y;
    glfwGetMousePos(&mouse_x, &mouse_y);
    */

    // Rotate camera up if up key is pressed.
    if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
    {
        v_angle -= (ROTATE_SPEED);
    }
    // Rotate camera down if down key is pressed.
    if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        v_angle += (ROTATE_SPEED);
    }
    
    // Rotate camera left if left key is pressed.
    if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        h_angle -= (ROTATE_SPEED);
    }
    // Rotate camera right if left right is pressed.
    if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        h_angle += (ROTATE_SPEED);
    }

    // Re-scale the vertical angle to prevent overflows.
    while(v_angle > 360.0f) v_angle -= 360.0f;
    while(v_angle < 0.0f) v_angle += 360.0f;

    // Re-scale the horizontal angle to prevent overflows.
    while(h_angle > 360.0f) h_angle -= 360.0f;
    while(h_angle < 0.0f) h_angle += 360.0f;

}

/**
 * Move the camera.
 */
void OpenGLView::move(double delta)
{
    float xvel = 0.0f;
    float yvel = 0.0f;
    float zvel = 0.0f;

    // Compute vertical rotation in radians
    float yrotrad = v_angle * DEG2RAD;
    // Compute horizontal rotation in radians
    float xrotrad = h_angle * DEG2RAD; 
    // Projection on x and z axes
    float xproj = max_speed * float(std::cos(xrotrad)); 
    float zproj = max_speed * float(std::sin(xrotrad));
    float yproj = -max_speed * float(std::sin(yrotrad));

    // Move left if 'a' is pressed.
    if (glfwGetKey('a') == GLFW_PRESS || glfwGetKey('A') == GLFW_PRESS)
    {
        xvel -= xproj;
        zvel -= zproj;
    }

    // Move forward if 'w' is pressed.
    if (glfwGetKey('w') == GLFW_PRESS || glfwGetKey('W') == GLFW_PRESS)
    {
        yvel += yproj;
        xvel += zproj;
        zvel -= xproj;
    }

    // Move back if 's' is pressed.
    if (glfwGetKey('s') == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS)
    {
        yvel -= yproj;
        xvel -= zproj;
        zvel += xproj;
    }

    // Move right is 'd' is pressed.
    if (glfwGetKey('d') == GLFW_PRESS || glfwGetKey('D') == GLFW_PRESS)
    {
        xvel += xproj;
        zvel += zproj;
    }

    // Update the position.
    x_pos += (xvel * delta);
    y_pos += (yvel * delta);
    z_pos += (zvel * delta);

}

/**
 * Update the state of the world.
 */
void OpenGLView::update(double dt)
{
    look_around();
    move(dt);
}


/**
 * Initialize all the VBOs beforehand.
 */
void OpenGLView::initVBOs()
{
    // Create VBOs. These need to be deleted when the program exits.
    // We put vertex, normals, and colors in the same object.
    // Calling glBufferDataARB with NULL pointer reserves only memory space.

    // Create the factory VBO.
    glGenBuffersARB(1, &factoryVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, factoryVBO);

    // TODO Find the minimal number of faces necessary.


    // Create the worker VBO.
    glGenBuffersARB(1, &workerVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, workerVBO);
    // Target, size, data, usage.
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cube_vertices)+sizeof(cube_normals)+sizeof(worker_colors), nullptr, GL_STATIC_DRAW_ARB);
    // Copy vertices starting from 0 offest.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(cube_vertices), cube_vertices); 
    // Copy normals after vertices.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cube_vertices), sizeof(cube_normals), cube_normals); 
    // Copy colors after normals.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cube_vertices)+sizeof(cube_normals), sizeof(worker_colors), worker_colors);

}


/**
 * Encapsulates the method that occurs when this class is run in a Thread.
 */
void OpenGLView::run()
{
    // Initialize the VBOs.
    initVBOs();

    const double dt = 0.01;

    double prevTime = glfwGetTime();
    double accumulator = 0.0;

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        // Calculate time since last frame
        double currentTime = glfwGetTime();
        double delta = currentTime - prevTime;

        if (delta > 0.25)
            delta = 0.25;

        accumulator += delta;

        prevTime = currentTime;
        
        while ( accumulator >= dt )
        {
            update(dt);
            accumulator -= dt;
        }

        // Exit
        if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
        {
            shut_down();
            break;
        }


        render(); 
    }
}

/**
 * Set the factory representation.
 * We also build the static factory layout before returning.
 */
void OpenGLView::set_factory(Factory *factory)
{
    this->factory = factory;
}

/**
 * Set the simulation parameters.
 */
void OpenGLView::set_sim_params(time_t start_time, int sim_length)
{
    this->start_time = start_time;
    this->sim_length = sim_length;
}
