#include "Object.h"
#include <iostream>

inline bool solveQuadratic(const float &a,
                        const float &b,
                        const float &c,
                        float &x0,
                        float &x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

void Object::setMaterial(Material * mat){
    m = mat;
}

Triangle::Triangle(Point v1,Point v2, Point v3, Point no){
    p1 = v1;
    p2 = v2;
    p3 = v3;

    n = no;
    isLight = false;
}

Sphere::Sphere(Point p, double r){
    center = p;
    radius = r;

    isLight = false;
}
#define EPS 1E-6

Point Triangle::getIntersection(Ray r){

	// YOUR INTERSECTION CODE HERE.
	// RETURN THE POINT OF INTERSECTION FOR THIS TRIANGLE.
    float t, u, v;

    Point side1, side2, side3;
    Point pVec, qVec, tVec;

    side1 = p2-p1;
    side2 = p3-p1;
    side3 = r.p - p1;
    //float det = side1.cross(side2) * r.v;//(r.v * -1.0);
    float det = r.v.cross(side2) * side1;

    float invDet = 1 / det;

    //u = (side3.cross(side2) * (r.v)) * (invDet);
    u = (r.v.cross(side2) * side3) * (invDet);
    //std::cout << "u" << u << std::endl;
    if (u < 0 || u > 1) return Point::Infinite();

    //v = (side1.cross(side3) * (r.v)) * (invDet);
    v = (side3.cross(side1) * r.v) * (invDet);
    //std::cout << "v" << v << std::endl;
    if (v < 0 || v > 1) return Point::Infinite();

    if (u + v >= 1) return Point::Infinite();

    //t = (side1.cross(side2) * side3) * (-1.0 * invDet);
    t = (side3.cross(side1) * side2) * (invDet);
    if (t < 0) return Point::Infinite();

    //Point intersection = r.p + r.v * t;
    Point intersection = p1 + side1 * u + side2 * v;

    return intersection;
}

Point Triangle::getNormal(Point p){
    Point one = p1-p2;
    Point two = p1-p3;
    Point ret = one.cross(two);
    ret.normalize();
    return ret;
}

Point Sphere::getNormal(Point p){
    Point ret = (p-center);

    ret.normalize();
    return ret;
}

Point Sphere::getIntersection(Ray r){

    float t0, t1;
    float a = r.v * r.v;
    float b = 2. * (r.v * (r.p - center));
    float c = (r.p - center) * (r.p - center) - radius * radius;

    if (!solveQuadratic(a, b, c, t0, t1)) return Point::Infinite();

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return Point::Infinite();
    }

    Point offset = Point(r.v.x * t0, r.v.y * t0, r.v.z*t0);

    return r.p + offset;
}
