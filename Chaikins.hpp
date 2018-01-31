//
//  Chaikins.hpp
//  OpenGL_test
//
//  Created by Stephen Board on 1/24/18.
//  Copyright © 2018 Stephen Board. All rights reserved.
//

#ifndef Chaikins_hpp
#define Chaikins_hpp

#include <stdio.h>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#endif /* Chaikins_hpp */

class Vertex {
    GLfloat x, y;
public:
    Vertex(GLfloat, GLfloat);
    GLfloat get_y() { return y; };
    GLfloat get_x() { return x; };
};


struct Coordinate{
    GLfloat x, y;
    Coordinate(GLfloat x_, GLfloat y_) : x(x_), y(y_) {}
};


std::vector<Coordinate> generate_points(std::vector<Coordinate> control_points);
