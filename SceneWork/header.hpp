//
//  header.h
//  cubeSpin
//
//  Created by Stephen Board on 3/2/18.
//  Copyright © 2018 Stephen Board. All rights reserved.
//

#ifndef header_h
#define header_h

#include <stdio.h>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#endif /* header_h */

std::vector<GLfloat> ident ();

std::vector<GLfloat> init_unit_plane();

GLfloat* vector2array(std::vector<GLfloat> vec);

std::vector<GLfloat> to_homogenous_coord(std::vector<GLfloat> cartesian_coords);

std::vector<GLfloat> to_cartesian_coord(std::vector<GLfloat> homogenous_coords);

std::vector<GLfloat> translate (float dx, float dy, float dz);

std::vector<GLfloat> scale (float sx, float sy, float sz);

std::vector<GLfloat> rotate (char axis, double theta);

//std::vector<GLfloat> rotate_x (float theta);

std::vector<GLfloat> rotate_y (float theta);

std::vector<GLfloat> rotate_z (float theta);

std::vector<GLfloat> mat_mult(std::vector<GLfloat> A, std::vector<GLfloat> B);

void setup();

void display_func(GLfloat vertices[]);

std::vector<std::vector<GLfloat>> build_cube();

void init_camera();

GLfloat* init_scene();

GLfloat* init_color();





