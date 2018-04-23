/***
 Assignment-3: Shading via Lighting and Colors
 
 Name: Board, Stephen
 
 Collaborators: Professor Alex Wong
 
 Project Summary: For this project, I had to simulate a lighting fixture being placed
 in the scene. Objects were to shine because of this light. This is for a class. I took
 way too long to get started and I'm finishing up the last two weeks of my senior year so
 I have  alot of other obligations cropping up.. Then I spent too much time obsessively 
 trying to make a nice icosehedron.  I looked some stuff up on wiki but a lot of the 
 result is just from checking how it turned out and adjusting accordingly.  It turned out
 a little sloppy.  
 
 Results: The lighting is mostly simulated.  There is definitely a lighter hue that persists
 in the direction of the supposed lighting fixture.  However, the more vertical sides of the
 icosehedron are too dark for some reason.  I tried moving the light around: the results were
 not as expected.  a
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
#include "Assignment3Header.h"
#include <cassert>
#include <stdlib.h>
using namespace std;
vector<GLfloat> point_Vector;
vector<GLfloat> myScene;
vector<GLfloat> myColors;
vector<GLfloat> shaded_Colors;
vector<GLfloat> light;
vector<GLfloat> camera;
vector<GLfloat> normals;
GLfloat* vertices;
GLfloat* paint;
int num_Points;
float theta = 0.0;


/**************************************************
 *              Object Model Class                *
 *                                                *
 *  Stores the points of the object as a vector   *
 *  along with the colors and normals for each    *
 *  point. Normals are computed from the points.  *
 *                                                *
 *************************************************/
class ObjectModel {
    vector<GLfloat> _points;
    vector<GLfloat> _normals;
    vector<GLfloat> _base_colors;
    vector<GLfloat> _colors;
public:
    ObjectModel() { };
    vector<GLfloat> get_points() { return _points; };
    vector<GLfloat> get_normals() { return _normals; };
    vector<GLfloat> get_base_colors() { return _base_colors; };
    vector<GLfloat> get_colors() { return _colors; };
    void set_points(vector<GLfloat> points) { _points = points; };
    void set_normals(vector<GLfloat> normals) { _normals = normals; };
    void set_base_colors(vector<GLfloat> base_colors) { _base_colors = base_colors; };
    void set_colors(vector<GLfloat> colors) { _colors = colors; };
};


/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  Using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene.*
 *                                                *
 *************************************************/


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
 Returns the homogeneous identity matrix for testing
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
    result = scale(0.7, 0.7, 0.7, result);
    return result;
}
/*

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
*/

// Builds a very sloppy icosehedon
vector<GLfloat> build_ball()
{
    GLfloat a = 0.1;
    GLfloat h = 4.472*a;
    
//    instantiate the equilatoral triangle
    vector<GLfloat> init_tri = {
        -2*a, 0.0, 0.0,
        +2*a, 0.0, 0.0,
        0.0, h, 0.0
    };
    
    vector<vector<GLfloat>> bottomSides;
    GLfloat dist = 0.755761* (4*a);
    vector<GLfloat> one = translate(0.0, -0.0, -dist, rotate('x',-10,init_tri));
    bottomSides.push_back(one);
    vector<GLfloat> two = translate(0.0, 0.0, -dist, rotate('x',138,init_tri));
    bottomSides.push_back(two);
    vector<GLfloat> three = rotate('y',72,one);
    bottomSides.push_back(three);
    vector<GLfloat> four = rotate('y',72,two);
    bottomSides.push_back(four);
    vector<GLfloat> five = rotate('y',144,one);
    bottomSides.push_back(five);
    vector<GLfloat> six = rotate('y',144,two);
    bottomSides.push_back(six);
    vector<GLfloat> seven = rotate('y',216,one);
    bottomSides.push_back(seven);
    vector<GLfloat> eight = rotate('y',216,two);
    bottomSides.push_back(eight);
    vector<GLfloat> nine = rotate('y',288,one);
    bottomSides.push_back(nine);
    vector<GLfloat> ten = rotate('y',288,two);
    bottomSides.push_back(ten);

    vector<GLfloat> ball;
    for (int i = 0; i < bottomSides.size(); i++) {
        vector<GLfloat> curVec = translate(0.0,-0.22,0.0,bottomSides[i]);
        for (int j = 0; j < curVec.size(); j++) {
            ball.push_back(curVec[j]);
        }
    }
    vector<vector<GLfloat>> topSides;
    for (int i = 0; i < bottomSides.size(); i++)
    {
        topSides.push_back(translate(0.0,0.22,0.0,rotate('y',36,rotate('z',180,bottomSides[i]))));
    }
    for (int i = 0; i < topSides.size(); i++) {
        vector<GLfloat> curVec = topSides[i];
        for (int j = 0; j < curVec.size(); j++) {
            ball.push_back(curVec[j]);
        }
    }
    return ball;
    
    
}
/**************************************************
 *           Generating Surface Normals           *
 *                                                *
 *  Generate the surface normals of the objects   *
 *  using the cross product between two vectors   *
 *  that lie on the surface (plane) of interest.  *
 *  Recall that the direction of the normal to a  *
 *  surface follows the Right Hand Rule.          *
 *                                                *
 *************************************************/

