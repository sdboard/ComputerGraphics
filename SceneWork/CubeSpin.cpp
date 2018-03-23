/***************************************************************************************************************
 Assignment-2: Geometric Modeling of a Scene

  Name: Board, Stephen
 
  Project Summary: This will make a scene consisting of objects composed of rudimentary objects such as a cube
                    or diamond
  Collaborators: Wong, Alex (Professor)
 
 ****************************************************************************************************************/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <vector>
#include "header.hpp"
#include <cassert>
#include <stdlib.h>
using namespace std;
float theta = 0.0;
int numPoints;
int num_heart_Points;
int num_star_Points;
double star_point = -2.236;
GLfloat h;
GLfloat* scene_vertices;
GLfloat* heart_vertices;
GLfloat* star_vertices;
GLfloat* heart_paint;
GLfloat* star_paint;
vector<GLfloat> scene_Vector;
vector<GLfloat> heart_Vector;
vector<GLfloat> star_Vector;
vector<GLfloat> colorOR = {
    // O R A N G E
    1.0,    0.627,   0.478,
};
vector<GLfloat> colorYE = {
    // Y E L L O W
    1.0,    1.0,   0.0,
};


/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

// Initializes a square plane of unit lengths
vector<GLfloat> init_unit_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }

    return arr;
}

/*
 Converts Cartesian coordinates to homogeneous coordinates by appending
 a 1 in the 4th dimension
 inputs: a list of suceessive 3-demensional vectors
*/
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    
    if( (int)cartesian_coords.size() % 3 == 0 ) { // make sure we have a valid cartesian coordinate input
        
        for (int i = 0; i < (int) cartesian_coords.size(); i++ ){
            
            if ( (i+1) % 3 != 0){ // if we're not at the last coordinate, add the given elements

                result.push_back(cartesian_coords[i]);
    
            } else { // else add the given element then add the homogenous coordinate
                
                result.push_back(cartesian_coords[i]);
                
                result.push_back(1.0);
                
            }
        }
        

    } else { // not a valid cartesian point set
    }
    
    return result;
}

/*
 Converts homogeneous coordinates to Cartesian coordinates by removing
 the 1 in the 4th dimension
 inputs: a list of suceessive 4-demensional vectors
 */
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
    vector<GLfloat> result;
    
    if( (int)homogeneous_coords.size() % 4 == 0 ) { // make sure we have a valid homogenous coordinate input
    
        for (int i = 1; i < (int)homogeneous_coords.size() + 1; i++ ){
            
            if ( i % 4 != 0){ // if we're not at the homogeneous coordinate spot, add the given elements
                
                result.push_back(homogeneous_coords[i-1]);
                
            } else { // else if we are at the homogenous coordinate
                     // do nothing, skip the homogeneous coordinate
            }
        }
    } else { // not a valid homogeneous point set
    }
    
    return result;
}

/*
  Returns the homogeneous identity matrix
*/
vector<GLfloat> ident () {
    vector<GLfloat> id_mat = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    
    return id_mat;
}

/*
 Returns a custom translation matrix
 inputs: x, y, and z translation specifications
*/
vector<GLfloat> translate (float dx, float dy, float dz, vector<GLfloat> plane) {
    
    vector<GLfloat> translate_mat = {
        1.0, 0.0, 0.0, dx,
        0.0, 1.0, 0.0, dy,
        0.0, 0.0, 1.0, dz,
        0.0, 0.0, 0.0, 1.0
    };
    
    return to_cartesian_coord(mat_mult(translate_mat,to_homogenous_coord(plane)));
}

