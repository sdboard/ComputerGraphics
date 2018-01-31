//
//  main.cpp
//  OpenGL_test
//
//  Created by Stephen Board on 1/10/18.
//  Copyright © 2018 Stephen Board. All rights reserved.
//  not really

#include <vector>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "Chaikins.hpp"


using namespace std;

//struct Coordinate{
//    GLfloat x, y;
//    Coordinate(GLfloat x_, GLfloat y_) : x(x_), y(y_) {}
//};

Coordinate a = Coordinate(-0.4f, 0.0f);
Coordinate b = Coordinate(0.0f, 0.6f);
Coordinate c = Coordinate(0.4f, 0.0f);

vector<Coordinate> points = {a, b, c};

//Vertex::Vertex(GLfloat X, GLfloat Y) {
//    x = X;
//    y = Y;
//}


vector<Coordinate> generate_points(vector<Coordinate> control_points){
 
    vector<Coordinate> points;
    
    if (control_points.size() == 1) {
        
        return control_points;
        
    } else {
        
        // Iterate through our control points
        for (int p = 0; p < control_points.size(); p++) {
            
            Coordinate v0 = control_points[p];
            
            Coordinate v1 = control_points[p+1];
            
            // Find the Midpoint
            GLfloat newX = (v0.x + v1.x)/2;
            GLfloat newY = (v0.y + v1.y)/2;
            
            
            points.push_back(Coordinate(newX, newY));
            
        }
        
        vector<Coordinate> toReturn = {control_points[0]};
        
        vector<Coordinate> recurse = generate_points(points);
        
        for(int i = 0; i < recurse.size(); i++) {
            toReturn.push_back((recurse[i]));
        }
        
        toReturn.push_back(control_points[control_points.size()]);
        
        return toReturn;
    }
}

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // clears the screen
}




void display(){
    //where we do all the drawings
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0.0f, 0.0f, 0.0f); //set our color to BLACK
    
    vector<Vertex> v;
    
    glBegin(GL_LINES);
    
    glVertex2f(-0.4f, 0.0f);
    glVertex2f(0.0f, 0.6f);
    
    glVertex2f(0.0f, 0.6f);
    glVertex2f(0.4f, 0.0f);

    glEnd();
    glutSwapBuffers();
}


int main(int argc, char * argv[]) {
    // insert code here...
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutCreateWindow("My Window");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    GLfloat x=0.6, y=0.4;
    Coordinate P(x,y);
    std::cout << P.x<<"\n";
    
    
    
    return 0;
}
