#include "opengl_box.h"

/*
 * What follows are the fix faces defined for a cube.
 */

std::vector<GLfloat> OpenGLBox::front_vertices{ 
     0.5, 0.5, 0.5, -0.5, 0.5, 0.5,  -0.5,-0.5, 0.5,
    -0.5,-0.5, 0.5,  0.5,-0.5, 0.5,   0.5, 0.5, 0.5
};

std::vector<GLfloat> OpenGLBox::right_vertices{ 
    0.5, 0.5, 0.5, 0.5,-0.5, 0.5, 0.5,-0.5,-0.5,
    0.5,-0.5,-0.5, 0.5, 0.5,-0.5, 0.5, 0.5, 0.5
};

std::vector<GLfloat> OpenGLBox::top_vertices{ 
    0.5, 0.5, 0.5,   0.5, 0.5,-0.5,  -0.5, 0.5,-0.5,      
   -0.5, 0.5,-0.5,  -0.5, 0.5, 0.5,   0.5, 0.5, 0.5,     
};

std::vector<GLfloat> OpenGLBox::left_vertices{ 
   -0.5, 0.5, 0.5,  -0.5, 0.5,-0.5,  -0.5,-0.5,-0.5,      
   -0.5,-0.5,-0.5,  -0.5,-0.5, 0.5,  -0.5, 0.5, 0.5,     
};

std::vector<GLfloat> OpenGLBox::bot_vertices{ 
   -0.5,-0.5,-0.5,   0.5,-0.5,-0.5,   0.5,-0.5, 0.5, 
    0.5,-0.5, 0.5,  -0.5,-0.5, 0.5,  -0.5,-0.5,-0.5,
};

std::vector<GLfloat> OpenGLBox::back_vertices{ 
    0.5,-0.5,-0.5,  -0.5,-0.5,-0.5,  -0.5, 0.5,-0.5,
   -0.5, 0.5,-0.5,   0.5, 0.5,-0.5,   0.5,-0.5,-0.5
};

/*
 * What follows are six normal vectors defined for the various faces of a cube.
 */

std::vector<GLfloat> OpenGLBox::front_normals{ 
    0, 0, 1,   0, 0, 1,   0, 0, 1,
    0, 0, 1,   0, 0, 1,   0, 0, 1
};

std::vector<GLfloat> OpenGLBox::right_normals{ 
    1, 0, 0,   1, 0, 0,   1, 0, 0,
    1, 0, 0,   1, 0, 0,   1, 0, 0
};

std::vector<GLfloat> OpenGLBox::top_normals{ 
    0, 1, 0,   0, 1, 0,   0, 1, 0,
    0, 1, 0,   0, 1, 0,   0, 1, 0,
};

std::vector<GLfloat> OpenGLBox::left_normals{ 
   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,
   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,
};

std::vector<GLfloat> OpenGLBox::bot_normals{ 
    0,-1, 0,   0,-1, 0,   0,-1, 0,
    0,-1, 0,   0,-1, 0,   0,-1, 0,
};

std::vector<GLfloat> OpenGLBox::back_normals{ 
    0, 0,-1,   0, 0,-1,   0, 0,-1,
    0, 0,-1,   0, 0,-1,   0, 0,-1 
};

/**
 * Returns the top vertices.
 *
 * @param scalex The scale of x coords.
 * @param scaley The scale of the y coords.
 * @param scalez The scale of the z coords.
 * @param offx The offsets of the x-vertices.
 * @param offy The offsets of the y-vertices.
 * @param offz The offsets of the z-vertices.
 */
std::vector<GLfloat> OpenGLBox::get_top_vertices(float scalex, float scaley, 
        float scalez, float offx, float offy, float offz)
{
    // Make a copy of the base vertices.
    std::vector<GLfloat> temp(top_vertices);

    // Apply the scaling factor.
    temp[0] *= scalex;
    temp[1] *= scaley;
    temp[2] *= scalez;
    temp[3] *= scalex;
    temp[4] *= scaley;
    temp[5] *= scalez;
    temp[6] *= scalex;
    temp[7] *= scaley;
    temp[8] *= scalez;
    temp[9] *= scalex;
    temp[10] *= scaley;
    temp[11] *= scalez;
    temp[12] *= scalex;
    temp[13] *= scaley;
    temp[14] *= scalez;
    temp[15] *= scalex;
    temp[16] *= scaley;
    temp[17] *= scalez;

    // Apply dimensional offsets.
    temp[0] += offx;
    temp[1] += offy;
    temp[2] += offz;
    temp[3] += offx;
    temp[4] += offy;
    temp[5] += offz;
    temp[6] += offx;
    temp[7] += offy;
    temp[8] += offz;
    temp[9] += offx;
    temp[10] += offy;
    temp[11] += offz;
    temp[12] += offx;
    temp[13] += offy;
    temp[14] += offz;
    temp[15] += offx;
    temp[16] += offy;
    temp[17] += offz;

    return temp;
}