/*
 Returns a custom scaling matrix
 inputs: x, y, and z scaling specifications
*/
vector<GLfloat> scale (float sx, float sy, float sz, vector<GLfloat> plane) {
    vector<GLfloat> scale_mat = {
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    return to_cartesian_coord(mat_mult(scale_mat,to_homogenous_coord(plane)));
}

/*
 Returns a rotation matrix about the axis by theta degrees
 inputs: axis as a char ('x', 'y', or 'z')
         theta in degrees (90.1)
*/
vector<GLfloat> rotate (char axis, double theta, vector<GLfloat> plane) {
    
    vector<GLfloat> rotate_mat;
    
    theta = theta * 3.1415926536 / 180; //convert to radians
    
    GLfloat costheta = (GLfloat) cos(theta);
    GLfloat sintheta = (GLfloat) sin(theta);
 
    if (axis == 'x') {
        rotate_mat = {
            1.0, 0.0, 0.0, 0.0,
            0.0, costheta, -sintheta, 0.0,
            0.0, sintheta, costheta, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        
    } else if (axis == 'y'){
        rotate_mat = {
            costheta, 0.0, sintheta, 0.0,
            0.0, 1.0, 0.0, 0.0,
            -sintheta,  0.0, costheta, 0.0,
            0.0, 0.0, 0.0, 1.0
        };

    } else if (axis == 'z'){
        rotate_mat = {
            costheta, -sintheta, 0.0, 0.0,
            sintheta, costheta, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };

    } else {
//        return vector<GLfloat> nullptr;
    }
    
    return to_cartesian_coord(mat_mult(rotate_mat,to_homogenous_coord(plane)));
}


/*
 Perform matrix multiplication for square matricies A and B
 requires a 4x4 and a list of points as the inputs A and B respectively
*/
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    
    if ( ((int)A.size() == 16) and ((int)B.size()%4 == 0) ) {
        for (int i = 0; i < B.size()/4; i++) {
            for (int j = 0; j < A.size(); j+=4) {
                GLfloat temp = 0;
                for (int k = 0; k < 4; k++) {
                    temp += (A[j + k] * B[(i*4) + k]);
                }
                result.push_back(temp);
                //delete temp;
            }
        }
    } else {
        //not a valid square matrix input
    }
    
    return result;
}

vector<vector<GLfloat>> double_concat(vector<vector<GLfloat>> A, vector<vector<GLfloat>> B) {
    vector<vector<GLfloat>> result;
    
    for (int i = 0; i < A.size(); i++) {
        result.push_back(A[i]);
    }
    
    for (int i = 0; i < B.size(); i++) {
        result.push_back(B[i]);
    }
    
    return result;
}

vector<GLfloat> double_to_single(vector<vector<GLfloat>> A) {
    vector<GLfloat> result;
    
    for (int i = 0; i < A.size(); i++) {
        vector<GLfloat> curVec = A[i];
        for (int j = 0; j < curVec.size(); j++) {
            result.push_back(curVec[j]);
        }
    }
    
    return result;
}

vector<GLfloat> build_diamond() {
    GLfloat a = sqrtf(2.0)/2;
    
    vector<GLfloat> init_plane = {
        +a,   +0.0,   +0.0,
        -a,   +0.0,   +0.0,
        +0.0,   +2*a,   +0.0,
    };
    
    vector<vector<GLfloat>> parts;
    vector<GLfloat> topfront = translate(0.0, 0.0, +a, rotate('x',-30,init_plane));
    parts.push_back(topfront);
    
    vector<GLfloat> topback = translate(0.0, 0.0, -a, rotate('x',+30,rotate('y',180,init_plane)));
    parts.push_back(topback);
    
    vector<GLfloat> topright = translate(+a, 0.0, 0.0, rotate('z',+30,rotate('y',+90,init_plane)));
    parts.push_back(topright);
    
    vector<GLfloat> topleft = translate(-a, 0.0, 0.0, rotate('z',-30,rotate('y',-90,init_plane)));
    parts.push_back(topleft);
    
    vector<GLfloat> botfront = translate(0.0, 0.0, +a, rotate('x',+30,rotate('z',180,init_plane)));
    parts.push_back(botfront);
    
    vector<GLfloat> botback = translate(0.0, 0.0, -a, rotate('x',-30,rotate('y',180,rotate('z',180,init_plane))));
    parts.push_back(botback);
    
    vector<GLfloat> botright = translate(+a, 0.0, 0.0, rotate('z',-30,rotate('y',+90,rotate('z',180,init_plane))));
    parts.push_back(botright);
    
    vector<GLfloat> botleft = translate(-a, 0.0, 0.0, rotate('z',+30,rotate('y',-90,rotate('z',180,init_plane))));
    parts.push_back(botleft);
    
    vector<GLfloat> result;
    for (int i = 0; i < parts.size(); i++) {
        vector<GLfloat> curVec = parts[i];
        for (int j = 0; j < curVec.size(); j++) {
            result.push_back(curVec[j]);
        }
    }
    result = scale(0.1, 0.1, 0.1, result);
    return result;
}


// Builds a unit cube of side length 'sl' centered at the origin
vector<GLfloat> build_cube() {
    GLfloat a = 1.0;
    
    // instantiate the vector square plane to be d
    vector<GLfloat> init_plane = {
    +a,   +a,   +0.0,
    +a,   -a,   +0.0,
    -a,   -a,   +0.0,
    -a,   +a,   +0.0
    };

    vector<vector<GLfloat>> result;
    
    // Creates a unit cube by transforming a set of planes
    
    vector<GLfloat> front = translate(0.0, 0.0, a, init_plane);
    result.push_back(front);
    
    vector<GLfloat> back = translate(0.0, 0.0, -a, rotate('y', 180, init_plane));
    result.push_back(back);
    
    vector<GLfloat> left = translate(-a, 0.0, 0.0, rotate('y', -90, init_plane));
    result.push_back(left);
    
    vector<GLfloat> right = translate(a, 0.0, 0.0, rotate('y', +90, init_plane));
    result.push_back(right);
    
    vector<GLfloat> top = translate(0.0, a, 0.0,rotate('x', +90, init_plane));
    result.push_back(top);
    
    vector<GLfloat> bot =  translate(0.0, -a, 0.0, rotate('x', -90, init_plane));
    result.push_back(bot);
    
    vector<GLfloat> lastRes;
    for (int i = 0; i < result.size(); i++) {
        vector<GLfloat> curVec = result[i];
        for (int j = 0; j < curVec.size(); j++) {
            lastRes.push_back(curVec[j]);
        }
    }
    
    return lastRes;
}




///**************************************************
// *            Camera and World Modeling           *
// *                                                *
// *  create a scene by applying transformations to *
// *  the objects built from planes and position    *
// *  the camera to view the scene by using setting *
// *  the projection viewing matrices               *
// *                                                *
// *************************************************/

void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}




