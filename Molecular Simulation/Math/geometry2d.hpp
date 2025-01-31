//
//  geometry2d.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#ifndef geometry2d_hpp
#define geometry2d_hpp

#include <stdio.h>
#include <tuple>
#include <glm/glm.hpp>
#include <variant>
#include <iostream>

#include "../Boundaries/Cylinder.hpp"


std::tuple<double, double> lineOfTwoPoints(glm::dvec2 point1, glm::dvec2 point2);
std::variant<std::monostate, glm::dvec2, std::tuple<glm::dvec2, glm::dvec2>>
intersectionOfLineAndCircle(double m, double b, double x0, double y0, double r);
glm::dvec2 reflectionAboutPoint(glm::dvec2 reflectedPoint, glm::dvec2 pivotPoint);
glm::dvec2 reflectionAboutLine(glm::dvec2 reflectedPoint, double m, double b);
glm::dvec2 reflectingOffCircle(glm::dvec2 oldPosition, glm::dvec2 newPosition, glm::dvec2 circleCenter, double cirleRadius);


#endif /* geometry2d_hpp */
