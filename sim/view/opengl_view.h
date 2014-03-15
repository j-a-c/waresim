#ifndef RENDER_OPENGL_VIEW_H
#define RENDER_OPENGL_VIEW_H

#include <GL/gl.h>
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
        void set_factory(Factory *) override;
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

        // The factory representation.
        Factory *factory;
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

        // Holds the static factory VBO.
        GLuint factoryVBO = 0; 
        // Holds the worker VBO.
        GLuint workerVBO = 0;

        // Vector holding factory info.
        std::size_t factory_vert_len;
        std::size_t factory_norm_len;
        std::size_t factory_color_len;
        // The default factory color.
        std::vector<GLfloat> default_factory_color{
            0.75, 0.75, 0.75,  
            0.75, 0.75, 0.75,  
            0.75, 0.75, 0.75
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
        // The number of triangles it took to represent the factory.
        int num_factory_tris = 0;
        
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

};

#endif
