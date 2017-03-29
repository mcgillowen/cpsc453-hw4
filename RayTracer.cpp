#include "RayTracer.h"

RayTracer::RayTracer(Scene * s, int maxd, int sm){
  scene = s;
  maxdepth = maxd;
  samples = sm;
}

// This function determines the closest object that a ray intersects.
// The getIntersection() method is polymorphic. Please provide implementations
// for all the objects in your scene (see Object.h and Object.cpp).
Object * RayTracer::intersect(Ray r){
  scene->startIteration();
  Object * current = NULL;
  Object * minObject = NULL;
  Point inter;
  //Initialize min to infinite point;
  Point min = Point::Infinite();
  while((current = scene->getNextObject()) != NULL){
    inter = current->getIntersection(r);
    if((inter - r.p).length() > 1E-6){
      if((inter-r.p).length()  < (min-r.p).length()){
        min = inter;
        minObject = current;
      }
    }
  }
  return minObject;
}

// Trace a ray recursively
Color RayTracer::trace(Ray r, int depth){
  Color rad=Color(0.0,0.0,0.0,0.0);

  // YOUR CODE FOR RECURSIVE RAY TRACING GOES HERE

  if (depth > maxdepth) return rad;

  Object *obj = this->intersect(r);
  if (obj == NULL) return rad;

  Point pt = obj->getIntersection(r);
  Point normal = obj->getNormal(pt);

  Ray reflected = r.reflect(normal, pt);

  Material *mat = obj->getMaterial();

  Color shadedColor = rad + this->Phong(normal, pt, r, mat, obj);

  if (mat->type == REFLECTIVE) {
    rad = shadedColor + this->trace(reflected, depth + 1) * mat->kr;
    } else {
    rad = shadedColor;
}
    rad.clamp(1.0);
  return rad ;
}

// Local Phong illumination at a point.
Color RayTracer::Phong(Point normal,Point p, Ray r, Material * m, Object * o){
  Color ret = Color(0.0, 0.0, 0.0, 0.0);

  ret = ret + m->getAmbient(p);

  int count = scene->lights.size();

  Point current = Point::Infinite();

  int specularDegree = 1;

  // YOUR CODE HERE.
  // There is ambient lighting irrespective of shadow.
  // Specular-diffuse lighting only if the point is not in shadow

  while ((current = scene->getNextLight()) < Point::Infinite() && count > 0) {
    count--;
    Point lightVector = current - p;
    Ray lightRay = Ray(p, lightVector);
    Object * obj = this->intersect(lightRay);
    if (obj != NULL) {
        Point pt = obj->getIntersection(lightRay);
        Point origin = Point();
        double distanceIntersection = pt.length();
        double distanceLight = current.length();
        if (distanceIntersection < distanceLight) {
            continue;
        }
    }

    // calculate phong shading here
    Ray reflectedLight = lightRay.reflect(normal, p);

    double diffuse = std::max(lightVector * normal, 0.0);

    double specular = std::max(reflectedLight.v * r.v, 0.0);

    for (int i = 0; i < specularDegree; i++) {
        specular = specular * specular;
    }


    ret = ret + m->getDiffuse(p) * diffuse;
    ret = ret + m->getSpecular(p) * specular;


  }

  // Remember, you need to account for all the light sources.
  ret.clamp(1.0);
  return ret;
}


// This function generates point on the image plane and starts a trace
// through them.
// Grid supersampling is also implemented.
Color RayTracer::calculate(int x, int y){
  Color c = Color(0.0,0.0,0.0,0.0);
  for(int i = 1; i <= samples; i++){
    for(int j = 1; j <= samples; j++){
      double one, two;
      // We subtract 0.5 so that the (u,v)-origin is at the
      //center of the image plane.
      one = double(x)-0.5+double(i)/double(samples);
      two = double(y)-0.5+double(j)/double(samples);
      Ray r = scene->makeRay(one,two);
      c = c+trace(r,0);
    }
  }
  c = c*(1.0/double(samples*samples));
  return c;
}
