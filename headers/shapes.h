//
// Created by will on 19/01/25.
//

#ifndef SPHERE_H
#define SPHERE_H

#include <utility>

#include "hittable.h"

struct Vertex {
    point3 pos[3];
};


class sphere : public hittable {
public:
    sphere(const point3 &center, double radius, shared_ptr<material> mat) : center(center),
                                                                            radius(std::fmax(0, radius)),
                                                                            mat(std::move(mat)) {
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto disc = h * h - a * c;
        if (disc < 0) {
            return false;
        }
        auto sqrtd = std::sqrt(disc);

        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

class triangle : public hittable {
public:
    point3 pos;
    explicit triangle(const Vertex vertices[3],const point3& pos) : pos(pos) {
        // vertices are local coordinates, will require view transformation to place into the world
        this->vertices[0] = vertices[0];
        this->vertices[1] = vertices[1];
        this->vertices[2] = vertices[2];
    }



private:
    Vertex vertices[3];
};


#endif //SPHERE_H
