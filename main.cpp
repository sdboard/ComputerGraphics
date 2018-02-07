
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
        
//        Iterate through our control points
        for (int p = 0; p < (control_points.size()-1); p++) {
            
            Coordinate v0 = control_points[p];
            
            Coordinate v1 = control_points[p+1];
            
//            Find the Midpoint
            GLfloat newX = (v0.x + v1.x)/2;
            GLfloat newY = (v0.y + v1.y)/2;
            
            points.push_back(Coordinate(newX, newY));
            
        }
        
        vector<Coordinate> toReturn = {control_points[0]};
        
        vector<Coordinate> recurse = generate_points(points); // RECURSE
        
        for (int i = 0; i < recurse.size(); i++) {
            
            toReturn.push_back((recurse[i]));
        
        }
        
//        Add the end point
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
    Coordinate c3 = Coordinate(-0.5720f,    0.0620f);
//    Coordinate c4 = Coordinate(-0.5100f,    0.0280f);
    Coordinate c2 = Coordinate(-0.5280f,    0.0340f);
    vector<Coordinate> points2 = {a1, b2, b3, c3, c2};//, c2};
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
    Coordinate cmu0 = Coordinate(-0.048f,-0.362f);
//    Coordinate cmu1 = Coordinate(-0.228f,-0.38f);
    Coordinate cmu1 = Coordinate(-0.228f,-0.4f);
    Coordinate cmu2 = Coordinate(-0.396f,-0.464f);
    Coordinate cmu3 = Coordinate(-0.472f,-0.436f);
    vector<Coordinate> pointsmu= {cmu0, cmu1, cmu2, cmu3};
    vector<Coordinate> newPointsmu = recursive_call(pointsmu);
    
//    mouthLower
//    Coordinate cl0 = Coordinate(-0.04f,-0.416f);
    Coordinate cl0 = Coordinate(-0.048f,-0.362f);
    Coordinate cl1 = Coordinate(-0.18f,-0.44f);
    Coordinate cl2 = Coordinate(-0.332f,-0.496f);
    Coordinate cl3 = Coordinate(-0.428f,-0.488f);
//    Coordinate cl4 = Coordinate(-0.436f,-0.456f);
    Coordinate cl4 = Coordinate(-0.472f,-0.436f);
    vector<Coordinate> pointsl= {cl0, cl1, cl2, cl3, cl4};
    vector<Coordinate> newPointsl = recursive_call(pointsl);
    
//    head left
    Coordinate ch0 = Coordinate(-0.292f,-0.796f);
    Coordinate ch1 = Coordinate(-0.492f,-0.784f);
    Coordinate ch2 = Coordinate(-0.62f,-0.484f);
    Coordinate ch3 = Coordinate(-0.796f,-0.24f);
    Coordinate ch4 = Coordinate(-0.628f,-0.028f);
    Coordinate ch5 = Coordinate(-0.892f,0.208f);
    Coordinate ch6 = Coordinate(-0.76f,0.404f);
    Coordinate ch7 = Coordinate(-0.656f,0.772f);
    Coordinate ch8 = Coordinate(-0.496f,0.932f);
    Coordinate ch9 = Coordinate(-0.024f,0.992f);
    vector<Coordinate> pointsh= {ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9};
    vector<Coordinate> newPointsh = recursive_call(pointsh);
    
//    head top right
    Coordinate chr0 = Coordinate(-0.048f,0.988f);
    Coordinate chr1 = Coordinate(0.14f,1.004f);
    Coordinate chr2 = Coordinate(0.388f,0.98f);
    Coordinate chr3 = Coordinate(0.548f,0.888f);
    Coordinate chr4 = Coordinate(0.664f,0.712f);
    Coordinate chr5 = Coordinate(0.698f,0.544f);
    Coordinate chr6 = Coordinate(0.676f,0.28f);
    vector<Coordinate> pointshr= {chr0, chr1, chr2, chr3, chr4, chr5, chr6};
    vector<Coordinate> newPointshr = recursive_call(pointshr);
    
//    ear
    Coordinate cear0 = Coordinate(0.58f,0.236f);
    Coordinate cear1 = Coordinate(0.68f,0.324f);
    Coordinate cear2 = Coordinate(0.828f,0.296f);
    Coordinate cear3 = Coordinate(0.884f,0.148f);
    Coordinate cear4 = Coordinate(0.792f,-0.088f);
    Coordinate cear5 = Coordinate(0.58f,-0.244f);
    Coordinate cear6 = Coordinate(0.492f,-0.212f);
    vector<Coordinate> pointsear= {cear0, cear1, cear2, cear3, cear4, cear5, cear6};
    vector<Coordinate> newPointsear = recursive_call(pointsear);
    

//    glasses right
    Coordinate cglr0 = Coordinate(-0.396f,0.016f);
    Coordinate cglr1 = Coordinate(-0.376f,0.152f);
    Coordinate cglr2 = Coordinate(-0.268f,0.236f);
    Coordinate cglr3 = Coordinate(-0.124f,0.192f);
    Coordinate cglr4 = Coordinate(-0.048f,0.092f);
    Coordinate cglr5 = Coordinate(-0.052f,-0.096f);
    Coordinate cglr6 = Coordinate(-0.128f,-0.176f);
    Coordinate cglr7 = Coordinate(-0.332f,-0.136f);
    Coordinate cglr8 = Coordinate(-0.392f,0.008f);
    vector<Coordinate> pointsglr= {cglr0, cglr1, cglr2, cglr3, cglr4, cglr5, cglr6, cglr7, cglr8};
    vector<Coordinate> newPointsglr = recursive_call(pointsglr);

