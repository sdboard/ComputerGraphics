
#include <vector>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "cartoonify.hpp"

using namespace std;

vector<Coordinate> generate_points(vector<Coordinate> control_points){
    
    vector<Coordinate> points;
    
    if (control_points.size() == 1) {
        
        return control_points;
        
    } else {
        
        // Iterate through our control points
        for (int p = 0; p < (control_points.size()-1); p++) {
            
            Coordinate v0 = control_points[p];
            
            Coordinate v1 = control_points[p+1];
            
            // Find the Midpoint
            GLfloat newX = (v0.x + v1.x)/2;
            GLfloat newY = (v0.y + v1.y)/2;
            
            points.push_back(Coordinate(newX, newY));
            
        }
        
        vector<Coordinate> toReturn = {control_points[0]};
        
        vector<Coordinate> recurse = generate_points(points);
        
        for (int i = 0; i < recurse.size(); i++) {
            
            toReturn.push_back((recurse[i]));
        
        }
        
        toReturn.push_back(control_points[control_points.size()-1]);
        
        return toReturn;
    }
}

vector<Coordinate> recursive_call(vector<Coordinate> control_points){
    vector<Coordinate> toReturn = generate_points(control_points);
    for (int i = 0; i < 3; i ++){
        toReturn = generate_points(toReturn);
    }
    
    return toReturn;
}

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // clears the screen
}

void display(){
//    drawings

//    nose
    Coordinate cn0 = Coordinate(-0.304f,-0.16f);
    Coordinate cn1 = Coordinate(-0.228f,-0.188f);
    Coordinate cn2 = Coordinate(-0.244f,-0.276f);
    Coordinate cn3 = Coordinate(-0.348f,-0.328f);
    Coordinate cn4 = Coordinate(-0.484f,-0.404f);
    Coordinate cn5 = Coordinate(-0.588f,-0.412f);
    Coordinate cn6 = Coordinate(-0.608f,-0.292f);
    Coordinate cn7 = Coordinate(-0.52f,-0.016f);
    vector<Coordinate> pointsn = {cn0, cn1, cn2, cn3, cn4, cn5, cn6, cn7};
    vector<Coordinate> newPointsn = recursive_call(pointsn);
    
//    left eye top
    Coordinate a1 = Coordinate(-0.6880f,    0.0280f);
    Coordinate b2 = Coordinate(-0.6640f,    0.0600f);
    Coordinate b3 = Coordinate(-0.6280f,    0.0720f);
    Coordinate c3 = Coordinate(-0.5720f,    0.0520f);
    Coordinate c2 = Coordinate(-0.5120f,    0.0440f);
    vector<Coordinate> points2 = {a1, b2, b3, c3, c2};
    vector<Coordinate> newPoints2 = recursive_call(points2);
    
//    left eye bot
    Coordinate a2 = Coordinate(-0.6960f,    0.0200f);
    Coordinate a3 = Coordinate(-0.6280f,   -0.0080f);
    Coordinate a4 = Coordinate(-0.5840f,   -0.0040f);
    Coordinate a5 = Coordinate(-0.5400f,    0.0280f);
    vector<Coordinate> points3 = {a2, a3, a4, a5};
    vector<Coordinate> newPoints3= recursive_call(points3);
    
//    right eye top
    Coordinate b1 = Coordinate(-0.3200f,    0.0440f);
    Coordinate b4 = Coordinate(-0.2560f,    0.1160f);
    Coordinate b5 = Coordinate(-0.1320f,    0.1200f);
    Coordinate b6 = Coordinate(-0.0560f,    0.0840f);
    vector<Coordinate> points4 = {b1, b4, b5, b6};
    vector<Coordinate> newPoints4 = recursive_call(points4);
    
//    right eye bottom
    Coordinate e1 = Coordinate(-0.2920f,    0.0360f);
    Coordinate e2 = Coordinate(-0.2160f,    0.0360f);
    Coordinate e3 = Coordinate(-0.1320f,    0.0520f);
    Coordinate e4 = Coordinate(-0.0800f,    0.0760f);
    vector<Coordinate> points5 = {e1, e2, e3, e4};
    vector<Coordinate> newPoints5 = recursive_call(points5);

//    mouthUpper
    Coordinate cm0 = Coordinate(-0.044f,-0.4f);
    Coordinate cm1 = Coordinate(-0.18f,-0.384f);
    Coordinate cm2 = Coordinate(-0.332f,-0.428f);
    Coordinate cm3 = Coordinate(-0.456f,-0.436f);
    Coordinate cm4 = Coordinate(-0.5f,-0.412f);
    vector<Coordinate> pointsm= {cm0,cm1,cm2, cm3, cm4};
    vector<Coordinate> newPointsm = recursive_call(pointsm);
    
//    mouthLower
    Coordinate cl0 = Coordinate(-0.04f,-0.416f);
    Coordinate cl1 = Coordinate(-0.18f,-0.44f);
    Coordinate cl2 = Coordinate(-0.332f,-0.496f);
    Coordinate cl3 = Coordinate(-0.428f,-0.488f);
    Coordinate cl4 = Coordinate(-0.436f,-0.456f);
    vector<Coordinate> pointsl= {cl0, cl1, cl2, cl3, cl4};
    vector<Coordinate> newPointsl = recursive_call(pointsl);
    
    vector<vector<Coordinate>> toTour = {newPointsl, newPointsm, newPointsn, newPoints2, newPoints3, newPoints4, newPoints5};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0.0f, 0.0f, 0.0f); //set our color to BLACK
    
    glBegin(GL_LINES);
    
    
    for (int i =0; i < toTour.size(); i++) {
        
        vector<Coordinate> currVec = toTour[i];
        
        for (int i = 0; i < currVec.size()-1; i++) {
            
            glVertex2f(currVec[i].x, currVec[i].y);
            
            glVertex2f(currVec[i+1].x, currVec[i+1].y);
            
        }
        
    }

    
    glEnd();
    glutSwapBuffers();
    
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutCreateWindow("My Window");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
