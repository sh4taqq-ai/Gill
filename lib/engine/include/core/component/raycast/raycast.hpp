#pragma once
#include "mathpp.hpp"


struct Ray {
    mathpp::vec3f origin;
    mathpp::vec3f direction;
};

Ray ScreenToRay(float ndcX,float ndcY,const mathpp::mat4f& view,const mathpp::mat4f& projection);

bool RaySphereIntersect(const Ray& ray, const mathpp::vec3f& origin, float radius);