/**
 * Returns the normals for the right face.
 */
std::vector<GLfloat> OpenGLBox::get_top_normals()
{
    return top_normals;
}

/**
 * Returns the bottom vertices.
 *
 * @param scalex The scale of x coords.
 * @param scaley The scale of the y coords.
 * @param scalez The scale of the z coords.
 * @param offx The offsets of the x-vertices.
 * @param offy The offsets of the y-vertices.
 * @param offz The offsets of the z-vertices.
 */
std::vector<GLfloat> OpenGLBox::get_bot_vertices(float scalex, float scaley, 
        float scalez, float offx, float offy, float offz)
{
    // Make a copy of the base vertices.
    std::vector<GLfloat> temp(bot_vertices);
    // Apply the scaling factor.
    temp[0] *= scalex;
    temp[1] *= scaley;
    temp[2] *= scalez;
    temp[3] *= scalex;
    temp[4] *= scaley;
    temp[5] *= scalez;
    temp[6] *= scalex;
    temp[7] *= scaley;
    temp[8] *= scalez;
    temp[9] *= scalex;
    temp[10] *= scaley;
    temp[11] *= scalez;
    temp[12] *= scalex;
    temp[13] *= scaley;
    temp[14] *= scalez;
    temp[15] *= scalex;
    temp[16] *= scaley;
    temp[17] *= scalez;
 
    // Apply dimensional offsets.
    temp[0] += offx;
    temp[1] += offy;
    temp[2] += offz;
    temp[3] += offx;
    temp[4] += offy;
    temp[5] += offz;
    temp[6] += offx;
    temp[7] += offy;
    temp[8] += offz;
    temp[9] += offx;
    temp[10] += offy;
    temp[11] += offz;
    temp[12] += offx;
    temp[13] += offy;
    temp[14] += offz;
    temp[15] += offx;
    temp[16] += offy;
    temp[17] += offz;

    return temp;
}

/**
 * Returns the normals for the right face.
 */
std::vector<GLfloat> OpenGLBox::get_bot_normals()
{
    return bot_normals;
}

/**
 * Returns the front vertices.
 *
 * @param scalex The scale of x coords.
 * @param scaley The scale of the y coords.
 * @param scalez The scale of the z coords.
 * @param offx The offsets of the x-vertices.
 * @param offy The offsets of the y-vertices.
 * @param offz The offsets of the z-vertices.
 */
std::vector<GLfloat> OpenGLBox::get_front_vertices( float scalex, float scaley,
        float scalez, float offx, float offy, float offz)
{
    // Make a copy of the base vertices.
    std::vector<GLfloat> temp(front_vertices);
    // Apply the scaling factor.
    temp[0] *= scalex;
    temp[1] *= scaley;
    temp[2] *= scalez;
    temp[3] *= scalex;
    temp[4] *= scaley;
    temp[5] *= scalez;
    temp[6] *= scalex;
    temp[7] *= scaley;
    temp[8] *= scalez;
    temp[9] *= scalex;
    temp[10] *= scaley;
    temp[11] *= scalez;
    temp[12] *= scalex;
    temp[13] *= scaley;
    temp[14] *= scalez;
    temp[15] *= scalex;
    temp[16] *= scaley;
    temp[17] *= scalez;

    // Apply dimensional offsets.
    temp[0] += offx;
    temp[1] += offy;
    temp[2] += offz;
    temp[3] += offx;
    temp[4] += offy;
    temp[5] += offz;
    temp[6] += offx;
    temp[7] += offy;
    temp[8] += offz;
    temp[9] += offx;
    temp[10] += offy;
    temp[11] += offz;
    temp[12] += offx;
    temp[13] += offy;
    temp[14] += offz;
    temp[15] += offx;
    temp[16] += offy;
    temp[17] += offz;

    return temp;

}

/**
 * Returns the normals for the right face.
 */
std::vector<GLfloat> OpenGLBox::get_front_normals()
{
    return front_normals;
}

/**
 * Returns the back vertices.
 *
 * @param scalex The scale of x coords.
 * @param scaley The scale of the y coords.
 * @param scalez The scale of the z coords.
 * @param offx The offsets of the x-vertices.
 * @param offy The offsets of the y-vertices.
 * @param offz The offsets of the z-vertices.
 */
