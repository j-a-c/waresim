#ifndef RENDER_OPENGL_VIEW_H
#define RENDER_OPENGL_VIEW_H

#ifndef SIM_GL_H
#define SIM_GL_H
#include <GL/gl.h>
#endif
#include <cstddef>
#include <vector>

#include "view.h"

/**
 * Renders the simulation using OpenGL.
 */
class OpenGLView : public View
{
    public:
        OpenGLView();
        ~OpenGLView();

        // Initialize the VBOs we will render. Must be called before run().
        void setup() override;

        void run() override;

        // Override from View.
        void set_warehouse(Warehouse *) override;
        // Override from View.
        void set_sim_params(time_t, int);
    private:
        // Override from Thread.
        //void run() override;
        
        // Initialize parameters.
        void init();
        // Enable OpenGL functions.
        void enable();
        // Update the state of the world.
        void update(double);
        // Render the view.
        void render();
        // Handle mouse events.
        void look_around();
        // Move the camera.
        void move(double);
        // Shut down the OpenGL view.
        void shut_down();

        // Dimensions of the render view.
        int screen_w, screen_h;
        // Dimensions of a statistics image.
        int stat_w, stat_h;
        // The number of statistics we will render.
        double NUM_STATS = 4.0;

        // The warehouse representation.
        Warehouse *warehouse;
        // The start time of the simulation.
        time_t start_time;
        // The legnth of the simulation in seconds.
        int sim_length;

        // Dimensions are defined in terms of UNITs.
        const float UNIT = 1.0f;

        // Constant to convert degrees to radians.
        const float DEG2RAD = 3.141593f / 180;

        // Rotation speed of the camera.
        const float ROTATE_SPEED = 1.0f;

        // The max speed of the camera.
        float max_speed = 6;

        // The horizontal and vertical rotations (in degrees).
        // Horizonal is side to size, vertical is up and down.
        float h_angle = 0.0f;
        float v_angle = 45.0f;

        // The x, y and z position (in OpenGL coordinates).
        float x_pos = 0.0f;
        float y_pos = 5.0f;
        float z_pos = 5.0f;

        // Holds the static warehouse VBO.
        GLuint warehouseVBO = 0; 
        // Holds the worker VBO.
        GLuint workerVBO = 0;

        // Vector holding warehouse info.
        std::size_t warehouse_vert_len;
        std::size_t warehouse_norm_len;
        std::size_t warehouse_color_len;
        // The default warehouse color.
        std::vector<GLfloat> default_warehouse_color{
            0.75, 0.75, 0.75,  
            0.75, 0.75, 0.75,  
            0.75, 0.75, 0.75
        };
        // The default wall color. We use a different color than the rest of
        // the warehouse because we have not implemented shadows, so it is hard
        // to tell the difference.
        std::vector<GLfloat> default_wall_color{
            0.6, 0.6, 0.6,  
            0.6, 0.6, 0.6,  
            0.6, 0.6, 0.6
        };
        // The bin color.
        std::vector<GLfloat> drop_color{
            1, 1, 0,  
            1, 1, 0,  
            1, 1, 0
        };
        // The drop-off color.
        std::vector<GLfloat> bin_color{
            0.4, 0.2, 0,  
            0.4, 0.2, 0,  
            0.4, 0.2, 0
        };
        // The number of triangles it took to represent the warehouse.
        int num_warehouse_tris = 0;
        
        // Worker data.
        std::size_t worker_vert_len;
        std::size_t worker_norm_len;
        std::size_t worker_color_len;
        // Color array for a worker's cube.
        std::vector<GLfloat> worker_color{
            1, 0, 0, 
            1, 0, 0, 
            1, 0, 0
        };
        // We need 36 triangles to form a cube.
        int num_worker_tris = 36;

        // Bin dimensions.
        const float BIN_X = 0.75;
        const float BIN_Y = 0.75;
        const float BIN_Z = 0.75;
        // Drop dimensions.
        const float DROP_X = 0.75;
        const float DROP_Y = 0.75;
        const float DROP_Z = 0.75;


};

#endif
