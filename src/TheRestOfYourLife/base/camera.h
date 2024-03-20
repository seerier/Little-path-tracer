#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "pdf.h"

class camera {
public:
    //Public Camera Parameters Here.
    double aspect_ratio = 1.0;      //Ratio of image width over height
    int image_width = 100;          //Rendered image width in pixel count
    int samples_per_pixel = 10;     //Count of random samples for each pixel
    int max_depth = 10;             //Maximum number of ray bounces into scene
    color background;               //Scene background color

    double vfov = 90;                    //Vertical view angle (field of view)
    point3 lookfrom = point3(0, 0, -1);  //Point camera is looking from
    point3 lookat = point3(0, 0, 0);     //Point camera is looking at
    vec3 vup = vec3(0, 1, 0);            //Camera-relative "up" direction

    double defocus_angle = 0;  //Variation angle of rays through each pixel
    double focus_dist = 10;    //Distance from camera lookfrom point to plane of perfect focus

    void render(const hittable &world, const hittable &lights) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << image_height - j << " " << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                for (int s_j = 0; s_j < sqrt_spp; ++s_j)
                    for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
                        ray r = get_ray(i, j, s_i, s_j);
                        pixel_color += ray_color(r, max_depth, world, lights);
                    }

                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }



        std::clog << "\rDone.                    \n";
    }

private:
    //Private Camera Parameters Here.
    int image_height;
    int sqrt_spp;
    double recip_sqrt_spp;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;        //Camera frame basis vectors
    vec3 defocus_disk_u; //Defocus disk horizontal radius
    vec3 defocus_disk_v; //Defocus disk vertical radius

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        //Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        //!!!!!Notice!  
        // The sqrt_spp truncated the sqrt of samples_per_pixel.
        //For example, sqrt(1023) will be truncated to 31, then cast to 961.
        sqrt_spp = static_cast<int>(sqrt(samples_per_pixel));
        recip_sqrt_spp = 1.0 / sqrt_spp;

        //Calculations
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto viewport_u = viewport_width * u;
        auto viewport_v = -viewport_height * v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - focus_dist * w - 0.5 * (viewport_u + viewport_v);
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * tan(degrees_to_radians((defocus_angle) / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j, int s_i, int s_j) const {
        //Get a randomly sampled camera ray for the pixel at location i, j, originating from the camera defocus disk.
        auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
        auto pixel_sample = pixel_center + pixel_sample_square(s_i, s_j);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 pixel_sample_square(int s_i, int s_j) const {
        //Returns a random point in the square surrounding a pixel at the origin, given
        //the two subpixel indices.
        auto px = -0.5 + recip_sqrt_spp * (s_i + random_double());
        auto py = -0.5 + recip_sqrt_spp * (s_j + random_double());
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    point3 defocus_disk_sample() const {
        //Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray &r, int depth, const hittable &world, const hittable &lights) {
        hit_record rec;

        //If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0, 0, 0);

        //If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        scatter_record srec;
        color color_from_emission = rec.mat->emitted(r, rec);

        if (!rec.mat->scatter(r, rec, srec))
            return color_from_emission;

        if (srec.skip_pdf) {
            return srec.attenuation * ray_color(srec.skip_pdf_ray, depth - 1, world, lights);
        }

        auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
        mixture_pdf mixed_pdf(light_ptr, srec.pdf_ptr);

        ray scattered = ray(rec.p, mixed_pdf.generate(), r.time());
        double pdf_val = mixed_pdf.value(scattered.direction());

        double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

        color sample_color = ray_color(scattered, depth - 1, world, lights);
        color color_from_scatter = (srec.attenuation * scattering_pdf * sample_color) / pdf_val;
        return color_from_emission + color_from_scatter;
    }
};

#endif



