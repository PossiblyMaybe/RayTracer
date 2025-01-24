//
// Created by will on 19/01/25.
//

#ifndef SPHERE_H
#define SPHERE_H

#include <utility>

#include "hittable.h"

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

    vec3 normal;

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

struct Vertex {
    point3 pos;

    explicit Vertex(const point3 &pos): pos(pos) {
    }
};


class triangle : public hittable {
public:
    point3 pos;

    explicit triangle(Vertex vertices[3], const point3 &pos, const shared_ptr<material> &mat) : vertices(vertices[0],
            vertices[1], vertices[2]),
        pos(pos), mat(mat),
        normal(cross(vertices[1].pos - vertices[0].pos, vertices[2].pos - vertices[0].pos)) {
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        // standard linear algebra to find t:
        // used n.r = n.a to find it, it's written down on my obsidian

        double rn = dot(normal, vertices[0].pos);
        double qn = dot(r.origin(), normal);
        double dn = std::abs(dot(r.direction(), normal));

        double t = (qn - rn) / dn;


        if (!ray_t.surrounds(t)) return false;
        rec.p = r.at(t);

        double total_area = get_area(vertices[0].pos, vertices[1].pos, vertices[2].pos);

        double area_sum = get_area(vertices[0].pos, vertices[1].pos, rec.p)
                          + get_area(vertices[0].pos, vertices[2].pos, rec.p)
                          + get_area(vertices[1].pos, vertices[2].pos, rec.p);

        if (total_area != area_sum) return false;

        rec.t = t;
        rec.set_face_normal(r, normal);
        rec.mat = mat;
        return true;
    }

private:
    vec3 normal;
    Vertex vertices[3];
    shared_ptr<material> mat;

    [[nodiscard]] static double get_area(point3 a, point3 b, point3 c) {
        vec3 AB = b - a;
        vec3 AC = c - a;
        return cross(AB, AC).length() / 2;
    }
};

#include "rt_weekend.h"

#endif //SPHERE_H
