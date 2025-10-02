#include "myVector3D.h"
#include <iostream>
#include <cmath>

myVector3D::myVector3D() : x(0.0), y(0.0), z(0.0) {}

myVector3D::myVector3D(double x, double y, double z) : x(x), y(y), z(z) {}

myVector3D::myVector3D(const myVector3D& other) : x(other.x), y(other.y), z(other.z) {}

myVector3D& myVector3D::operator=(const myVector3D& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

myVector3D myVector3D::operator+(const myVector3D& other) const {
    return myVector3D(x + other.x, y + other.y, z + other.z);
}

myVector3D myVector3D::operator-(const myVector3D& other) const {
    return myVector3D(x - other.x, y - other.y, z - other.z);
}

myVector3D myVector3D::operator*(double scalar) const {
    return myVector3D(x * scalar, y * scalar, z * scalar);
}

double myVector3D::getMod() const {
    return std::sqrt(x * x + y * y + z * z);
}

void myVector3D::normalizeVector() {
    double mod = getMod();
    if (mod > 0.0) {
        x /= mod;
        y /= mod;
        z /= mod;
    }
}

double myVector3D::dotProduct(const myVector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

double myVector3D::getX() const { return x; }
double myVector3D::getY() const { return y; }
double myVector3D::getZ() const { return z; }

void myVector3D::setVector(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void myVector3D::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}

myVector3D operator*(double scalar, const myVector3D& vector) {
    return myVector3D(vector.getX() * scalar, vector.getY() * scalar, vector.getZ() * scalar);
}