#pragma once
#include <math.h>
#define PI 3.141592653;

class Vector3
{
public:
	float x, y, z;

	inline Vector3() { x = y = z = 0.0f; }
	inline Vector3(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	inline float operator[](int i) const { return ((float*)this)[i]; }
	inline Vector3& operator-=(float v) { x -= v; y -= v; z -= v; return *this; }
	inline Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	inline Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	inline Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	inline Vector3 operator*(float v) const { return Vector3(x * v, y * v, z * v); }
	inline Vector3 operator/(float v) const { return Vector3(x / v, y / v, z / v); }
	inline bool operator==(const Vector3& v) const { return (x == v.x && y == v.y && z == v.z); }
	inline bool operator!=(const Vector3& v) const { return (x != v.x && y != v.y && z != v.z); }
	inline float Length() { return sqrtf(x * x + y * y + z * z); }
	float Magnitude() { return sqrt(this->x * this->x + this->y * this->y + this->z * this->z); }
	Vector3 Normalized() { float num = this->Magnitude(); if (num > 9.99999974737875E-06) { return { this->x / num, this->y / num, this->z / num }; } return Vector3(); }
};

class Vector2 {
public:
	float x, y;

	inline Vector2() { x = y = 0.0f; }
	inline Vector2(float X, float Y) { x = X; y = Y; }
	inline Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	inline Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	inline Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	inline Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	inline Vector2 operator*(float v) const { return Vector2(x * v, y * v); }
	inline Vector2 operator/(float v) const { return Vector2(x / v, y / v); }
	inline bool operator==(const Vector2& v) const { return (x == v.x && y == v.y); }
	inline bool operator!=(const Vector2& v) const { return (x != v.x && y != v.y); }
	inline bool Zero() const { return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f); }
};

class Vector4
{
public:
	float x, y, z, w;
};