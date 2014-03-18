// In order to get function prototypes from glext.h, 
// define GL_GLEXT_PROTOTYPES before including glext.h
#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <GL/glfw.h>
#include <cmath>


#include "opengl_box.h"
#include "glext.h"
#include "opengl_view.h"
#include "../util/util.h"

// Delete after debug.
#include <iostream>

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
    GLfloat light_diffuse[] = {1.0, 0.5, 1.0, 1.0};

    // Enable a single OpenGL light.
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT0);

    // Automatically normalize normals.
    //glEnable(GL_NORMALIZE);

    // Back face culling
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glCullFace(GL_FRONT);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    
	// Allows z-buffering. Can chew up extra CPU time and graphics memory for 
    // very complicated scenes.
    // glDepthMask(GL_TRUE);

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

    // Enable vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);


    /*
     * Render the factory.
     */

    // Bind the factory VBO.
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, factoryVBO);

    // Set the pointers.
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, (void*) factory_vert_len);
    glColorPointer(3, GL_FLOAT, 0, 
            (void*) (factory_vert_len + factory_norm_len));

    glPushMatrix();
    glTranslated(0,0,-factory->get_height());
    glDrawArrays(GL_TRIANGLES, 0, num_factory_tris);
    glPopMatrix();
    
    /*
     * Render the workers.
     */

    // Bind the workers VBO.
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, workerVBO);
 
    // Set the pointers.
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, (void*) worker_vert_len);
    glColorPointer(3, GL_FLOAT, 0, 
            (void*) (worker_vert_len + worker_norm_len));

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
        glTranslated(x,0,z);
        glDrawArrays(GL_TRIANGLES, 0, num_worker_tris);
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
void OpenGLView::setup()
{

    // Create VBOs. These need to be deleted when the program exits.
    // We put vertex, normals, and colors in the same object.
    // Calling glBufferDataARB with NULL pointer reserves only memory space.

    // Create the factory VBO.
    glGenBuffersARB(1, &factoryVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, factoryVBO);

    std::vector<GLfloat> factory_verts{};
    std::vector<GLfloat> factory_norms{};
    std::vector<GLfloat> factory_colors{};

    // Add the factory floor. We use 'top' so we can see the floor.
    // We move the top -1 units down (so it acts as a floor) and re-align it.
    auto floor = OpenGLBox::get_top_vertices(factory->get_width(), 
            1, factory->get_height(), factory->get_width()/2.0f-0.5f, -1, 
            factory->get_height()/2.0f-0.5f);
    factory_verts.insert(factory_verts.end(), floor.begin(), floor.end());
    auto floorn = OpenGLBox::get_top_normals();
    factory_norms.insert(factory_norms.end(), floorn.begin(), floorn.end());
    for (int i = 0; i < 2; i++)
    {
        factory_colors.insert(factory_colors.end(), default_factory_color.begin(), 
                default_factory_color.end());
    }
    num_factory_tris += 2;

    // Note: the loops for adding the walls, bins, and drop locs is very
    // similar. However, we leave them separated in case we decide to render
    // each entity with a different representation than a box.

    // Add each wall.
    for (auto& pos : factory->get_walls())
    {
        num_factory_tris += 36;

        // Translate the position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, factory->get_width());

         // Form vertices.
        auto ffront = OpenGLBox::get_front_vertices(1, 1, 1, x, 0, z);
        auto fback = OpenGLBox::get_back_vertices(1, 1, 1, x, 0, z);
        auto ftop = OpenGLBox::get_top_vertices(1, 1, 1, x, 0, z);
        auto fbot = OpenGLBox::get_bot_vertices(1, 1, 1, x, 0, z);
        auto fleft = OpenGLBox::get_left_vertices(1, 1, 1, x, 0, z);
        auto fright = OpenGLBox::get_right_vertices(1, 1, 1, x, 0, z);

        // Add the vertices.
        factory_verts.insert(factory_verts.end(), ffront.begin(), ffront.end());
        factory_verts.insert(factory_verts.end(), fback.begin(), fback.end());
        factory_verts.insert(factory_verts.end(), ftop.begin(), ftop.end());
        factory_verts.insert(factory_verts.end(), fbot.begin(), fbot.end());
        factory_verts.insert(factory_verts.end(), fleft.begin(), fleft.end());
        factory_verts.insert(factory_verts.end(), fright.begin(), fright.end());

        // Form normals.
        auto fnfront = OpenGLBox::get_front_normals();
        auto fnback = OpenGLBox::get_back_normals();
        auto fntop = OpenGLBox::get_top_normals();
        auto fnbot = OpenGLBox::get_bot_normals();
        auto fnleft = OpenGLBox::get_left_normals();
        auto fnright = OpenGLBox::get_right_normals();

        // Add the normals.
        factory_norms.insert(factory_norms.end(), fnfront.begin(), fnfront.end());
        factory_norms.insert(factory_norms.end(), fnback.begin(), fnback.end());
        factory_norms.insert(factory_norms.end(), fntop.begin(), fntop.end());
        factory_norms.insert(factory_norms.end(), fnbot.begin(), fnbot.end());
        factory_norms.insert(factory_norms.end(), fnleft.begin(), fnleft.end());
        factory_norms.insert(factory_norms.end(), fnright.begin(), fnright.end());

        // Add the colors.
        // Insert twice for each face because each face is two triangles.
        for (int i = 0; i < 12; i++)
        {
            factory_colors.insert(factory_colors.end(), 
                    default_factory_color.begin(), default_factory_color.end());
        }   
    }

    // Add each bin.
    for (auto& pos : factory->get_bins())
    {
        num_factory_tris += 36;

        // Translate the position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, factory->get_width());

         // Form vertices.
        auto ffront = OpenGLBox::get_front_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fback = OpenGLBox::get_back_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto ftop = OpenGLBox::get_top_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fbot = OpenGLBox::get_bot_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fleft = OpenGLBox::get_left_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fright = OpenGLBox::get_right_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);

        // Add vertices.
        factory_verts.insert(factory_verts.end(), ffront.begin(), ffront.end());
        factory_verts.insert(factory_verts.end(), fback.begin(), fback.end());
        factory_verts.insert(factory_verts.end(), ftop.begin(), ftop.end());
        factory_verts.insert(factory_verts.end(), fbot.begin(), fbot.end());
        factory_verts.insert(factory_verts.end(), fleft.begin(), fleft.end());
        factory_verts.insert(factory_verts.end(), fright.begin(), fright.end());

        // Form normals.
        auto fnfront = OpenGLBox::get_front_normals();
        auto fnback = OpenGLBox::get_back_normals();
        auto fntop = OpenGLBox::get_top_normals();
        auto fnbot = OpenGLBox::get_bot_normals();
        auto fnleft = OpenGLBox::get_left_normals();
        auto fnright = OpenGLBox::get_right_normals();

        // Add the normals.
        factory_norms.insert(factory_norms.end(), fnfront.begin(), fnfront.end());
        factory_norms.insert(factory_norms.end(), fnback.begin(), fnback.end());
        factory_norms.insert(factory_norms.end(), fntop.begin(), fntop.end());
        factory_norms.insert(factory_norms.end(), fnbot.begin(), fnbot.end());
        factory_norms.insert(factory_norms.end(), fnleft.begin(), fnleft.end());
        factory_norms.insert(factory_norms.end(), fnright.begin(), fnright.end());

        // Add colors.
        // Insert twice for each face because each face is two triangles.
        for (int i = 0; i < 12; i++)
        {
            factory_colors.insert(factory_colors.end(), 
                    bin_color.begin(), bin_color.end());
        }   
    }

    // Add drop offs.
    for (auto& pos : factory->get_drops())
    {
        num_factory_tris += 36;

        // Translate the position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, factory->get_width());

         // Form vertices.
        auto ffront = OpenGLBox::get_front_vertices(1, 1, 1, x, 0, z);
        auto fback = OpenGLBox::get_back_vertices(1, 1, 1, x, 0, z);
        auto ftop = OpenGLBox::get_top_vertices(1, 1, 1, x, 0, z);
        auto fbot = OpenGLBox::get_bot_vertices(1, 1, 1, x, 0, z);
        auto fleft = OpenGLBox::get_left_vertices(1, 1, 1, x, 0, z);
        auto fright = OpenGLBox::get_right_vertices(1, 1, 1, x, 0, z);

        // Add vertices.
        factory_verts.insert(factory_verts.end(), ffront.begin(), ffront.end());
        factory_verts.insert(factory_verts.end(), fback.begin(), fback.end());
        factory_verts.insert(factory_verts.end(), ftop.begin(), ftop.end());
        factory_verts.insert(factory_verts.end(), fbot.begin(), fbot.end());
        factory_verts.insert(factory_verts.end(), fleft.begin(), fleft.end());
        factory_verts.insert(factory_verts.end(), fright.begin(), fright.end());

        // Form normals.
        auto fnfront = OpenGLBox::get_front_normals();
        auto fnback = OpenGLBox::get_back_normals();
        auto fntop = OpenGLBox::get_top_normals();
        auto fnbot = OpenGLBox::get_bot_normals();
        auto fnleft = OpenGLBox::get_left_normals();
        auto fnright = OpenGLBox::get_right_normals();

        // Add the normals.
        factory_norms.insert(factory_norms.end(), fnfront.begin(), fnfront.end());
        factory_norms.insert(factory_norms.end(), fnback.begin(), fnback.end());
        factory_norms.insert(factory_norms.end(), fntop.begin(), fntop.end());
        factory_norms.insert(factory_norms.end(), fnbot.begin(), fnbot.end());
        factory_norms.insert(factory_norms.end(), fnleft.begin(), fnleft.end());
        factory_norms.insert(factory_norms.end(), fnright.begin(), fnright.end());

        // Add colors.
        // Insert twice for each face because each face is two triangles.
        for (int i = 0; i < 12; i++)
        {
            factory_colors.insert(factory_colors.end(), 
                    drop_color.begin(), drop_color.end());
        }   
    }
    
    // Store lengths index lengths.
    factory_vert_len = sizeof(&factory_verts[0]) * factory_verts.size();
    factory_norm_len = sizeof(&factory_norms[0]) * factory_norms.size();
    factory_color_len = sizeof(&factory_colors[0]) * factory_colors.size();

    // Target, size, data, usage.
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
            factory_vert_len + factory_norm_len + factory_color_len, 
            0, GL_STATIC_DRAW_ARB);
    // Copy vertices starting from 0 offest.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, factory_vert_len, &factory_verts[0]); 
    // Copy normals after vertices.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, factory_vert_len, factory_norm_len, 
            &factory_norms[0]); 
    // Copy colors after normals.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, factory_vert_len + factory_norm_len, 
            factory_color_len, &factory_colors[0]);


    // Create the worker VBO.
    glGenBuffersARB(1, &workerVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, workerVBO);

    std::vector<GLfloat> worker_verts{}; 
    std::vector<GLfloat> worker_norms{}; 
    std::vector<GLfloat> worker_colors{}; 

    // Form vertices
    auto wfront = OpenGLBox::get_front_vertices(1, 1, 1, 0, 0, 0);
    auto wback = OpenGLBox::get_back_vertices(1, 1, 1, 0, 0, 0);
    auto wtop = OpenGLBox::get_top_vertices(1, 1, 1, 0, 0, 0);
    auto wbot = OpenGLBox::get_bot_vertices(1, 1, 1, 0, 0, 0);
    auto wleft = OpenGLBox::get_left_vertices(1, 1, 1, 0, 0, 0);
    auto wright = OpenGLBox::get_right_vertices(1, 1, 1, 0, 0, 0);

    worker_verts.insert(worker_verts.end(), wfront.begin(), wfront.end());
    worker_verts.insert(worker_verts.end(), wback.begin(), wback.end());
    worker_verts.insert(worker_verts.end(), wtop.begin(), wtop.end());
    worker_verts.insert(worker_verts.end(), wbot.begin(), wbot.end());
    worker_verts.insert(worker_verts.end(), wleft.begin(), wleft.end());
    worker_verts.insert(worker_verts.end(), wright.begin(), wright.end());

    // Form normals
    auto wnfront = OpenGLBox::get_front_normals();
    auto wnback = OpenGLBox::get_back_normals();
    auto wntop = OpenGLBox::get_top_normals();
    auto wnbot = OpenGLBox::get_bot_normals();
    auto wnleft = OpenGLBox::get_left_normals();
    auto wnright = OpenGLBox::get_right_normals();

    worker_norms.insert(worker_norms.end(), wnfront.begin(), wnfront.end());
    worker_norms.insert(worker_norms.end(), wnback.begin(), wnback.end());
    worker_norms.insert(worker_norms.end(), wntop.begin(), wntop.end());
    worker_norms.insert(worker_norms.end(), wnbot.begin(), wnbot.end());
    worker_norms.insert(worker_norms.end(), wnleft.begin(), wnleft.end());
    worker_norms.insert(worker_norms.end(), wnright.begin(), wnright.end());

    // Form colors
    // Insert twice for each face because each face is two triangles.
    for (int i = 0; i < 12; i++)
    {
        worker_colors.insert(worker_colors.end(), 
                worker_color.begin(), worker_color.end());
    }

    // Store lengths index lengths.
    worker_vert_len = sizeof(&worker_verts[0]) * worker_verts.size();
    worker_norm_len = sizeof(&worker_norms[0]) * worker_norms.size();
    worker_color_len = sizeof(&worker_colors[0]) * worker_colors.size();

    // Target, size, data, usage.
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
            worker_vert_len+worker_norm_len+worker_color_len, 
            0, GL_STATIC_DRAW_ARB);
    // Copy vertices starting from 0 offest.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, worker_vert_len, &worker_verts[0]); 
    // Copy normals after vertices.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, worker_vert_len, worker_norm_len, &worker_norms[0]); 
    // Copy colors after normals.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, worker_vert_len+worker_norm_len, worker_color_len, &worker_colors[0]);

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
