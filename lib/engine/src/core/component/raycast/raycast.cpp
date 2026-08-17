#include "../include/core/component/raycast/raycast.hpp"

Ray ScreenToRay(float ndcX, float ndcY, const mathpp::mat4f &view, const mathpp::mat4f &projection) {


    mathpp::mat4f invProjView = mathpp::inverse(projection*view);

    mathpp::vec4f rayStartNDC = {ndcX,ndcY,-1.0f,1.0f};
    mathpp::vec4f rayEndNDC = {ndcX,ndcY,1.0f,1.0f};

    mathpp::vec4f rayStartWorldHomogenous = invProjView * rayStartNDC;
    mathpp::vec4f rayEndWorldHomogenous = invProjView * rayEndNDC;

    mathpp::vec3f rayStartWorld = rayStartWorldHomogenous / rayStartWorldHomogenous.w;
    mathpp::vec3f rayEndWorld = rayEndWorldHomogenous / rayEndWorldHomogenous.w;


    Ray ray;
    ray.origin = rayStartWorld;
    ray.direction = mathpp::normalize(rayEndWorld-rayStartWorld);


    return ray;
}

bool RaySphereIntersect(const Ray &ray, const mathpp::vec3f &origin, float radius) {
    mathpp::vec3f Ro = ray.origin - origin;
    float a = dot(ray.direction,ray.direction);
    float b = dot(2.0f * Ro,ray.direction);
    float c = dot ( Ro,Ro) - (radius*radius);

    float det = (b*b) - 4.0f * a * c;

    return det>=0;

}
