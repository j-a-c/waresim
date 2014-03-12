#include <GL/glew.h>
#include <GL/glfw.h>
#include <cmath>

#include "opengl_view.h"

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

    // Create window
    if (glfwOpenWindow(desktop.Width, desktop.Height, 8, 8, 8, 8, 24, 8, GLFW_FULLSCREEN) != GL_TRUE)
    //desktop.Width /= 2;
    //desktop.Height /= 2;
    //if (glfwOpenWindow(desktop.Width, desktop.Height, 8, 8, 8, 8, 24, 8, GLFW_WINDOW) != GL_TRUE)
        shut_down();

    // Initialize mouse
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0,0);
    
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

    glPushMatrix();

    glBegin(GL_TRIANGLES);

    // TODO This is where we would render the simulation.
    glColor3f(1.0f, 0.0f, 0.0f);
    b.createFront();
    b.createBack();
    b.createLeft();
    b.createRight();
    b.createTop();
    b.createBottom();
    glEnd();

    glPopMatrix();

    // Swap back and front buffers
    glfwSwapBuffers();
}

#include <iostream>

/**
 * Look around.
 */
void OpenGLView::look_around()
{
    // Capture mouse position.
    int mouse_x, mouse_y;
    glfwGetMousePos(&mouse_x, &mouse_y);

    std::cout << mouse_x << " " << mouse_y << std::endl;

    // Update and re-scale the horizonal angle.
    h_angle += (mouse_x * mouse_sensitivity);
    while(h_angle > 360.0f) h_angle -= 360.0f;
    while(h_angle < 0.0f) h_angle += 360.0f;

    float max_vert= 80.0f;

    // Update and re-scale the vertical angle.
    //v_angle += (mouse_y * mouse_sensitivity);
    //if (v_angle > max_vert) v_angle = max_vert;
    //if (v_angle < -max_vert) v_angle = -max_vert;

    // Reset the mouse position.
    glfwSetMousePos(0,0);
}

/**
 * Move the camera.
 */
void OpenGLView::move(double delta)
{
    float xvel = 0.0f;
    float yvel = 0.0f;
    float zvel = 0.0f;

    // Compute horizontal rotation in radians
    float xrot = h_angle;
    float xrotrad = xrot / 180 * 3.141592654f; 
    // Projection on x and z axes
    float xproj = max_speed * float(std::cos(xrotrad)); 
    float zproj = max_speed * float(std::sin(xrotrad));

    // Move left if 'a' is pressed.
    if (glfwGetKey('a') == GLFW_PRESS || glfwGetKey('A') == GLFW_PRESS)
    {
        xvel -= xproj;
        zvel -= zproj;
    }

    // Move forward if 'w' is pressed.
    if (glfwGetKey('w') == GLFW_PRESS || glfwGetKey('W') == GLFW_PRESS)
    {
        xvel += zproj;
        zvel -= xproj;
    }

    // Move back if 's' is pressed.
    if (glfwGetKey('s') == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS)
    {
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

void OpenGLView::update(double dt)
{
    look_around();
    move(dt);
}


/**
 * Encapsulates the method that occurs when this class is run in a Thread.
 */
void OpenGLView::run()
{
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
 * Set the factory representation
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
