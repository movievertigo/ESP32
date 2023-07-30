// By Movie Vertigo
// https://youtube.com/movievertigo
// https://twitter.com/movievertigo
 
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_MPU6050.h>
#include "Ball2D.h"

const int MatrixWidth = 32, MatrixHeight = 32;
const float BallRadius = 2.5;
//const int MatrixWidth = 64, MatrixHeight = 64;
//const float BallRadius = 4.5;

MatrixPanel_I2S_DMA *matrix = nullptr;
Adafruit_MPU6050 mpu;
std::vector<Ball2D> balls;

void setup()
{
  HUB75_I2S_CFG matrixConfig(MatrixWidth, MatrixHeight);
  matrixConfig.gpio.e = 18;
  matrixConfig.clkphase = false;
  matrixConfig.double_buff = true;

  matrix = new MatrixPanel_I2S_DMA(matrixConfig);
  matrix->begin();
  matrix->setLatBlanking(4);
  matrix->clearScreen();

  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  mpu.setMotionInterrupt(false);


  for (int ball = 1; ball <= 6; ++ball)
  {
    balls.emplace_back(Vector2D(rand() % MatrixWidth, rand() % MatrixHeight), BallRadius, matrix->color565(ball&1?255:0, ball&2?255:0, ball&4?255:0));
  }
}

void loop()
{
  matrix->flipDMABuffer();
  matrix->clearScreen();

  delay(10);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Vector2D acceleration(a.acceleration.y, -a.acceleration.x);
  float deltaTime = MatrixHeight / 320.0f;

  for (int index = 0; index < balls.size(); ++index)
  {
    Ball2D& b = balls[index];
    b.Vel += acceleration * deltaTime;
    b.Pos += b.Vel * deltaTime;

    for (int collisionIndex = 0; collisionIndex < balls.size(); ++collisionIndex)
    {
      if (index != collisionIndex)
      {
        Ball2D& o = balls[collisionIndex];
        Vector2D difference = o.Pos - b.Pos;
        Vector2D normalised = difference.Normalised();
        float distance = difference.Length();
        float minDistance = b.Radius + o.Radius;
        if (distance < minDistance)
        {
          b.Pos -= normalised * (minDistance - distance);

          Vector2D bVelComponent = normalised * b.Vel.DotProduct(normalised);
          Vector2D oVelComponent = normalised * o.Vel.DotProduct(normalised);
          b.Vel = b.Vel - bVelComponent + bVelComponent * ((b.Radius-o.Radius)/(b.Radius+o.Radius)) + oVelComponent * ((2*o.Radius)/(b.Radius+o.Radius));
          o.Vel = o.Vel - oVelComponent + oVelComponent * ((o.Radius-b.Radius)/(o.Radius+b.Radius)) + bVelComponent * ((2*b.Radius)/(o.Radius+b.Radius));
        }
      }
    }

    if (b.Pos.X < b.Radius) { b.Pos.X = b.Radius; b.Vel.X = -b.Vel.X * b.Bounciness; }
    if (b.Pos.X > MatrixWidth - b.Radius) { b.Pos.X = MatrixWidth - b.Radius; b.Vel.X = -b.Vel.X * b.Bounciness; }
    if (b.Pos.Y < b.Radius) { b.Pos.Y = b.Radius; b.Vel.Y = -b.Vel.Y * b.Bounciness; }
    if (b.Pos.Y > MatrixHeight - b.Radius) { b.Pos.Y = MatrixHeight - b.Radius; b.Vel.Y = -b.Vel.Y * b.Bounciness; }
  }    

  for (int index = 0; index < balls.size(); ++index)
  {
    balls[index].draw(matrix);
  }    
}


