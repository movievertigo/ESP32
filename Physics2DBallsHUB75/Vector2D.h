// By Movie Vertigo
// https://youtube.com/movievertigo
// https://twitter.com/movievertigo
 
class Vector2D
{
public:
  Vector2D() : X(0.0f), Y(0.0f) {}
  Vector2D(float x, float y) : X(x), Y(y) {}
  Vector2D(const Vector2D& other) : X(other.X), Y(other.Y) {}

  Vector2D& operator=(const Vector2D& other)
  {
    X = other.X;
    Y = other.Y;
    return *this;
  }

  Vector2D& operator+=(const Vector2D& other)
  {
    X += other.X;
    Y += other.Y;
    return *this;
  }

  Vector2D& operator-=(const Vector2D& other)
  {
    X -= other.X;
    Y -= other.Y;
    return *this;
  }

  Vector2D& operator*=(float scalar)
  {
    X *= scalar;
    Y *= scalar;
    return *this;
  }

  Vector2D& operator/=(float scalar)
  {
    X /= scalar;
    Y /= scalar;
    return *this;
  }

  Vector2D operator+(const Vector2D& other) const
  {
    return Vector2D(X + other.X, Y + other.Y);
  }

  Vector2D operator-(const Vector2D& other) const
  {
    return Vector2D(X - other.X, Y - other.Y);
  }

  Vector2D operator*(float scalar) const
  {
    return Vector2D(X * scalar, Y * scalar);
  }

  Vector2D operator/(float scalar) const
  {
    return Vector2D(X / scalar, Y / scalar);
  }

  float Length() const
  {
    return sqrtf(X*X + Y*Y);
  }

  Vector2D Normalised() const
  {
    return *this / Length();
  }

  float DotProduct(const Vector2D& other) const
  {
    return (X * other.X) + (Y * other.Y);
  }

  float X = 0.0f, Y = 0.0f;
};
