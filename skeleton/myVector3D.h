#pragma once

class myVector3D
{
public:
    myVector3D();
    myVector3D(double x, double y, double z);
    myVector3D(const myVector3D& other);

    myVector3D& operator=(const myVector3D& other);
    myVector3D operator+(const myVector3D& other) const;
    myVector3D operator-(const myVector3D& other) const;
    myVector3D operator*(double scalar) const;

    double getMod() const;
    void normalizeVector();
    double dotProduct(const myVector3D& other) const;

    double getX() const;
    double getY() const;
    double getZ() const;
    void setVector(double x, double y, double z);
    void print() const;

private:
    double x;
    double y;
    double z;
};

myVector3D operator*(double scalar, const myVector3D& vector);