void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
    gluPerspective(50.0, 1.0, 2.0, 10.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

vector<vector<GLfloat>> init_star() {
    vector<vector<GLfloat>> star_heap;
    vector<GLfloat> unit_diamond = scale(0.7, 0.7, 0.7, build_diamond());
//    vector<GLfloat> unit_diamond = build_diamond();
    star_heap.push_back(unit_diamond);
    star_heap.push_back(translate(0.0, 0.7, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.35, -0.55, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.35, -0.55, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.5, +0.25, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.5, +0.25, 0.0, unit_diamond));
    
    star_heap.push_back(translate(+0.35, +0.25, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.35, +0.25, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.2, +0.25, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.2, +0.25, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.12, +0.4, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.12, +0.4, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.05, +0.53, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.05, +0.53, 0.0, unit_diamond));
    
    star_heap.push_back(translate(+0.41, +0.14, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.41, +0.14, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.33, +0.05, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.33, +0.05, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.25, -0.05, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.25, -0.05, 0.0, unit_diamond));
    
    star_heap.push_back(translate(+0.27, -0.21, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.27, -0.21, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.315, -0.38, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.315, -0.38, 0.0, unit_diamond));
    
    star_heap.push_back(translate(+0.21, -0.47, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.21, -0.47, 0.0, unit_diamond));
    star_heap.push_back(translate(+0.11, -0.38, 0.0, unit_diamond));
    star_heap.push_back(translate(-0.11, -0.38, 0.0, unit_diamond));
    
    star_heap.push_back(translate(-0.00, -0.31, 0.0, unit_diamond));
    
    return star_heap;
}

// Construct the scene using objects built from cubes/prisms
vector<vector<GLfloat>> init_heart() {
    vector<vector<GLfloat>> heart_heap;
    vector<GLfloat> unit_obj = scale(0.1, 0.1, 0.1, build_cube());
    
    //center
    heart_heap.push_back(translate(0.0,0.42,0.0,unit_obj));
    
    heart_heap.push_back(translate(0.15,0.64,0.0,unit_obj));
    heart_heap.push_back(translate(-0.15,0.64,0.0,unit_obj));
    heart_heap.push_back(translate(0.37,0.79,0.0,unit_obj));
    heart_heap.push_back(translate(-0.37,0.79,0.0,unit_obj));
    heart_heap.push_back(translate(0.61,0.84,0.0,unit_obj));
    heart_heap.push_back(translate(-0.61,0.84,0.0,unit_obj));
    heart_heap.push_back(translate(0.78,0.63,0.0,unit_obj));
    heart_heap.push_back(translate(-0.78,0.63,0.0,unit_obj));
    heart_heap.push_back(translate(0.84,0.415,0.0,unit_obj));
    heart_heap.push_back(translate(-0.84,0.415,0.0,unit_obj));
    heart_heap.push_back(translate(0.82,0.19,0.0,unit_obj));
    heart_heap.push_back(translate(-0.82,0.19,0.0,unit_obj));
    heart_heap.push_back(translate(0.78,-0.03,0.0,unit_obj));
    heart_heap.push_back(translate(-0.78,-0.03,0.0,unit_obj));
    heart_heap.push_back(translate(0.69,-0.25,0.0,unit_obj));
    heart_heap.push_back(translate(-0.69,-0.25,0.0,unit_obj));
    heart_heap.push_back(translate(0.6,-0.47,0.0,unit_obj));
    heart_heap.push_back(translate(-0.6,-0.47,0.0,unit_obj));
    heart_heap.push_back(translate(0.43,-0.7,0.0,unit_obj));
    heart_heap.push_back(translate(-0.43,-0.7,0.0,unit_obj));
    heart_heap.push_back(translate(0.23,-0.92,0.0,unit_obj));
    heart_heap.push_back(translate(-0.23,-0.92,0.0,unit_obj));
    heart_heap.push_back(translate(0.0,-1.1,0.0,unit_obj));

//   +1
    heart_heap.push_back(translate(0.14,0.55,0.21,unit_obj));
    heart_heap.push_back(translate(-0.14,0.55,0.21,unit_obj));
    heart_heap.push_back(translate(0.35,0.7,0.21,unit_obj));
    heart_heap.push_back(translate(-0.35,0.7,0.21,unit_obj));
    heart_heap.push_back(translate(0.55,0.63,0.21,unit_obj));
    heart_heap.push_back(translate(-0.55,0.63,0.21,unit_obj));
    heart_heap.push_back(translate(0.63,0.415,0.21,unit_obj));
    heart_heap.push_back(translate(-0.63,0.415,0.21,unit_obj));
    heart_heap.push_back(translate(0.61,0.19,0.21,unit_obj));
    heart_heap.push_back(translate(-0.61,0.19,0.21,unit_obj));
    heart_heap.push_back(translate(0.58,-0.03,0.21,unit_obj));
    heart_heap.push_back(translate(-0.58,-0.03,0.21,unit_obj));
    heart_heap.push_back(translate(0.47,-0.25,0.21,unit_obj));
    heart_heap.push_back(translate(-0.47,-0.25,0.21,unit_obj));
    heart_heap.push_back(translate(0.34,-0.47,0.21,unit_obj));
    heart_heap.push_back(translate(-0.34,-0.47,0.21,unit_obj));
    heart_heap.push_back(translate(0.22,-0.7,0.21,unit_obj));
    heart_heap.push_back(translate(-0.22,-0.7,0.21,unit_obj));
    heart_heap.push_back(translate(0.1,-0.92,0.21,unit_obj));
    heart_heap.push_back(translate(-0.1,-0.92,0.21,unit_obj));

//    -1
    heart_heap.push_back(translate(0.14,0.55,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.14,0.55,-0.21,unit_obj));
    heart_heap.push_back(translate(0.35,0.7,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.35,0.7,-0.21,unit_obj));
    heart_heap.push_back(translate(0.55,0.63,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.55,0.63,-0.21,unit_obj));
    heart_heap.push_back(translate(0.63,0.415,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.63,0.415,-0.21,unit_obj));
    heart_heap.push_back(translate(0.61,0.19,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.61,0.19,-0.21,unit_obj));
    heart_heap.push_back(translate(0.58,-0.03,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.58,-0.03,-0.21,unit_obj));
    heart_heap.push_back(translate(0.47,-0.25,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.47,-0.25,-0.21,unit_obj));
    heart_heap.push_back(translate(0.34,-0.47,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.34,-0.47,-0.21,unit_obj));
    heart_heap.push_back(translate(0.22,-0.7,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.22,-0.7,-0.21,unit_obj));
    heart_heap.push_back(translate(0.1,-0.92,-0.21,unit_obj));
    heart_heap.push_back(translate(-0.1,-0.92,-0.21,unit_obj));
 
    
//    +2
    heart_heap.push_back(translate(0.15,0.5,0.36,scale(1.0,1.0,0.5,unit_obj)));
    heart_heap.push_back(translate(-0.15,0.5,0.36,scale(1.0,1.0,0.5,unit_obj)));
    heart_heap.push_back(translate(0.4,0.42,0.32,unit_obj));
    heart_heap.push_back(translate(-0.4,0.42,0.32,unit_obj));
    heart_heap.push_back(translate(0.43,0.19,0.32,unit_obj));
    heart_heap.push_back(translate(-0.43,0.19,0.32,unit_obj));
    heart_heap.push_back(translate(0.38,-0.03,0.32,unit_obj));
    heart_heap.push_back(translate(-0.38,-0.03,0.32,unit_obj));
    heart_heap.push_back(translate(0.27,-0.25,0.32,unit_obj));
    heart_heap.push_back(translate(-0.27,-0.25,0.32,unit_obj));
    heart_heap.push_back(translate(0.14,-0.47,0.32,unit_obj));
    heart_heap.push_back(translate(-0.14,-0.47,0.32,unit_obj));
    heart_heap.push_back(translate(0.0,-0.7,0.32,unit_obj));
    
    //    -2
    heart_heap.push_back(translate(0.15,0.5,-0.36,scale(1.0,1.0,0.5,unit_obj)));
    heart_heap.push_back(translate(-0.15,0.5,-0.36,scale(1.0,1.0,0.5,unit_obj)));
    heart_heap.push_back(translate(0.4,0.42,-0.32,unit_obj));
    heart_heap.push_back(translate(-0.4,0.42,-0.32,unit_obj));
    heart_heap.push_back(translate(0.43,0.19,-0.32,unit_obj));
    heart_heap.push_back(translate(-0.43,0.19,-0.32,unit_obj));
    heart_heap.push_back(translate(0.38,-0.03,-0.32,unit_obj));
    heart_heap.push_back(translate(-0.38,-0.03,-0.32,unit_obj));
    heart_heap.push_back(translate(0.27,-0.25,-0.32,unit_obj));
    heart_heap.push_back(translate(-0.27,-0.25,-0.32,unit_obj));
    heart_heap.push_back(translate(0.14,-0.47,-0.32,unit_obj));
    heart_heap.push_back(translate(-0.14,-0.47,-0.32,unit_obj));
    heart_heap.push_back(translate(0.0,-0.7,-0.32,unit_obj));
    
   
//    +3
    heart_heap.push_back(translate(0.15,0.33,0.38,scale(1.0,0.6,1.0,unit_obj)));
    heart_heap.push_back(translate(-0.15,0.33,0.38,scale(1.0,0.6,1.0,unit_obj)));
    heart_heap.push_back(translate(0.18,0.16,0.39,unit_obj));
    heart_heap.push_back(translate(-0.18,0.16,0.39,unit_obj));
    heart_heap.push_back(translate(0.17,-0.07,0.39,unit_obj));
    heart_heap.push_back(translate(-0.17,-0.07,0.39,unit_obj));
    heart_heap.push_back(translate(0.0,-0.27,0.39,unit_obj));
    
    heart_heap.push_back(translate(0.0,0.155,0.42,scale(0.5,0.95,1.0,unit_obj)));
    heart_heap.push_back(translate(0.0,-0.06,0.42,scale(0.5,0.9,1.0,unit_obj)));
    heart_heap.push_back(translate(0.0,0.33,0.4,scale(0.5,0.5,1.0,unit_obj)));

    //    -3
    heart_heap.push_back(translate(0.15,0.33,-0.38,scale(1.0,0.6,1.0,unit_obj)));
    heart_heap.push_back(translate(-0.15,0.33,-0.38,scale(1.0,0.6,1.0,unit_obj)));
    heart_heap.push_back(translate(0.18,0.16,-0.39,unit_obj));
    heart_heap.push_back(translate(-0.18,0.16,-0.39,unit_obj));
    heart_heap.push_back(translate(0.17,-0.07,-0.39,unit_obj));
    heart_heap.push_back(translate(-0.17,-0.07,-0.39,unit_obj));
    heart_heap.push_back(translate(0.0,-0.27,-0.39,unit_obj));
    
    heart_heap.push_back(translate(0.0,0.155,-0.42,scale(0.5,0.95,1.0,unit_obj)));
    heart_heap.push_back(translate(0.0,-0.06,-0.42,scale(0.5,0.9,1.0,unit_obj)));
    heart_heap.push_back(translate(0.0,0.33,-0.4,scale(0.5,0.5,1.0,unit_obj)));
    

    vector<GLfloat> scene;
//    for (int i = 0; i < heart_heap.size(); i++) {
//        vector<GLfloat> curVec = heart_heap[i];
//        for (int j = 0; j < curVec.size(); j++) {
//            scene.push_back(curVec[j]);
//        }
//    }
    scene = translate(1.2, -0.6, -2.4, scale(0.4, 0.4, 0.4, scene));
    return heart_heap;
}

// Construct the scene using objects built from cubes/prisms
void spin_heart(double theta) {
    heart_Vector = translate(1.2, -0.6, -2.4, rotate('y',theta,translate(-1.2, 0.6, 2.4, heart_Vector)));
    heart_vertices = vector2array(heart_Vector);
}

void jump_heart() {
    h = (5*star_point) - (star_point*star_point*star_point)/3;
    
//    star_Vector = translate(-0.0,(h/1500),0.0,star_Vector);
    
//    if (star_point < 2.236) {
//        star_point += 0.01;
//    } else if (star_point > -2.236){
//        star_point -= 0.01;
//    }
    
    GLfloat t = (GLfloat) sin((star_point++)/10); // + sin((star_point++)/1000);
    star_Vector = translate(-0.0,(t/15),0.0,star_Vector);
//    star_Vector = translate(-0.8,0.2,0.4, rotate('y',theta,translate(-1.2, 0.6, 2.4, star_Vector)));
    
    star_vertices = vector2array(star_Vector);
}

void bounce_star(double y){
    
}

vector<GLfloat> single_concat (vector<GLfloat> A, vector<GLfloat> B){
    for (int i = 0; i < B.size(); i++) {
        A.push_back(B[i]);
    }
    return A;
}

vector<GLfloat> init_color_vec (vector<GLfloat> vertices, vector<GLfloat> RGB ) {
    vector<GLfloat> result;
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = 0; j < 3; j++) {
            result.push_back(RGB[j]);
        }
    }
    return result;
}

