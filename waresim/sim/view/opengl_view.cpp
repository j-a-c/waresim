// In order to get function prototypes from glext.h, 
// define GL_GLEXT_PROTOTYPES before including glext.h
#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <GL/glfw.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader_utils.h"

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
    glDeleteBuffersARB(1, &warehouseVBO);

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

    // Create a window 80% of the screen.
    desktop.Width *= 0.8;
    desktop.Height *= 0.8;
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

    // Initialize FreeType.
    FT_Library ft;
     
    if (FT_Init_FreeType(&ft)) 
    {
        // TODO Unable to initialize the FreeType library.
        std::cout << "Unable to load FreeType library" << std::endl;
    }

    // Load a font.
    if (FT_New_Face(ft, "fonts/FreeSans.ttf", 0, &face)) 
    {
        // TODO Unable to load font.
        std::cout << "Unable to load font." << std::endl;
    }

    // Set the pixel size.
    FT_Set_Pixel_Sizes(face, 0, 14);

    // Create the text shader.
	textProgram = create_program("shaders/text.v.glsl", "shaders/text.f.glsl");
	if(textProgram == 0)
        std::cout << "Error creating program." << std::endl;

    // Get various attributes.
	attribute_coord = get_attrib(textProgram, "coord");
	uniform_tex = get_uniform(textProgram, "tex");
	uniform_color = get_uniform(textProgram, "color");

    // Check that the attributes are set.
	if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1)
		std::cout << "Error setting shader attributes." << std::endl;

	// Create the vertex buffer object for the text.
	glGenBuffers(1, &textVBO);

    // Set the viewing size.
    glViewport(0, 0, (GLsizei)desktop.Width, (GLsizei)desktop.Height);
    
    // Clear stuff before rendering.
    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void OpenGLView::render_text(const char *text, float x, float y, float sx, float sy) 
{
	const char *p;
	FT_GlyphSlot g = face->glyph;

	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Loop through all characters */
	for (p = text; *p; p++) 
    {
		/* Try to load and render the character */
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{x2, -y2, 0, 0},
			{x2 + w, -y2, 1, 0},
			{x2, -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
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
    //GLfloat light_diffuse[] = {1.0, 0.5, 1.0, 1.0};

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
     * Render the warehouse.
     */

    // Bind the warehouse VBO.
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, warehouseVBO);

    // Set the pointers.
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, (void*) warehouse_vert_len);
    glColorPointer(3, GL_FLOAT, 0, 
            (void*) (warehouse_vert_len + warehouse_norm_len));

    glPushMatrix();
    glTranslated(0,0,-warehouse->get_height());
    glDrawArrays(GL_TRIANGLES, 0, num_warehouse_tris);
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
    for (auto& pos : warehouse->get_worker_locs())
    {
        // Translate the worker's position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, warehouse->get_width());
 
        // Translate the 'y' coordinate to OpenGL's z coordinate.
        z -= warehouse->get_height();
        
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

    // Determine how many points from the warehouse to group together, and how 
    // many points paint at a time on the screen. We will group 'group_w'
    // points together when calculating that color to paint 'paint_w' pixels.
    // We need to group (or spread) the points because the warehouse
    // representation and the allocated space to draw the statistics are
    // different sizes.
    int group_w, group_h, paint_w, paint_h;
    int warehouse_w = warehouse->get_width();
    int warehouse_h = warehouse->get_height();
    
    // There are less warehouse points than available points.
    if (warehouse_w < stat_w)
    {
        group_w = 1;
        paint_w = stat_w / warehouse_w;
    }
    else // There are more warehouse points than available points.
    {
        // TODO Implement.
        throw "not implemented yet";
    }
    // There are less warehouse points than available points. 
    if (warehouse_h < stat_h)
    {
        group_h = 1;
        paint_h = stat_h / warehouse_h;
    }
    else // There are more warehouse points that available points.
    {
        // TODO Implement.
        throw "not implemented yet";
    }
    
    // Track the max heat window.
    double max_heat_window = 0;
    auto heat_window = warehouse->get_heat_window();
    std::vector<double> hw_vals{};
    
    double max_heat_total = 0;
    auto heat_total = warehouse->get_heat_total();
    std::vector<double> ht_vals{};

    double max_contentions = 0;
    auto contentions = warehouse->get_contention_spots();
    std::vector<double> cont_vals{};

    double max_deadlocks = 0;
    auto deadlocks = warehouse->get_deadlock_spots();
    std::vector<double> dead_vals{};

    // TODO Clean up this loop and the following loop.
    // TODO We run this loop twice because the first time we find the value,
    // which we will use in the following loop to normalize the values before
    // we render them.
    for (int x_offset = 0, num_x = 0; num_x <= ceil(1.0*warehouse_w / group_w); x_offset+=group_w, num_x++)
    {
        for (int y_offset = 0, num_y = 0; num_y <= ceil(1.0*warehouse_h/ group_h); y_offset+=group_h, num_y++)
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

            for (int x = 0; x < group_w && x+x_offset < warehouse_w; x++)
            {
                for (int y = 0; y < group_h && y+y_offset < warehouse_h; y++)
                {
                    group_size++;
                    int pos = coord_to_pos(x+x_offset, y+y_offset, warehouse_w);

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
                // Average the values. We need this because if the warehouse size
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

    // We need to align the text based on the last point where pixels were
    // painted for the statistics. I didn't have time to derive the equation
    // for this so I will calculate it at runtime.
    int text_x_align = 0;

    glBegin(GL_TRIANGLES);
    int index = 0;
    // This is the loop where we will actually render the statistics.
    // TODO this is basically the same as the previous loop, but until we clean
    // up the previous loop's logic we have to do this.
    for (int x_offset = 0, num_x = 0; num_x <= ceil(1.0*warehouse_w / group_w); x_offset+=group_w, num_x++)
    {
        for (int y_offset = 0, num_y = 0; num_y <= ceil(1.0*warehouse_h/ group_h); y_offset+=group_h, num_y++)
        {    

            int group_size = 0;
            for (int x = 0; x < group_w && x+x_offset < warehouse_w; x++)
                for (int y = 0; y < group_h && y+y_offset < warehouse_h; y++)
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

                // Update where the text needs to be aligned. This corresponds
                // to the right-most pixel that was painted.
                text_x_align = paint_w * x_offset + paint_w;

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
                glVertex2f(paint_w*x_offset, paint_h*y_offset + stat_h + STAT_SEP);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+paint_h+stat_h + STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+stat_h + STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+stat_h + STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+stat_h + STAT_SEP);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+stat_h + STAT_SEP);

                // Draw the contentions.
                set_glColor_heat(cont_sum);
                glVertex2f(paint_w*x_offset, paint_h*y_offset + 2*stat_h + 2*STAT_SEP);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+paint_h+2*stat_h + 2*STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+2*stat_h + 2*STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+2*stat_h + 2*STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+2*stat_h + 2*STAT_SEP);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+2*stat_h + 2*STAT_SEP);

                // Draw the deadlocks.
                set_glColor_heat(dead_sum);
                glVertex2f(paint_w*x_offset, paint_h*y_offset + 3*stat_h + 3*STAT_SEP);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+paint_h+3*stat_h + 3*STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+3*stat_h + 3*STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+paint_h+3*stat_h + 3*STAT_SEP);
                glVertex2f(paint_w*x_offset+paint_w, paint_h*y_offset+3*stat_h + 3*STAT_SEP);
                glVertex2f(paint_w*x_offset, paint_h*y_offset+3*stat_h + 3*STAT_SEP);
                
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

    /*
     * Render text for the statistics.
     */

    // Scale for the text.
    float sx = 2.0 / screen_w;
	float sy = 2.0 / screen_h;

    // Shader for text.
	glUseProgram(textProgram);

	// Enable blending, necessary for our alpha texture.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We will color the font white.
	GLfloat white[4] = { 1, 1, 1, 1 };

	// Set font size to, set the color.
    FT_Set_Pixel_Sizes(face, 0, 24);
	glUniform4fv(uniform_color, 1, white);

    // Render the actual text. Each text needs to be aligned with the top of
    // its corresponding stat rendering and then moved down a tab.
    // TODO Rotate 90 degrees for a nicer rendering.

    // We will offset the text from the statistics by 5 pixels so it looks nicer.
    text_x_align += STAT_SEP;
	// Heat window's text.
    render_text("Heat window", -1 + text_x_align * sx, 1 - (0 + 25) * sy, sx, sy);
    // Total heat's text.
    render_text("Total heat", -1 + text_x_align * sx, 1 - (stat_h + 25) * sy, sx, sy);
    // Contention's text.
    render_text("Contentions", -1 + text_x_align * sx, 1 - (2*stat_h + 25) * sy, sx, sy);
    // Deadlock's text.
    render_text("Deadlocks", -1 + text_x_align * sx, 1 - (3*stat_h + 25) * sy, sx, sy);

    // Stop using the text shader.
    glUseProgram(0);


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
 * Initialize all the VBOs for the workers and warehouse.
 * Calculate the render size for on-screen statistics.
 */
void OpenGLView::setup()
{
    int warehouse_w = warehouse->get_width();
    int warehouse_h = warehouse->get_height();

    // Allocate space to each statistic.
    // Dedicate 1/4 of screen width to stats. 
    // Each stat will have an equal amount of height.
    int alloc_w = 0.25 * screen_w;
    int alloc_h = (screen_h - (STAT_SEP * (NUM_STATS-1))) / NUM_STATS;
    // We need to fit the warehouse image to the allocated space.
    double stat_r = alloc_w / alloc_h;
    double warehouse_r = 1.0 * warehouse_w / warehouse_h;
    
    // These two cases determine which axis we should scale the warehouse by.
    if (warehouse_r < stat_r)
    {
        stat_h = alloc_h;
        stat_w = std::round(stat_h * warehouse_r);

        double scale = 1.0;
        // We need to rescale if the warehouse statistic allocation is larger
        // than the allocated space.
        while (stat_w > alloc_w)
        {
            scale -= 0.05;
            stat_h = scale * alloc_h;
            stat_w = scale * std::round(stat_h * warehouse_r);
        }
    }
    else
    {
        stat_w = alloc_w;
        stat_h = std::round(stat_w / warehouse_r);

        // We need to re-scale if stat_h is bigger than the dimension allocated.
        double scale = 1.0;
        // We need to rescale if the warehouse statistic allocation is larger
        // than the allocated space.
        while (stat_h > alloc_h)
        {
            scale -= 0.05;
            stat_w = scale * alloc_w;
            stat_h = scale * std::round(stat_w / warehouse_r);

        }
    }
        
    // Create VBOs. These need to be deleted when the program exits.
    // We put vertex, normals, and colors in the same object.
    // Calling glBufferDataARB with NULL pointer reserves only memory space.

    // Create the warehouse VBO.
    glGenBuffersARB(1, &warehouseVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, warehouseVBO);

    std::vector<GLfloat> warehouse_verts{};
    std::vector<GLfloat> warehouse_norms{};
    std::vector<GLfloat> warehouse_colors{};

    // Add the warehouse floor. We use 'top' so we can see the floor.
    // We move the top -1 units down (so it acts as a floor) and re-align it.
    auto floor = OpenGLBox::get_top_vertices(warehouse->get_width(), 
            1, warehouse->get_height(), warehouse->get_width()/2.0f-0.5f, -1, 
            warehouse->get_height()/2.0f-0.5f);
    warehouse_verts.insert(warehouse_verts.end(), floor.begin(), floor.end());
    auto floorn = OpenGLBox::get_top_normals();
    warehouse_norms.insert(warehouse_norms.end(), floorn.begin(), floorn.end());
    for (int i = 0; i < 2; i++)
    {
        warehouse_colors.insert(warehouse_colors.end(), default_warehouse_color.begin(), 
                default_warehouse_color.end());
    }
    num_warehouse_tris += 2;

    // Note: the loops for adding the walls, bins, and drop locs is very
    // similar. However, we leave them separated in case we decide to render
    // each entity with a different representation than a box.

    // Add each wall.
    for (auto& pos : warehouse->get_walls())
    {
        num_warehouse_tris += 36;

        // Translate the position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, warehouse->get_width());

         // Form vertices.
        auto ffront = OpenGLBox::get_front_vertices(1, 1, 1, x, 0, z);
        auto fback = OpenGLBox::get_back_vertices(1, 1, 1, x, 0, z);
        auto ftop = OpenGLBox::get_top_vertices(1, 1, 1, x, 0, z);
        auto fbot = OpenGLBox::get_bot_vertices(1, 1, 1, x, 0, z);
        auto fleft = OpenGLBox::get_left_vertices(1, 1, 1, x, 0, z);
        auto fright = OpenGLBox::get_right_vertices(1, 1, 1, x, 0, z);

        // Add the vertices.
        warehouse_verts.insert(warehouse_verts.end(), ffront.begin(), ffront.end());
        warehouse_verts.insert(warehouse_verts.end(), fback.begin(), fback.end());
        warehouse_verts.insert(warehouse_verts.end(), ftop.begin(), ftop.end());
        warehouse_verts.insert(warehouse_verts.end(), fbot.begin(), fbot.end());
        warehouse_verts.insert(warehouse_verts.end(), fleft.begin(), fleft.end());
        warehouse_verts.insert(warehouse_verts.end(), fright.begin(), fright.end());

        // Form normals.
        auto fnfront = OpenGLBox::get_front_normals();
        auto fnback = OpenGLBox::get_back_normals();
        auto fntop = OpenGLBox::get_top_normals();
        auto fnbot = OpenGLBox::get_bot_normals();
        auto fnleft = OpenGLBox::get_left_normals();
        auto fnright = OpenGLBox::get_right_normals();

        // Add the normals.
        warehouse_norms.insert(warehouse_norms.end(), fnfront.begin(), fnfront.end());
        warehouse_norms.insert(warehouse_norms.end(), fnback.begin(), fnback.end());
        warehouse_norms.insert(warehouse_norms.end(), fntop.begin(), fntop.end());
        warehouse_norms.insert(warehouse_norms.end(), fnbot.begin(), fnbot.end());
        warehouse_norms.insert(warehouse_norms.end(), fnleft.begin(), fnleft.end());
        warehouse_norms.insert(warehouse_norms.end(), fnright.begin(), fnright.end());

        // Add the colors.
        // Insert twice for each face because each face is two triangles.
        for (int i = 0; i < 12; i++)
        {
            warehouse_colors.insert(warehouse_colors.end(), 
                    default_wall_color.begin(), default_wall_color.end());
        }   
    }

    // Add each bin.
    for (auto& pos : warehouse->get_bins())
    {
        num_warehouse_tris += 36;

        // Translate the position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, warehouse->get_width());

         // Form vertices.
        auto ffront = OpenGLBox::get_front_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fback = OpenGLBox::get_back_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto ftop = OpenGLBox::get_top_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fbot = OpenGLBox::get_bot_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fleft = OpenGLBox::get_left_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);
        auto fright = OpenGLBox::get_right_vertices(BIN_X, BIN_Y, BIN_Z, x, 0, z);

        // Add vertices.
        warehouse_verts.insert(warehouse_verts.end(), ffront.begin(), ffront.end());
        warehouse_verts.insert(warehouse_verts.end(), fback.begin(), fback.end());
        warehouse_verts.insert(warehouse_verts.end(), ftop.begin(), ftop.end());
        warehouse_verts.insert(warehouse_verts.end(), fbot.begin(), fbot.end());
        warehouse_verts.insert(warehouse_verts.end(), fleft.begin(), fleft.end());
        warehouse_verts.insert(warehouse_verts.end(), fright.begin(), fright.end());

        // Form normals.
        auto fnfront = OpenGLBox::get_front_normals();
        auto fnback = OpenGLBox::get_back_normals();
        auto fntop = OpenGLBox::get_top_normals();
        auto fnbot = OpenGLBox::get_bot_normals();
        auto fnleft = OpenGLBox::get_left_normals();
        auto fnright = OpenGLBox::get_right_normals();

        // Add the normals.
        warehouse_norms.insert(warehouse_norms.end(), fnfront.begin(), fnfront.end());
        warehouse_norms.insert(warehouse_norms.end(), fnback.begin(), fnback.end());
        warehouse_norms.insert(warehouse_norms.end(), fntop.begin(), fntop.end());
        warehouse_norms.insert(warehouse_norms.end(), fnbot.begin(), fnbot.end());
        warehouse_norms.insert(warehouse_norms.end(), fnleft.begin(), fnleft.end());
        warehouse_norms.insert(warehouse_norms.end(), fnright.begin(), fnright.end());

        // Add colors.
        // Insert twice for each face because each face is two triangles.
        for (int i = 0; i < 12; i++)
        {
            warehouse_colors.insert(warehouse_colors.end(), 
                    bin_color.begin(), bin_color.end());
        }   
    }

    // Add drop offs.
    for (auto& pos : warehouse->get_drops())
    {
        num_warehouse_tris += 36;

        // Translate the position to a coordinate.
        int x,z;
        pos_to_coord(&x, &z, pos, warehouse->get_width());

         // Form vertices.
        auto ffront = OpenGLBox::get_front_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fback = OpenGLBox::get_back_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto ftop = OpenGLBox::get_top_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fbot = OpenGLBox::get_bot_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fleft = OpenGLBox::get_left_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);
        auto fright = OpenGLBox::get_right_vertices(DROP_X, DROP_Y, DROP_Z, x, 0, z);

        // Add vertices.
        warehouse_verts.insert(warehouse_verts.end(), ffront.begin(), ffront.end());
        warehouse_verts.insert(warehouse_verts.end(), fback.begin(), fback.end());
        warehouse_verts.insert(warehouse_verts.end(), ftop.begin(), ftop.end());
        warehouse_verts.insert(warehouse_verts.end(), fbot.begin(), fbot.end());
        warehouse_verts.insert(warehouse_verts.end(), fleft.begin(), fleft.end());
        warehouse_verts.insert(warehouse_verts.end(), fright.begin(), fright.end());

        // Form normals.
        auto fnfront = OpenGLBox::get_front_normals();
        auto fnback = OpenGLBox::get_back_normals();
        auto fntop = OpenGLBox::get_top_normals();
        auto fnbot = OpenGLBox::get_bot_normals();
        auto fnleft = OpenGLBox::get_left_normals();
        auto fnright = OpenGLBox::get_right_normals();

        // Add the normals.
        warehouse_norms.insert(warehouse_norms.end(), fnfront.begin(), fnfront.end());
        warehouse_norms.insert(warehouse_norms.end(), fnback.begin(), fnback.end());
        warehouse_norms.insert(warehouse_norms.end(), fntop.begin(), fntop.end());
        warehouse_norms.insert(warehouse_norms.end(), fnbot.begin(), fnbot.end());
        warehouse_norms.insert(warehouse_norms.end(), fnleft.begin(), fnleft.end());
        warehouse_norms.insert(warehouse_norms.end(), fnright.begin(), fnright.end());

        // Add colors.
        // Insert twice for each face because each face is two triangles.
        for (int i = 0; i < 12; i++)
        {
            warehouse_colors.insert(warehouse_colors.end(), 
                    drop_color.begin(), drop_color.end());
        }   
    }
    
    // Store lengths index lengths.
    warehouse_vert_len = sizeof(&warehouse_verts[0]) * warehouse_verts.size();
    warehouse_norm_len = sizeof(&warehouse_norms[0]) * warehouse_norms.size();
    warehouse_color_len = sizeof(&warehouse_colors[0]) * warehouse_colors.size();

    // Target, size, data, usage.
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
            warehouse_vert_len + warehouse_norm_len + warehouse_color_len, 
            0, GL_STATIC_DRAW_ARB);
    // Copy vertices starting from 0 offest.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, warehouse_vert_len, &warehouse_verts[0]); 
    // Copy normals after vertices.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, warehouse_vert_len, warehouse_norm_len, 
            &warehouse_norms[0]); 
    // Copy colors after normals.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, warehouse_vert_len + warehouse_norm_len, 
            warehouse_color_len, &warehouse_colors[0]);


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
 * Set the warehouse representation.
 * We also build the static warehouse layout before returning.
 */
void OpenGLView::set_warehouse(Warehouse *warehouse)
{
    this->warehouse = warehouse;
}

/**
 * Set the simulation parameters.
 */
void OpenGLView::set_sim_params(time_t start_time, int sim_length)
{
    this->start_time = start_time;
    this->sim_length = sim_length;
}
