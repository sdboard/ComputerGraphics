/***
 Assignment-2: Geometric Modeling of a Scene

 Name: Board, Stephen

 Project Summary: This will make a scene. Firt commit to github currently since I have done the basics including testing.

***/



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
using namespace std;

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

// Builds a unit cube of side length 'sl' centered at the origin
vector<GLfloat> build_cube(GLfloat sl) {
    GLfloat a = sl/2;
    
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
    
    vector<GLfloat> test_front = {
        +a,   +a,   +a,
        +a,   -a,   +a,
        -a,   -a,   +a,
        -a,   +a,   +a
    };
    
    
    bool same = true;
    for (int i = 0; i < front.size(); i++) {
        if (front[i] != test_front[i]) {
            same = false;
            i = (int)front.size();
        }
        //lastRes.push_back(curVec[j]);
    }
    
    if (same) std::cout << "\n front plane passed \n";
    
    result.push_back(front);
    
    vector<GLfloat> back = translate(0.0, 0.0, -a, rotate('y', 180, init_plane));
    
    vector<GLfloat> test_back = {
        -a,   +a,   -a,
        -a,   -a,   -a,
        +a,   -a,   -a,
        +a,   +a,   -a
    };
    
    for (int i = 0; i < back.size(); i++) {
        if (back[i] != test_back[i]) {
            same = false;
            i = (int)back.size();
        }
    }
    
    if (same) std::cout << "\n back plane passed \n";
    
    result.push_back(back);
    
    vector<GLfloat> left = translate(-a, 0.0, 0.0, rotate('y', -90, init_plane));
    
    vector<GLfloat> test_left = {
        -a,   +a,   +a,
        -a,   -a,   +a,
        -a,   -a,   -a,
        -a,   +a,   -a
    };
    
    for (int i = 0; i < left.size(); i++) {
        if (left[i] != test_left[i]) {
            same = false;
            i = (int)left.size();
            
        }
    }
    
    if (same) std::cout << "\n left plane passed \n";
    
    result.push_back(left);
    
    vector<GLfloat> right = translate(a, 0.0, 0.0, rotate('y', +90, init_plane));
    
    vector<GLfloat> test_right = {
        +a,   +a,   -a,
        +a,   -a,   -a,
        +a,   -a,   +a,
        +a,   +a,   +a
    };

    for (int i = 0; i < right.size(); i++) {
        if (right[i] != test_right[i]) {
            same = false;
            i = (int)right.size();
        }
    }
    
    if (same) std::cout << "\n right plane passed \n";
    
    result.push_back(right);
    
    vector<GLfloat> top = translate(0.0, a, 0.0,rotate('x', +90, init_plane));
    
    vector<GLfloat> test_top = {
        +a,   +a,   +a,
        +a,   +a,   -a,
        -a,   +a,   -a,
        -a,   +a,   +a
    };
    
    for (int i = 0; i < top.size(); i++) {
        if ((int)top[i] != (int)test_top[i]) {
            same = false;
            std::cout << i << "\n";
            std::cout << top[i] << "\n";
        }
    }
    
    if (same) std::cout << "\n top plane passed \n";
    
    result.push_back(top);
    
    vector<GLfloat> bot =  translate(0.0, -a, 0.0, rotate('x', -90, init_plane));
    
    vector<GLfloat> test_bot = {
        +a,   -a,   -a,
        +a,   -a,   +a,
        -a,   -a,   +a,
        -a,   -a,   -a
    };
    
    for (int i = 0; i < bot.size(); i++) {
        if ((int)bot[i] != (int)test_bot[i]) {
            same = false;
            std::cout << i << "\n";
            std::cout << bot[i] << "\n";
        }
    }
    
    if (same) std::cout << "\n bottom plane passed \n";
    
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
    gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
    return nullptr;
}

// Construct the color mapping of the scene
GLfloat* init_color() {
    return nullptr;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat colors[] = {
        // Front plane
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        // Back plane
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        // Right
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        // Left
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        // Top
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        // Bottom
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
    };
    
//    // Perform display functions
    vector<GLfloat> cubeVerts = build_cube(2.0);

//    GLfloat* vertices = new GLfloat[6*4*3];
//    vertices = vector2array(cubeVerts);
    
    GLfloat* vertices = vector2array(cubeVerts);
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    vertices);  // Pointer to memory location to read from
    
    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   colors);     // Pointer to memory location to read from

    glDrawArrays(GL_QUADS, 0, 6*4); // six planes of 4 points each.

    delete[] vertices;
//
    glFlush();			//Finish rendering
    glutSwapBuffers();
}

//****************************************************************************************
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************

