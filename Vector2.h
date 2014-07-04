#pragma once
#include <cmath>

namespace GameLayer
{
  template<class T>
  class Vector2
  {
  public:
    Vector2<T>(T a_x, T a_y) : x(a_x), y(a_y) {}
    Vector2<T>() : x(0), y(0) {}
    ~Vector2(){}

    Vector2<T> operator+(Vector2<T> a_rhs)  { return Vector2(x + a_rhs.x, y + a_rhs.y); }
    Vector2<T> operator-(const Vector2<T> a_rhs) const  { return Vector2(x - a_rhs.x, y - a_rhs.y); }
    Vector2<T>& operator+=(Vector2<T> a_rhs)  { x += a_rhs.x; y += a_rhs.y; return *this; }
    Vector2<T>& operator-=(Vector2<T> a_rhs)  { x -= a_rhs.x; y -= a_rhs.y; return *this; }

    Vector2<T> operator*(float a_scalar) const { return Vector2(x * a_scalar, y * a_scalar); }
    Vector2<T>& operator*=(float a_scalar)  { x *= a_scalar; y *= a_scalar; return *this; }

    Vector2<T> operator/(float a_scalar) const { return Vector2(x / a_scalar, y / a_scalar); }
    Vector2<T>& operator/=(float a_scalar)  { x /= a_scalar; y /= a_scalar; return *this; }

    const float Dot(Vector2<T> a_other) const { return x * a_other.x + y * a_other.y; }
    const void Normalize() {
      float mag = Magnitude(); x /= mag; y /= mag;
    }

    const float Magnitude() const { return std::sqrtf(x*x + y*y); }
    const float Magnitude(Vector2<T> a_vec) const { return std::sqrtf(a_vec.x*a_vec.x + a_vec.y*a_vec.y); }
    const float MagnitudeSquared() const { return x*x + y*y; }

    const float Distance(Vector2<T> a_rhs) const { Vector2<T> vec = (*this) - a_rhs; return vec.Magnitude(); }
    const float DistanceSquared() const { Vector2<T> vec = (*this) - a_rhs; return vec.MagnitudeSquared(); }
    
    static int ccw(const Vector2<float> &a, const Vector2<float> &b, const Vector2<float> &c)
    {
      int area = int((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
      if (area > 0)
        return -1;
      else if (area < 0)
        return 1;
      return 0;
    };

    union{
      struct{ T x, y; }; T vals[2];
    };


  };

  template<class T>
  Vector2<T> operator*(float a_scalar, Vector2<T> a_rhs)
  {
    return a_rhs * a_scalar;
  }

  typedef Vector2<int> Vector2i;
  typedef Vector2<float> Vector2f;

  struct Vertex
  {
    Vector2f m_position;
    float r, g, b, padding[3];
  };
};
