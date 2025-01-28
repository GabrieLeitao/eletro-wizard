#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class Vec3 {
public:
    double x, y, z;

    // Constructors
    Vec3();
    Vec3(double x, double y, double z);

    // Vector operations
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(double scalar) const;
    Vec3 cross(const Vec3& v) const;
    double dot(const Vec3& v) const;
    double magnitude() const;

    // Utility
    void print() const;
};

#endif // VEC3_H