GLfloat* texturize(GLfloat colorArr[], int m) {
    GLfloat* result = new GLfloat[m];
    
    for (int i = 0; i < m; i++) {
        double iRand = rand() % 300;
        GLfloat iNoise = (float) (iRand/1000) + 0.75;
        result[i] = colorArr[i]*iNoise;

    }
    
    return result;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    spin_heart(5);
    jump_heart();
    
    
    glRotatef(theta/3, 0.0, 1.0, 0.0);
//    glRotatef(theta/2, 1.0, 0.0, 0.0);
    
    // Perform display functions
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    heart_vertices);  // Pointer to memory location to read from
    
    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   heart_paint);     // Pointer to memory location to read from

    glDrawArrays(GL_QUADS, 0, num_heart_Points);
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    star_vertices);  // Pointer to memory location to read from
    
    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   star_paint);     // Pointer to memory location to read from
    
    glDrawArrays(GL_TRIANGLES, 0, num_star_Points);
    
    
    glFlush();			//Finish rendering
    glutSwapBuffers();
}


void idle_func() {
    theta = theta+1.0;
    display_func();
}
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************

int main (int argc, char **argv) {
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    // draw heart
    heart_Vector = double_to_single(init_heart());
    heart_Vector = translate(1.2, -0.6, -2.4, scale(0.4, 0.4, 0.4, heart_Vector));
    
    num_heart_Points = (int)heart_Vector.size()/3;
    vector<GLfloat> heart_color_vec = init_color_vec(heart_Vector, colorOR);
    

    heart_paint = vector2array(heart_color_vec);
    heart_paint = texturize(heart_paint, (num_heart_Points*3));
    heart_vertices = vector2array(heart_Vector);
    
    
    star_Vector = double_to_single(init_star());
    star_Vector = translate(-0.8,0.2,0.4,scale(0.6,0.6,0.6,star_Vector));
    //star_Vector = build_diamond();
    num_star_Points = (int) star_Vector.size()/3;
    vector<GLfloat> star_color_vec = init_color_vec(star_Vector, colorYE);
    star_paint = vector2array(star_color_vec);
    star_paint = texturize(star_paint, (num_star_Points*3));
    star_vertices = vector2array(star_Vector);

    
    std::cout<<numPoints<<"\n";

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 2");
    //
    setup();
    init_camera();
    
    //     Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);  
    
    // Render our world
    glutMainLoop();
    delete[] heart_vertices;
    delete[] heart_paint;
    delete[] star_vertices;
    delete[] star_paint;
    return 0;
}


/*
 //    scene_vertices = vector2array(scene_Vector);
 //    paint = init_color(scene_Vector, colorOR);
 
 
 // draw heart
 vector<vector<GLfloat>> heart_double_vec = init_heart();
 vector<GLfloat> heart_vec = double_to_single(heart_double_vec);
 heart_vec = translate(1.2, -0.6, -2.4, scale(0.4, 0.4, 0.4, heart_vec));
 
 // color heart
 vector<GLfloat> heart_color_vec = init_color_vec(heart_vec, colorOR);
 
 // draw star
 vector<vector<GLfloat>> star_double_vec = init_star();
 vector<GLfloat> star_vec = double_to_single(star_double_vec);
 // color star
 vector<GLfloat> star_color_vec = init_color_vec(star_vec, colorYE);
 
 scene_Vector = single_concat(heart_vec, star_vec);
 scene_vertices = vector2array(scene_Vector);
 numPoints = (int)scene_Vector.size()/3;
 paint = vector2array(single_concat(heart_color_vec, star_color_vec));
 
 paint = texturize(paint, (numPoints*3));
 
*/