vector<GLfloat> subtract(vector<GLfloat> b,vector<GLfloat> a)
{
    vector<GLfloat> subtraction =
    {
        b[0]-a[0], b[1]-a[1], b[2]-a[2]
    };
    return subtraction;
}

vector<GLfloat> vector_add(vector<GLfloat> a,vector<GLfloat> b)
{
    vector<GLfloat> addition =
    {
        a[0]+b[0], a[1]+b[1], a[2]+b[2]
    };
    return addition;
}

vector<GLfloat> normalize(vector<GLfloat> v)
{
    vector<GLfloat> normie;
    
    for(int i = 0; i < v.size(); i+=3)
    {
        GLfloat x2 = v[i]*v[i];
        GLfloat y2 = v[i+1]*v[i+1];
        GLfloat z2 = v[i+2]*v[i+2];
        
        double sum = x2+y2+z2;
        
        GLfloat mag = sqrt(sum);
        
        //        normalize
        normie.push_back(v[i]/mag);
        normie.push_back(v[i+1]/mag);
        normie.push_back(v[i+2]/mag);
    }
    
    return normie;
}

// Generates the normals to each surface (plane)
vector<GLfloat> generate_normals(vector<GLfloat> points)
{
    vector<GLfloat> normals;
    
    vector<GLfloat> centroids;
    //   get centroids
    for (int i = 0; i < points.size(); i=i+9)
    {
        centroids.push_back((points[i]+points[i+3]+points[i+6])/3);
        centroids.push_back((points[i+1]+points[i+4]+points[i+7])/3);
        centroids.push_back((points[i+2]+points[i+5]+points[i+8])/3);
    }
    
    // iterate through triangles
    // three 3D points per triangle
    for (int i = 0; i < points.size(); i=i+9)
    {
        // define subpoints a,b,c of each triangle
        vector<GLfloat> a = {
         points[i], points[i+1], points[i+2]
        };
        vector<GLfloat> b = {
         points[i+3], points[i+4], points[i+5]
        };
        vector<GLfloat> c = {
            points[i+6], points[i+7], points[i+8]
        };

        // obtain bottom and left vectors of the triangle
        vector<GLfloat> A = subtract(b,a);
        vector<GLfloat> B = subtract(c,a);
        
        // compute the cross product
        normals.push_back(A[1]*B[2]-A[2]*B[1]);
        normals.push_back(A[2]*B[0]-A[0]*B[2]);
        normals.push_back(A[0]*B[1]-A[1]*B[0]);
        
    }
    
//    iterate through each normal vector
    for(int i = 0; i < normals.size(); i=i+3)
    {
//        squere each component
        GLfloat x2 = normals[i]*normals[i];
        GLfloat y2 = normals[i+1]*normals[i+1];
        GLfloat z2 = normals[i+2]*normals[i+2];
        
        double sum = x2+y2+z2;
        
        GLfloat mag = sqrt(sum);
        
//        normalize
        normals[i]=normals[i]/mag;
        normals[i+1]=normals[i+1]/mag;
        normals[i+2]=normals[i+2]/mag;
    }
    
//    originage each normal from the centroid of each triangle
    for (int i = 0; i < normals.size(); i= i+3)
    {
        vector<GLfloat> a = {
            normals[i],normals[i+1],normals[i+2]
        };
        vector<GLfloat> b = {
            centroids[i],centroids[i+1],centroids[i+2]
        };
        vector<GLfloat> sum = vector_add(a,b);
        normals[i] = sum[0];
        normals[i+1] = sum[1];
        normals[i+2] = sum[2];
    }

    return normals;
}

void spin(double theta) {
    myScene = translate(0.2, -0.0, 0.3, rotate('y',theta,translate(-0.2, 0.0, -0.3, myScene)));
    vertices = vector2array(myScene);
    normals = generate_normals(myScene);
    shaded_Colors = apply_shading(myColors, light,  camera, normals);
    paint = vector2array(shaded_Colors);
}


/**************************************************
 *         Shading via Lighting and Color         *
 *                                                *
 *  Generate the set of colors for each face of   *
 *  the planes that compose the objects in the    *
 *  scene. Based on the light source and surface  *
 *  normals, render the colors of the objects by  *
 *  applying the shading equation.                *
 *                                                *
 *************************************************/



// Initializes the base color of a plane to a single color
vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
    vector<GLfloat> base_color = {
        r,   g,   b,
        r,   g,   b,
        r,   g,   b
    };
    return base_color;
}


// Initializes the base color of a plane by specifying the color of each point
vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
                                GLfloat r2, GLfloat g2, GLfloat b2) {
    // This enables OpenGL to use interpolation for (Gouraud) shading the plane
    vector<GLfloat> base_color = {
        r0,   g0,   b0,
        r1,   g1,   b1,
        r2,   g2,   b2,
    };
    return base_color;
}

