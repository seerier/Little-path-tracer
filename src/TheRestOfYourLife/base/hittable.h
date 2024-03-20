#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"

class material;

class hit_record {
public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    double u;
    double v;
    bool front_face;

    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        //Sets the hit record normal vector.
        //NOTE: the parameter'outward_normal' is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
    virtual aabb bounding_box() const = 0;

    virtual double pdf_value(const point3 &o, const vec3 &v) const {
        return 0.0;
    }

    virtual vec3 random(const vec3 &o) const {
        return vec3(1, 0, 0);
    }
};

class translate : public hittable {
public:
    translate(shared_ptr<hittable> p, const vec3 &displacement)
        :object(p), offset(displacement) {
        bbox = object->bounding_box() + offset;
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        //Move the ray backwards by the offset
        ray offset_r(r.origin() - offset, r.direction(), r.time());

        //Determine where (if any) an intersection occurs along the offset ray
        if (!object->hit(offset_r, ray_t, rec))
            return false;

        //Move the intersection point forwards by the offset
        rec.p += offset;

        return true;
    }

    aabb bounding_box() const override { return bbox; }
private:
    shared_ptr<hittable> object;
    vec3 offset;
    aabb bbox;
};

class scale : public hittable {
public:
    scale(shared_ptr<hittable> p, const vec3 &scaling)
        :object(p), sc(scaling) {
        bbox = object->bounding_box() * sc;
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        vec3 sc_inver(1.0 / sc.x(), 1.0 / sc.y(), 1.0 / sc.z());
        ray r_scaled(sc_inver * r.origin(), sc_inver * r.direction(), r.time());

        if (!object->hit(r_scaled, ray_t, rec))
            return false;

        rec.p = rec.p * sc;
        rec.normal = rec.normal * sc_inver;
        return true;
    }

private:
    shared_ptr<hittable> object;
    vec3 sc;
    aabb bbox;
};

class rotate_y : public hittable {
public:
    rotate_y(shared_ptr<hittable> p, double angle) :object(p) {
        auto radians = degrees_to_radians(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        bbox = object->bounding_box();

        point3 min(infinity, infinity, infinity);
        point3 max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k) {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto newx = cos_theta * x + sin_theta * z;
                    auto newz = -sin_theta * x + cos_theta * z;

                    vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; ++c) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
        bbox = aabb(min, max);
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        //Change the ray from world space to object space
        auto origin = r.origin();
        auto direction = r.direction();

        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

        if (!object->hit(ray(origin, direction, r.time()), ray_t, rec))
            return false;

        auto p = rec.p;
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

        auto normal = rec.normal;
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

        rec.p = p;
        rec.normal = normal;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};

#endif