//    lglass
    Coordinate clg0 = Coordinate(-0.508f,-0.02f);
    Coordinate clg1 = Coordinate(-0.608f,0.14f);
    Coordinate clg2 = Coordinate(-0.772f,0.152f);
    Coordinate clg3 = Coordinate(-0.792f,0.064f);
    Coordinate clg4 = Coordinate(-0.792f,-0.06f);
    Coordinate clg5 = Coordinate(-0.792f,-0.124f);
    Coordinate clg6 = Coordinate(-0.652f,-0.232f);
    Coordinate clg7 = Coordinate(-0.512f,-0.22f);
    Coordinate clg8 = Coordinate(-0.504f,-0.02f);
    vector<Coordinate> pointslg= {clg0, clg1, clg2, clg3, clg4, clg5, clg6, clg7, clg8};
    vector<Coordinate> newPointslg = recursive_call(pointslg);
  
//    glasses connector
    Coordinate cgs0 = Coordinate(-0.524f,-0.008f);
    Coordinate cgs1 = Coordinate(-0.44f,0.024f);
    Coordinate cgs2 = Coordinate(-0.388f,0.004f);
    vector<Coordinate> pointsgs= {cgs0, cgs1, cgs2};
    vector<Coordinate> newPointsgs = recursive_call(pointsgs);
    
//    glasses strap
    Coordinate css0 = Coordinate(-0.156f,0.032f);
    Coordinate css1 = Coordinate(0.348f,0.168f);
    Coordinate css2 = Coordinate(0.676f,0.276f);
    vector<Coordinate> pointsss= {css0, css1, css2};
    vector<Coordinate> newPointsss = recursive_call(pointsss);


//    cheek
    Coordinate cch0 = Coordinate(-0.24f,-0.156f);
    Coordinate cch1 = Coordinate(-0.16f,-0.252f);
    Coordinate cch2 = Coordinate(0.016f,-0.288f);
    vector<Coordinate> pointsch= {cch0, cch1, cch2};
    vector<Coordinate> newPointsch = recursive_call(pointsch);

//    jaw line
    Coordinate cjl0 = Coordinate(-0.296f,-0.796f);
    Coordinate cjl1 = Coordinate(-0.096f,-0.808f);
    Coordinate cjl2 = Coordinate(0.192f,-0.68f);
    Coordinate cjl3 = Coordinate(0.468f,-0.424f);
    Coordinate cjl4 = Coordinate(0.508f,-0.216f);
    vector<Coordinate> pointsjl= {cjl0, cjl1, cjl2, cjl3, cjl4};//, cjl5, cjl6, cjl7};
    vector<Coordinate> newPointsjl = recursive_call(pointsjl);
    
//    eye wrinkle 1
    Coordinate cwr10 = Coordinate(-0.08f,0.108f);
    Coordinate cwr11 = Coordinate(0.0f,0.064f);
    Coordinate cwr12 = Coordinate(0.084f,0.1f);
    vector<Coordinate> pointswr1= {cwr10, cwr11, cwr12};
    vector<Coordinate> newPointswr1 = recursive_call(pointswr1);
    
//    eye wrinkle 2
    Coordinate cwr20 = Coordinate(-0.136f,0.004f);
    Coordinate cwr21 = Coordinate(-0.016f,0.028f);
    Coordinate cwr22 = Coordinate(0.068f,2.2737e-16f);
    vector<Coordinate> pointswr2= {cwr20, cwr21, cwr22};
    vector<Coordinate> newPointswr2 = recursive_call(pointswr2);
    
//    inner ear
    Coordinate cie0 = Coordinate(0.556f,0.084f);
    Coordinate cie1 = Coordinate(0.62f,0.028f);
    Coordinate cie2 = Coordinate(0.546f,-0.008f);
    vector<Coordinate> pointsie= {cie0, cie1, cie2};
    vector<Coordinate> newPointsie = recursive_call(pointsie);
    
//    pupil right
    Coordinate cpr0 = Coordinate(-0.238f,0.09f);
    Coordinate cpr1 = Coordinate(-0.262f,0.06f);
    Coordinate cpr2 = Coordinate(-0.226f,0.044f);
    vector<Coordinate> pointspr= {cpr0, cpr1, cpr2};
    vector<Coordinate> newPointspr = recursive_call(pointspr);
    
//    pupil left
    Coordinate cpl0 = Coordinate(-0.632f,0.056f);
    Coordinate cpl1 = Coordinate(-0.662f,0.028f);
    Coordinate cpl2 = Coordinate(-0.626f,0.004f);
    vector<Coordinate> pointspl= {cpl0, cpl1, cpl2};
    vector<Coordinate> newPointspl = recursive_call(pointspl);
    
//    List of all the point vectors
    vector<vector<Coordinate>> toTour = {newPointsl, newPointsmu, newPointsn, newPoints2, newPoints3, newPoints4, newPoints5, newPointsh, newPointshr, newPointsear, newPointsglr, newPointslg, newPointsgs, newPointsss, newPointsjl, newPointsch, newPointswr1, newPointswr2, newPointsie, newPointspr, newPointspl};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0.0f, 0.0f, 0.0f); //set our color to BLACK
    
    glBegin(GL_LINES);
    
    
//    Draw all the lines
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
