#include "headers/rt_weekend.h"

#include "headers/camera.h"
#include "headers/hittable.h"
#include "headers/hittable_list.h"
#include "headers/shapes.h"


// THe base ray tracer was built following the Ray Tracing in One Weekend tutorial, as of this comment I have only
// followed the first of the tutorials, The code for this can be found in the rayTracingInOneWeekend folder in this dir

int main() {
    hittable_list world;


    auto ground_material = make_shared<lambertian>(colour(0.2, 0.4, 0.6));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, ground_material));

    auto emissive_mat = make_shared<emissive>(50, colour(1, 0.6, 0.4));
    world.add(make_shared<sphere>(point3(1.5, 0, -1), 0.1, emissive_mat));

    auto mirror_mat = make_shared<metal>(colour(1.0, 1.0, 1.0), 0.1);
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, mirror_mat));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 60.0;
    cam.lookfrom = point3(0, 0, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 5.0;
    cam.focus_dist = 2.0;

    cam.render(world);
}

//9.1