// Generates the colors of a set of surfaces based on the light source,
// surface normals, and base color of the surface
vector<GLfloat> apply_shading(vector<GLfloat> flat_colors, vector<GLfloat> light_source, vector<GLfloat> camera, vector<GLfloat> normals) {
    vector<GLfloat> v = subtract(camera, {0.0,0.0,0.0});
    vector<GLfloat> colors;

    vector<GLfloat> h = normalize(vector_add(v, light_source));
    
    for(int i = 0; i < normals.size();i+=3)
    {
        vector<GLfloat> n = {normals[i],normals[i+1],normals[i+2]};
        GLfloat specular = dot_prod(n,h);
        GLfloat diffuse = dot_prod(n,light_source);
        if (diffuse < 0) diffuse = 0;
        for(int j = 0; j < 9; j++)
        {
            GLfloat Ic = (flat_colors[i+j]*0.75+0.2*diffuse)+(0.25*flat_colors[i+j]*specular);
            colors.push_back(Ic);
        }
    }
    
    return colors;
}

// Allows for ambience (a vector of 3 values), diffusion (vector of 3 x n values) and specular (vector of 3 x n values)
// as input to the shading equation
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera,
                          vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec) {
    vector<GLfloat> colors;
    
    object_model.set_colors(colors);
    return object_model;
}

// Performs the dot product between two vectors
GLfloat dot_prod(vector<GLfloat> A, vector<GLfloat> B) {
    return (A[0]*B[0])+(A[1]*B[1])+(A[2]*B[2]);
}

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  Create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by using setting *
 *  the projection viewing matrices               *
 *                                                *
 *************************************************/

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
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    spin(1); // spin the shape
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, paint);
    glDrawArrays(GL_TRIANGLES, 0, num_Points);
    glFlush();			//Finish rendering
    glutSwapBuffers();
}
void idle_func() {
    theta = theta+1;
    display_func();
}

int main (int argc, char **argv)
{
    myScene =translate(0.2,0.0,0.3,scale(0.4,0.4,0.4,build_ball()));
    /*vector<GLfloat> addition_1 = translate(-0.4,-0.2,-0.3,scale(0.2,0.2,0.2,build_diamond()));
    for(int i = 0; i < addition_1.size(); i++)
    {
        myScene.push_back(addition_1[i]);       // commented out on purpose for now.
    }*/
    vector<vector<GLfloat>> base_Colors;
    // blues for every surface
    for(int i = 0; i < myScene.size(); i+=9)
    {
        base_Colors.push_back(init_base_color(0.5, 0.5, 1.0));
    }
    for (int i = 0; i < base_Colors.size(); i++) {
        vector<GLfloat> curVec = base_Colors[i];
        for (int j = 0; j < curVec.size(); j++) {
            myColors.push_back(curVec[j]);
        }
    }
    /////////////////////////////////////////////////////////
    // Directional light vector//////////////////////////////
    light = subtract({1.0,1.0,-0.5}, {0.0,0.0,-0.2});////////
    /////////////////////////////////////////////////////////
    camera = {0.0,0.0,3.0};//////////////////////////////////
    /////////////////////////////////////////////////////////
    
    
    normals = generate_normals(myScene);
    shaded_Colors = apply_shading(myColors, light, camera, normals);
    num_Points = (int) myScene.size()/3;
    vertices = vector2array(myScene);
    paint = vector2array(shaded_Colors);
    std::cout<<"\n";
    std::cout<<myColors.size()<<"\n";
    std::cout<<myScene.size()<<"\n";
    std::cout<<normals.size()<<"\n";
    std::cout<<shaded_Colors.size()<<"\n";
    
    /*
    point_Vector = build_ball();
    
    num_Points = (int)point_Vector.size()/3;
    
    vector<GLfloat> Color_Vec = init_base_color(0.7, 0.3, 1.0);
    
    paint = vector2array(Color_Vec);
    
    vertices = vector2array(point_Vector);
    
    vector<GLfloat> norms = generate_normals(point_Vector);
     */
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 3");

    setup();
    init_camera();
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}


//Throwing unneeded code here

//// Initializes the base color of a plane by specifying the color of each point
//vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
//                                GLfloat r2, GLfloat g2, GLfloat b2, GLfloat r3, GLfloat g3, GLfloat b3) {
//    // This enables OpenGL to use interpolation for (Gouraud) shading the plane
//    vector<GLfloat> base_color = {
//        r0,   g0,   b0,
//        r1,   g1,   b1,
//        r2,   g2,   b2,
//        r3,   g3,   b3
//    };
//    return base_color;
//}

//// Initializes the base color of a plane to a single color
//vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
//    vector<GLfloat> base_color = {
//        r,   g,   b,
//        r,   g,   b,
//        r,   g,   b,
//        r,   g,   b
//    };
//    return base_color;
//}


//// Performs the cross product between two vectors
//vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
//    vector<GLfloat> C;
//    
//    return C;
//}

//    std::cout<<"\nprinting normal\n";
//
//    for(int i = 1; i <= normals.size(); i++)
//    {
//        if(i%3 == 0)
//        {
//            std::cout<<normals[i-1]<<"\n";
//        } else {
//            std::cout<<normals[i-1]<<"\n";
//        }
//    }
//    std::cout<<"\ndone\n";
