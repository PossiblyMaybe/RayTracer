//
// Created by will on 20/01/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
public:
    double aspect_ratio = 1.0; // Ratio of image width over height
    int image_width = 100; // Rendered image width in pixel count
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    point3 lookfrom{0, 0, 0};
    point3 lookat{0, 0, -1};
    vec3 vup{0, 1, 0};

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable &world) {
        initialise();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                colour pixel_colour;
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_colour += ray_colour(r, max_depth, world);
                }
                write_colour(std::cout, pixel_samples_scale * pixel_colour);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int image_height{};
    double pixel_samples_scale{};
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u, pixel_delta_v, u, v, w,
            defocus_disk_u, defocus_disk_v;

    void initialise() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Viewport dims
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);


        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;;
        auto viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    [[nodiscard]] ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                            + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    [[nodiscard]] static vec3 sample_square() {
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    [[nodiscard]] point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    static colour ray_colour(const ray &r, int depth, const hittable &world) {
        if (depth <= 0) return {0, 0, 0};

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            colour attenuation = {0, 0, 0};
            bool notEmissive = rec.mat->scatter(r, rec, attenuation, scattered);
            if (notEmissive)
                return attenuation * ray_colour(scattered, depth - 1, world);
            return attenuation;
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
    }
};

#endif //CAMERA_H
