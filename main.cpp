#include "headers/rt_weekend.h"

#include "headers/camera.h"
#include "headers/hittable.h"
#include "headers/hittable_list.h"
#include "headers/shapes.h"


// THe base ray tracer was built following the Ray Tracing in One Weekend tutorial, as of this comment I have only
// followed the first of the tutorials, The code for this can be found in the rayTracingInOneWeekend folder in this dir

int main() {
    hittable_list world;


    auto mirror_material = make_shared<metal>(colour(0.2, 0.4, 0.6),0.0);

    Vertex vertices[3] = {
        Vertex({0, std::sqrt(3) / 2, -1}),
        Vertex({-1, -std::sqrt(3) / 2, -1}),
        Vertex({1, -std::sqrt(3) / 2, -1})
    };

    world.add(make_shared<triangle>(vertices, point3(0, 0, -1), mirror_material));

    auto ground= make_shared<lambertian>(colour(0,0,0));
    world.add(make_shared<sphere>(point3(0,-100.5,-1),100,ground));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 60.0;
    cam.lookfrom = point3(0, 1, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 5.0;
    cam.focus_dist = std::sqrt(3);

    cam.render(world);
}

//9.1
