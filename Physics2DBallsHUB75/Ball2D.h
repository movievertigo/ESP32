// By Movie Vertigo
// https://youtube.com/movievertigo
// https://twitter.com/movievertigo
 
#include "Vector2D.h"

class Ball2D
{
public:
  Ball2D(const Vector2D& pos, float radius, uint16_t colour)
  {
    Pos = pos;
    Radius = radius;
    Colour = colour;
  }

  void draw(MatrixPanel_I2S_DMA *matrix)
  {
    matrix->fillCircle(Pos.X, Pos.Y, Radius, Colour);
  }

  Vector2D Pos, Vel;
  float Radius = 1.0f, Angle = 1.0f, Bounciness = 0.5f;
  uint16_t Colour;
};