int main (int argc, char **argv) {

//    std::cout << "start" << " \n";
    
//    vector<GLfloat> verts = init_unit_plane();
//    int a = (int)verts.size();
////    std::cout << a <<"\n\n";
//    for (int i = 0; i < (int)verts.size(); i++ ){
//        if ( (i+1) % 3 == 0){
//            std::cout << verts[i] << "\n";
//        } else {
//            std::cout << verts[i] << " ";
//        }
//    }
    
    
//    vector<GLfloat> out = mat_mult(translate(0.0, 0.0, 1.0),  mat_mult(rotate('z', 90.0), to_homogenous_coord(verts)));
//    std::cout <<  (int)out.size() <<"\n\n";
//    for (int i = 0; i < (int)out.size(); i++ ){
//        if ( (i+1) % 4 == 0){
//            std::cout << out[i] << " \n";
//        } else {
//            std::cout << out[i] << " ";
//        }
//    }
    
//    vector<vector<GLfloat>> testvec = {verts, to_cartesian_coord(out)};
//    
//    GLfloat* vertices = new GLfloat[2*verts.size()];
//    
//    vertices = vector2array(testvec);
//    
//    std::cout << "\n\n here \n here" << " \n";
//
//    for (int i = 0; i < 2*(int)verts.size(); i++ ){
//        if ( (i+1) % 3 == 0){
//            std::cout << vertices[i] <<" \n";
//        } else {
//            std::cout << vertices[i] << " ";
//        }
//    }

//    std::cout << "\n\n here \n here" << " ";
//    
//    
//    
//    GLfloat* test = new GLfloat[16];
////    test = vector2array(out);
//    
//    std::cout << "\n\n end start" << " \n";
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;

    
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
    // Render our world
    glutMainLoop();
    
    
    return 0;
}

/*
Throw code here



 
 

 Returns a rotation matrix about the y-axis by theta degrees
 inputs: theta in degrees

vector<GLfloat> rotate_y (double theta) {
    theta = theta * 3.1415926536 / 180; //convert to radians
    GLfloat costheta = (GLfloat) cos(theta);
    GLfloat sintheta = (GLfloat) sin(theta);
    
    vector<GLfloat> rotate_mat_y = {
        costheta, 0.0, sintheta, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sintheta,  0.0, costheta, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    return rotate_mat_y;
}


 Returns a rotation matrix about the z-axis by theta degrees
 inputs: theta in degrees
 
vector<GLfloat> rotate_z (double theta) {
    theta = theta * 3.1415926536 / 180; //convert to radians
    GLfloat costheta = (GLfloat) cos(theta);
    GLfloat sintheta = (GLfloat) sin(theta);
    
    vector<GLfloat> rotate_mat_z = {
        costheta, -sintheta, 0.0, 0.0,
        sintheta, costheta, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    return rotate_mat_z;
}

 
 
 
 void init_camera() {
 // Camera parameters
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 }

 
 
 //    for (int i = 0; i < 24; i++) {
 //        int temp = (int)vertices[i];
 //        std::cout<< temp;
 //        delete temp;
 //    }
 
 
 //
 //    glColorPointer(3,           // 3 components (r, g, b)
 //                   GL_FLOAT,    // Vertex type is GL_FLOAT
 //                   0,           // Start position in referenced memory
 //                   colors);     // Pointer to memory location to read from
 //




 //    vector<GLfloat> verts = init_unit_plane();
 //    int a = (int)verts.size();
 //    std::cout << a <<"\n";
 //
 //    for (int i = 1; i < (int)verts.size() + 1; i++ ){
 //        if ( i % 3 == 0){
 //            std::cout << verts[i-1] <<" \n";
 //        } else {
 //            std::cout << verts[i-1] << " ";
 //        }
 //    }
 //
 //    GLfloat* arr = new GLfloat[4];
 //    arr[0] = 0.0;
 //    arr[1] = 1.0;
 //    arr[2] = 1.5;
 //    arr[3] = 2.5;
 //
 //    //int a = length(arr);
 //
 //    std::cout << arr << "\n";
 //
 //    delete[] arr;
 //
 //    vector<GLfloat> homs = to_homogenous_coord(verts);
 //
 //    int b = (int)homs.size();
 //    std::cout << b <<"\n\n";
 //
 //    for (int j = 1; j < (int)homs.size() + 1; j++ ){
 //        if ( j % 4 == 0){
 //            std::cout << homs[j-1] <<" \n";
 //        } else {
 //            std::cout << homs[j-1] << " ";
 //        }
 //    }
 //
 //
 //    std::cout << "\n ok \n\n";
 //
 //
 //    vector<GLfloat> same = mat_mult(ident(), homs);
 //
 //    int d = (int)same.size();
 //    std::cout << d <<"\n";
 //
 //    for (int l = 1; l < (int)same.size() + 1; l++ ){
 //        if ( l % 4 == 0){
 //            std::cout << same[l-1] <<" \n";
 //        } else {
 //            std::cout << same[l-1] << " ";
 //        }
 //    }
 //    
 //    std::cout << "\n\n";
 
 
 std::cout << "\n\nverticies " << " \n\n";
 
 for (int i = 0; i < 72; i++) {
 std::cout << "num " << i << " ";
 if ((i+1)%3 == 0) {
 std::cout << (int)vertices[i] << " \n";
 } else {
 std::cout << (int)vertices[i] << " ";
 }
 }
 

*/
