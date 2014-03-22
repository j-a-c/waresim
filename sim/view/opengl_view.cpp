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

// TODO Delete after debug.
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
    screen_w = desktop.Width;
    screen_h = desktop.Height; 

    if (glfwOpenWindow(screen_w, screen_h, 8, 8, 8, 8, 24, 8, GLFW_WINDOW) != GL_TRUE)
        shut_down();

    // Initialize GLEW
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0"))
    {
        // TODO
    }

    glViewport(0, 0, (GLsizei)desktop.Width, (GLsizei)desktop.Height);
    
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

    // Keep current colors.
	glEnable(GL_COLOR_MATERIAL);
    
    // White diffuse light.
    GLfloat light_diffuse[] = {1.0, 0.5, 1.0, 1.0};

    // Enable a single OpenGL light.
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    //glEnable(GL_LIGHT0);

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
 * Sets the OpenGL color for a value mapped to a heatmap, where blue = 0.0 and
 * red = 1.0. 
 *
 * @param val A double value in the range [0.0,1.0].
 */
void set_glColor_heat(double val)
{
    // Maps the RGB range (0,0,1) to (0,1,1) 
    // for values in the range [0, 0.25).
    if (0 <= val && val < 0.25)
    {
        glColor3f(0, val/0.25, 1);
        return;
    }

    val -= 0.25;
    // Maps the RGB range (0,1,1) to (0,1,0) 
    // for values in the range [0.25, 0.50).
    if (0 <= val && val < 0.25)
    {
        glColor3f(0, 1, 1-(val/0.25));
        return;
    }

    val -= 0.25;
    // Maps the RGB range (0,1,0) to (1,1,0) 
    // for values in the range [0.50, 0.75).
    if (0 <= val && val < 0.25)
    {
        glColor3f(val/0.25, 1, 0);
        return;
    }

    val -= 0.25;
    // Maps the RGB range (1,1,0) to (1,0,0) 
    // for values in the range [0.75, 1).
    glColor3f(1, 1-(val/0.25), 0);
    return;
}

/**
 * Rendering function
 */
