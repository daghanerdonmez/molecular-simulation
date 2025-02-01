//
//  geometry2d.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#include "geometry2d.hpp"

std::tuple<double, double> lineOfTwoPoints(glm::dvec2 point1, glm::dvec2 point2)
{
    // returns the line passing from two points in y = mx + b format.
    // not handling the verticle line case here
    // let's hope it never happens :(
    
    double m = (point2.y - point1.y) / (point2.x - point1.x);
    double b = point1.y - m * point1.x;

    return std::tuple<double,double>(m,b);
}

std::variant<std::monostate, glm::dvec2, std::tuple<glm::dvec2, glm::dvec2>>
intersectionOfLineAndCircle(double m, double b, double x0, double y0, double r)
{
    // returns the intersection point(s) of a line and a circle
    // Input: m,b -> y = mx + b
    //        x,y,r -> (x-x0)^2 + (y-y0)^2 = r^2
    // Output: monostate if no intersection
    //         vec3 if 1 intersection
    //         tuple of vec3 if 2 intersections
    
    double A = (m*m + 1);
    double B = 2 * (m*b - m*y0 - x0);
    double C = (y0*y0 - r*r + x0*x0 - 2*y0*b + b*b);
    
    double determinant = sqrt(B*B - 4*A*C);
    
    bool noIntersection = determinant < 0;
    bool oneIntersection = determinant == 0;
    bool twoIntersections = determinant > 0;

    if (noIntersection) {
        //std::cout << "buraya girdim1 \n";
        return std::monostate{};
    } else if (oneIntersection) {
        //std::cout << "buraya girdim2 \n";
        double x = (-B/(2*A));
        double y = m*x + b;
        return glm::dvec2(x,y);
    } else if (twoIntersections) {
        //std::cout << "buraya girdim3 \n";
        //std::cout << m << std::endl << b << std::endl << x0 << std::endl << y0 << std::endl << r << std::endl;
        double x1 = (-B + determinant)/(2*A);
        double x2 = (-B - determinant)/(2*A);
        double y1 = m*x1 + b;
        double y2 = m*x2 + b;
        
        return std::tuple<glm::dvec2, glm::dvec2>(glm::dvec2(x1,y1), glm::dvec2(x2,y2));
    }
    
    return std::monostate{}; // Default case (shouldn't be reached)
}

glm::dvec2 reflectionAboutPoint(glm::dvec2 reflectedPoint, glm::dvec2 pivotPoint)
{
    glm::dvec2 reflection = 2.0 * pivotPoint - reflectedPoint;
    return reflection;
}

glm::dvec2 reflectionAboutLine(glm::dvec2 reflectedPoint, double m, double b)
{
    double x1 = reflectedPoint.x;
    double y1 = reflectedPoint.y;

    double denom = 1 + m * m;
    double q1 = (x1 + m * y1 - m * b) / denom;
    double q2 = (m * (x1 + m * y1) + b) / denom;

    double xPrime = 2 * q1 - x1;
    double yPrime = 2 * q2 - y1;

    return glm::dvec2(xPrime, yPrime);
}

bool pointBetweenTwoPoints(glm::dvec2 checkedPoint, glm::dvec2 point1, glm::dvec2 point2)
{
    // This function checks if the given point is "between" the other two points
    // i.e. Checks if the point is inside the rectangle drawn by the other two
    
    // Check if checkedPoint's x is between point1.x and point2.x
   bool withinX = (checkedPoint.x >= std::min(point1.x, point2.x)) && (checkedPoint.x <= std::max(point1.x, point2.x));

   // Check if checkedPoint's y is between point1.y and point2.y
   bool withinY = (checkedPoint.y >= std::min(point1.y, point2.y)) && (checkedPoint.y <= std::max(point1.y, point2.y));

   return withinX && withinY;
}


glm::dvec2 reflectingOffCircle(glm::dvec2 oldPosition, glm::dvec2 newPosition, glm::dvec2 circleCenter, double cirleRadius)
{
    std::tuple<double, double> lineOfMovement = lineOfTwoPoints(oldPosition, newPosition);
    // Accessing tuple elements using std::get<>
    double m = std::get<0>(lineOfMovement);
    double b = std::get<1>(lineOfMovement);
    
    auto intersectionPoint = intersectionOfLineAndCircle(m, b, circleCenter.x, circleCenter.y, cirleRadius);
    
    // I make sure that there are two intersections (This should always be the case for my purposes)
    if (std::holds_alternative<std::tuple<glm::dvec2, glm::dvec2>>(intersectionPoint)) {
        auto [intersection1, intersection2] = std::get<std::tuple<glm::dvec2, glm::dvec2>>(intersectionPoint);
        // Now we have the 2 intersection points, but only one of them is the real one.
        // The other is the one at the opposite->(not completely opposite but far side) side of the circle
        // To distinguish one from the another we will check the requirement that the correct intersection point
        // must be in between our old and new position points.
        glm::dvec2 trueIntersection;
        if (pointBetweenTwoPoints(intersection1, oldPosition, newPosition)) {
            trueIntersection = intersection1;
        } else if (pointBetweenTwoPoints(intersection2, oldPosition, newPosition)) {
            trueIntersection = intersection2;
        } else {
            // Something is wrong
            // What I will do here is geometrically incorrect but it handles a case arising from precision of floats
            // The commented out prints show the problematic points but I will just go around the problem in a slightly inaccurate way I think it is a very subtle approximation
            // Edit: I fixed this problem by switching the whole project from floats to doubles (vecs to dvecs)
            std::cout << "GEOMETRY ERROR reflectingOffCircle 3" << std::endl;
            
            double newPosLength = glm::length(newPosition);
            newPosition /= (newPosLength * 1.01f);
            
        }
        glm::dvec2 intersectionPointVec2(trueIntersection.x, trueIntersection.y);
        glm::dvec2 reflectionAboutIntersection  = reflectionAboutPoint(newPosition, intersectionPointVec2);
        std::tuple<double,double> radius = lineOfTwoPoints(circleCenter, intersectionPointVec2);
        auto [m, b] = radius;
        glm::dvec2 reflectionAboutRadius = reflectionAboutLine(reflectionAboutIntersection, m, b);
        
        return reflectionAboutRadius;
    } else if (std::holds_alternative<glm::dvec2>(intersectionPoint)){
        // This should never be the case
        std::cout << "GEOMETRY ERROR reflectingOffCircle 1" << std::endl;
    } else {
        // This should never be the case
        std::cout << "GEOMETRY ERROR reflectingOffCircle 2" << std::endl;
    }
    
    return glm::dvec2(0.0);
}



