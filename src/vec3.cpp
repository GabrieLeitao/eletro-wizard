#include "vec3.h"

// Default constructor
Vec3::Vec3() : x(0), y(0), z(0) {}

// Parameterized constructor
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

// Vector addition
Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

// Vector subtraction
Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

// Scalar multiplication
Vec3 Vec3::operator*(double scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}

// Cross product
Vec3 Vec3::cross(const Vec3& v) const {
    return Vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

// Dot product
double Vec3::dot(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// Magnitude
double Vec3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Print vector
void Vec3::print() const {
    std::cout << "Vec3(" << x << ", " << y << ", " << z << ")" << std::endl;
}