void OpenGLView::render()
{
    // Prep for 3D rendering.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective (angle of sight, perspective, depth)
    gluPerspective(60, (GLfloat)screen_w/ (GLfloat)screen_h, 0.1, 100.0); 
    glMatrixMode(GL_MODELVIEW);
    // Prep for 3D drawing.
    glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
    // Re-load the orientation.
    glLoadIdentity();

    // Clean screen to black.
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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


    /*
     * Render 2D statistic projections.
     */

    // Prep for 2D rendering.
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screen_w, screen_h, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    // Trick for exact pixelation.
    glTranslatef(0.375, 0.375, 0);

    /*
     * Render statistics.
     */

    // Determine how many points from the factory to group together, and how 
    // many points paint at a time on the screen. We will group 'group_w'
    // points together when calculating that color to paint 'paint_w' pixels.
    // We need to group (or spread) the points because the factory
    // representation and the allocated space to draw the statistics are
    // different sizes.
    int group_w, group_h, paint_w, paint_h;
    int factory_w = factory->get_width();
    int factory_h = factory->get_height();
    
    // There are less factory points than available points.
    if (factory_w < stat_w)
    {
        group_w = 1;
        paint_w = stat_w / factory_w;
    }
    else // There are more factory points than available points.
    {
        // TODO Implement.
        throw "not implemented yet";
    }
    // There are less factory points than available points. 
    if (factory_h < stat_h)
    {
        group_h = 1;
        paint_h = stat_h / factory_h;
    }
    else // There are more factory points that available points.
    {
        // TODO Implement.
        throw "not implemented yet";
    }
    
    // Track the max heat window.
    double max_heat_window = 0;
    auto heat_window = factory->get_heat_window();
    std::vector<double> hw_vals{};
    
    double max_heat_total = 0;
    auto heat_total = factory->get_heat_total();
    std::vector<double> ht_vals{};

    double max_contentions = 0;
    auto contentions = factory->get_contention_spots();
    std::vector<double> cont_vals{};

    double max_deadlocks = 0;
    auto deadlocks = factory->get_deadlock_spots();
    std::vector<double> dead_vals{};

    // TODO Clean up this loop and the following loop.
    // TODO We run this loop twice because the first time we find the value,
    // which we will use in the following loop to normalize the values before
    // we render them.
    for (int x_offset = 0, num_x = 0; num_x <= ceil(1.0*factory_w / group_w); x_offset+=group_w, num_x++)
    {
        for (int y_offset = 0, num_y = 0; num_y <= ceil(1.0*factory_h/ group_h); y_offset+=group_h, num_y++)
        {
            // Start forming this group.
            int group_size = 0;

            // Sum for the heat window.
            double hw_sum = 0;
            // Sum for the total heat.
            double ht_sum = 0;
            // Sum for the contentions.
            double cont_sum = 0;
            // Sum for the deadlocks.
            double dead_sum = 0;

            for (int x = 0; x < group_w && x+x_offset < factory_w; x++)
            {
                for (int y = 0; y < group_h && y+y_offset < factory_h; y++)
                {
                    group_size++;
                    int pos = coord_to_pos(x+x_offset, y+y_offset, factory_w);

                    // Update values.
                    hw_sum += heat_window[pos];
                    ht_sum += heat_total[pos];
                    auto cont_it = contentions.find(pos);
                    if (cont_it != contentions.end()) cont_sum += cont_it->second;
                    auto dead_it = deadlocks.find(pos);
                    if (dead_it != deadlocks.end()) dead_sum += dead_it->second;
                }
            }
             
            // Calculate values for this group.
            if (group_size > 0)
            {
                // Average the values. We need this because if the factory size
                // was greater than the viewing window, we will need to merge
                // some points when rendering.
                hw_sum /= group_size;
                ht_sum /= group_size;
                cont_sum /= group_size;
                dead_sum /= group_size;

                // Update the max values.
                if (hw_sum > max_heat_window) max_heat_window = hw_sum;
                if (ht_sum > max_heat_total) max_heat_total = ht_sum;
                if (cont_sum > max_contentions) max_contentions = cont_sum;
                if (dead_sum > max_deadlocks) max_deadlocks = dead_sum;

                // Save the values.
                hw_vals.push_back(hw_sum);
                ht_vals.push_back(ht_sum);
                cont_vals.push_back(cont_sum);
                dead_vals.push_back(dead_sum);
            }
        }
    }

    glBegin(GL_TRIANGLES);
    int index = 0;
    // This is the loop where we will actually render the statistics.
    // TODO this is basically the same as the previous loop, but until we clean
    // up the previous loop's logic we have to do this.
    for (int x_offset = 0, num_x = 0; num_x <= ceil(1.0*factory_w / group_w); x_offset+=group_w, num_x++)
    {
        for (int y_offset = 0, num_y = 0; num_y <= ceil(1.0*factory_h/ group_h); y_offset+=group_h, num_y++)
        {    

            int group_size = 0;
            for (int x = 0; x < group_w && x+x_offset < factory_w; x++)
                for (int y = 0; y < group_h && y+y_offset < factory_h; y++)
                    group_size++;

            // Calculate the average for this group and paint.
            if (group_size > 0)
            {

                // Get the values.
                double hw_sum = hw_vals[index];
                double ht_sum = ht_vals[index];
                double cont_sum = cont_vals[index];
                double dead_sum = dead_vals[index];

                // Normalize the values.
                if (max_heat_window > 0)
                    hw_sum /= max_heat_window;
                if (max_heat_total > 0)
                    ht_sum /= max_heat_total;

                // Update the index.
                index++;

                // TODO Optimize. Draw pre-colored VBOs instead.

                // Draw the heat window.
                set_glColor_heat(hw_sum); 
                glVertex2f(paint_w*x_offset, paint_h*y_offset);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+paint_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset);
                glVertex2f(paint_w*x_offset, paint_h*y_offset);

                // Draw the total heat.
                set_glColor_heat(ht_sum);
                glVertex2f(paint_w*x_offset, paint_h*y_offset + stat_h);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+paint_h+stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+stat_h);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+stat_h);

                // Draw the contentions.
                set_glColor_heat(cont_sum);
                glVertex2f(paint_w*x_offset, paint_h*y_offset + 2*stat_h);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+paint_h+2*stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+2*stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+2*stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+2*stat_h);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+2*stat_h);

                // Draw the deadlocks.
                set_glColor_heat(dead_sum);
                glVertex2f(paint_w*x_offset, paint_h*y_offset + 3*stat_h);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+paint_h+3*stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+3*stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+3*stat_h);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+3*stat_h);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+3*stat_h);
                
            } 
        }
    } 
    glEnd();
    glPopMatrix();
    
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
 * Set up some things beforehand.
 * Initialize all the VBOs.
 * Calculate the render size for on-screen statistics.
 */
void OpenGLView::setup()
{
    int factory_w = factory->get_width();
    int factory_h = factory->get_height();
    // Allocate space to each statistic.
    // Dedicate 1/4 of screen width to stats. 
    // Each stat will have an equal amount of height.
    int alloc_w = 0.25 * screen_w;
    int alloc_h = screen_h / NUM_STATS;
    // We need to fit the factory image to the allocated space.
    double stat_r = alloc_w / alloc_h;
    double factory_r = 1.0 * factory_w / factory_h;
    std::cout << "alloc: " << alloc_w << " " << alloc_h << std::endl;
    if (factory_r < stat_r)
    {
        stat_h = alloc_h;
        stat_w = std::round(stat_h * factory_r);
    }
    else
    {
        stat_w = alloc_w;
        stat_h = std::round(stat_w / factory_r);
    }
    std::cout << "fact dim: " << factory_w << " " << factory_h << std::endl; 
    std::cout << "stat dim: " << stat_w <<  " " << stat_h << std::endl;
    
    // Determine the granularity for the statistics renderings.
    // TODO
    

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
                    default_wall_color.begin(), default_wall_color.end());
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
        auto ffront = OpenGLBox::get_front_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fback = OpenGLBox::get_back_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto ftop = OpenGLBox::get_top_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fbot = OpenGLBox::get_bot_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fleft = OpenGLBox::get_left_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fright = OpenGLBox::get_right_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);

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