std::vector<GLfloat> OpenGLBox::get_back_vertices( float scalex, float scaley,
        float scalez, float offx, float offy, float offz)
{
    // Make a copy of the base vertices.
    std::vector<GLfloat> temp(back_vertices);
    // Apply the scaling factor.
    temp[0] *= scalex;
    temp[1] *= scaley;
    temp[2] *= scalez;
    temp[3] *= scalex;
    temp[4] *= scaley;
    temp[5] *= scalez;
    temp[6] *= scalex;
    temp[7] *= scaley;
    temp[8] *= scalez;
    temp[9] *= scalex;
    temp[10] *= scaley;
    temp[11] *= scalez;
    temp[12] *= scalex;
    temp[13] *= scaley;
    temp[14] *= scalez;
    temp[15] *= scalex;
    temp[16] *= scaley;
    temp[17] *= scalez;

    // Apply dimensional offsets.
    temp[0] += offx;
    temp[1] += offy;
    temp[2] += offz;
    temp[3] += offx;
    temp[4] += offy;
    temp[5] += offz;
    temp[6] += offx;
    temp[7] += offy;
    temp[8] += offz;
    temp[9] += offx;
    temp[10] += offy;
    temp[11] += offz;
    temp[12] += offx;
    temp[13] += offy;
    temp[14] += offz;
    temp[15] += offx;
    temp[16] += offy;
    temp[17] += offz;

    return temp;

}

/**
 * Returns the normals for the right face.
 */
std::vector<GLfloat> OpenGLBox::get_back_normals()
{
    return back_normals;
}

/**
 * Returns the left vertices.
 *
 * @param scalex The scale of x coords.
 * @param scaley The scale of the y coords.
 * @param scalez The scale of the z coords.
 * @param offx The offsets of the x-vertices.
 * @param offy The offsets of the y-vertices.
 * @param offz The offsets of the z-vertices.
 */
std::vector<GLfloat> OpenGLBox::get_left_vertices( float scalex, float scaley,
        float scalez, float offx, float offy, float offz)
{
    // Make a copy of the base vertices.
    std::vector<GLfloat> temp(left_vertices);
    // Apply the scaling factor.
    temp[0] *= scalex;
    temp[1] *= scaley;
    temp[2] *= scalez;
    temp[3] *= scalex;
    temp[4] *= scaley;
    temp[5] *= scalez;
    temp[6] *= scalex;
    temp[7] *= scaley;
    temp[8] *= scalez;
    temp[9] *= scalex;
    temp[10] *= scaley;
    temp[11] *= scalez;
    temp[12] *= scalex;
    temp[13] *= scaley;
    temp[14] *= scalez;
    temp[15] *= scalex;
    temp[16] *= scaley;
    temp[17] *= scalez;

    // Apply dimensional offsets.
    temp[0] += offx;
    temp[1] += offy;
    temp[2] += offz;
    temp[3] += offx;
    temp[4] += offy;
    temp[5] += offz;
    temp[6] += offx;
    temp[7] += offy;
    temp[8] += offz;
    temp[9] += offx;
    temp[10] += offy;
    temp[11] += offz;
    temp[12] += offx;
    temp[13] += offy;
    temp[14] += offz;
    temp[15] += offx;
    temp[16] += offy;
    temp[17] += offz;

    return temp;

}

/**
 * Returns the normals for the right face.
 */
std::vector<GLfloat> OpenGLBox::get_left_normals()
{
    return left_normals;
}

/**
 * Returns the right vertices.
 *
 * @param scalex The scale of x coords.
 * @param scaley The scale of the y coords.
 * @param scalez The scale of the z coords.
 * @param offx The offsets of the x-vertices.
 * @param offy The offsets of the y-vertices.
 * @param offz The offsets of the z-vertices.
 */
std::vector<GLfloat> OpenGLBox::get_right_vertices( float scalex, float scaley,
        float scalez, float offx, float offy, float offz)
{
    // Make a copy of the base vertices.
    std::vector<GLfloat> temp(right_vertices);
    // Apply the scaling factor.
    temp[0] *= scalex;
    temp[1] *= scaley;
    temp[2] *= scalez;
    temp[3] *= scalex;
    temp[4] *= scaley;
    temp[5] *= scalez;
    temp[6] *= scalex;
    temp[7] *= scaley;
    temp[8] *= scalez;
    temp[9] *= scalex;
    temp[10] *= scaley;
    temp[11] *= scalez;
    temp[12] *= scalex;
    temp[13] *= scaley;
    temp[14] *= scalez;
    temp[15] *= scalex;
    temp[16] *= scaley;
    temp[17] *= scalez;

    // Apply dimensional offsets.
    temp[0] += offx;
    temp[1] += offy;
    temp[2] += offz;
    temp[3] += offx;
    temp[4] += offy;
    temp[5] += offz;
    temp[6] += offx;
    temp[7] += offy;
    temp[8] += offz;
    temp[9] += offx;
    temp[10] += offy;
    temp[11] += offz;
    temp[12] += offx;
    temp[13] += offy;
    temp[14] += offz;
    temp[15] += offx;
    temp[16] += offy;
    temp[17] += offz;

    return temp;

}

/**
 * Returns the normals for the right face.
 */
std::vector<GLfloat> OpenGLBox::get_right_normals()
{
    return right_normals;
}

