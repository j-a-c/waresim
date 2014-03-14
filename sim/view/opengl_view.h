#ifndef RENDER_OPENGL_VIEW_H
#define RENDER_OPENGL_VIEW_H

#include <GL/gl.h>

#include "view.h"

/**
 * Renders the simulation using OpenGL.
 */
class OpenGLView : public View
{
    public:
        OpenGLView();
        ~OpenGLView();

        void run();

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

        // Constant to convert degrees to radians.
        const float DEG2RAD = 3.141593f / 180;

        // Rotation speed of the camera.
        const float ROTATE_SPEED = 2.0f;

        // The max speed of the camera.
        float max_speed = 6;

        // The horizontal and vertical rotations (in degrees).
        float h_angle = 0.0f;
        float v_angle = 0.0f;

        // The x, y and z position (in OpenGL coordinates).
        float x_pos = 0.0f;
        float y_pos = 0.0f;
        float z_pos = 10.0f;

        // Holds the static factory VBO.
        GLuint factoryVBP = 0; 
        // Holds a cube VBO.
        GLuint cubeVBO = 0;
};

#endif
