//
//  Assignment3Header.h
//  Assignment 3
//
//  Created by Stephen Board on 4/16/18.
//  Copyright © 2018 Stephen Board. All rights reserved.
//

#ifndef Assignment3Header_h
#define Assignment3Header_h


#endif /* Assignment3Header_h */

std::vector<GLfloat> ident ();

std::vector<GLfloat> init_unit_plane();

GLfloat* vector2array(std::vector<GLfloat> vec);

std::vector<GLfloat> to_homogenous_coord(std::vector<GLfloat> cartesian_coords);

std::vector<GLfloat> to_cartesian_coord(std::vector<GLfloat> homogenous_coords);

std::vector<GLfloat> translate (float dx, float dy, float dz);

std::vector<GLfloat> scale (float sx, float sy, float sz);

std::vector<GLfloat> rotate (char axis, double theta);

std::vector<GLfloat> mat_mult(std::vector<GLfloat> A, std::vector<GLfloat> B);

GLfloat dot_prod(std::vector<GLfloat> A, std::vector<GLfloat> B);

std::vector<GLfloat> apply_shading(std::vector<GLfloat> flat_colors, std::vector<GLfloat> light_source, std::vector<GLfloat> camera, std::vector<GLfloat> normals);
void spin(double theta);
void setup();

void display_func();

void idle_func();

std::vector<GLfloat> build_cube();

void init_camera();

std::vector<std::vector<GLfloat>> double_concat(std::vector<std::vector<GLfloat>> A, std::vector<std::vector<GLfloat>> B);

std::vector<GLfloat> double_to_single(std::vector<std::vector<GLfloat>> A);

std::vector<std::vector<GLfloat>> init_heart();

GLfloat* init_color();
