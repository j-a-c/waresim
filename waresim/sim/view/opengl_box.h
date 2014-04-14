#ifndef SIM_VIEW_OPENGL_BOX
#define SIM_VIEW_OPENGL_BOX

#include <GL/gl.h>

#include <vector>

/**
 * Box representation in OpenGL triangle form.
 * Provides factory methods that return factory methods for parts of a box.
 */
class OpenGLBox
{
    public:
        static std::vector<GLfloat> get_top_vertices(float, float, float, float, float, float);
        static std::vector<GLfloat> get_top_normals();

        static std::vector<GLfloat> get_bot_vertices(float, float, float, float, float, float);
        static std::vector<GLfloat> get_bot_normals();

        static std::vector<GLfloat> get_front_vertices(float, float, float, float, float, float);
        static std::vector<GLfloat> get_front_normals();

        static std::vector<GLfloat> get_back_vertices(float, float, float, float, float, float);
        static std::vector<GLfloat> get_back_normals();

        static std::vector<GLfloat> get_left_vertices(float, float, float, float, float, float);
        static std::vector<GLfloat> get_left_normals();

        static std::vector<GLfloat> get_right_vertices(float, float, float, float, float, float);
        static std::vector<GLfloat> get_right_normals();

    private:

        // Vertex array for a cube.
        static std::vector<GLfloat> front_vertices; 
        static std::vector<GLfloat> right_vertices; 
        static std::vector<GLfloat> top_vertices; 
        static std::vector<GLfloat> left_vertices; 
        static std::vector<GLfloat> bot_vertices; 
        static std::vector<GLfloat> back_vertices; 

        // Normal array for a cube.
        static std::vector<GLfloat> front_normals; 
        static std::vector<GLfloat> right_normals; 
        static std::vector<GLfloat> top_normals; 
        static std::vector<GLfloat> left_normals; 
        static std::vector<GLfloat> bot_normals; 
        static std::vector<GLfloat> back_normals; 
};

#endif
