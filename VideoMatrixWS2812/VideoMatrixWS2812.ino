// By Movie Vertigo
// https://youtube.com/movievertigo
// https://twitter.com/movievertigo
 
#include <esp_task_wdt.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "BLEFastUDP.h"

const int MatrixBrightness = 32;
const int MatrixTileWidth = 8;
const int MatrixTileHeight = 8;
const int MatrixTileCountX = 2;
const int MatrixTileCountY = 2;

const std::string SERVICE_UUID = "5669644d-4d6f-7669-6556-65727469676f"; // VidM-Mo-vi-eV-ertigo

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MatrixTileWidth, MatrixTileHeight, MatrixTileCountX, MatrixTileCountY, 2,
  NEO_TILE_TOP       + NEO_TILE_RIGHT +
  NEO_TILE_COLUMNS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB);

BLEFastUDP bleData;
const int MatrixWidth = MatrixTileWidth * MatrixTileCountX;
const int MatrixHeight = MatrixTileHeight * MatrixTileCountY;
const int MatrixDataLength = MatrixWidth * MatrixHeight * 2;

void setup()
{
  Serial.begin(115200);

  matrix.begin();
  matrix.setBrightness(MatrixBrightness);

  std::string info = "{\"width\":"+std::to_string(MatrixWidth)+",\"height\":"+std::to_string(MatrixHeight)+"}";

  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
  bleData.begin("Video Matrix", info, SERVICE_UUID, processData);

  disableCore0WDT();
  disableCore1WDT();
  disableLoopWDT();
  esp_task_wdt_delete(NULL);
}

void processData(const uint8_t* data, int length)
{
  if (length == MatrixDataLength)
  {
    const uint16_t* pixelData = (const uint16_t*)data;

    for (int y = 0; y < MatrixHeight; ++y)
    {
      for (int x = 0; x < MatrixWidth; ++x)
      {
        matrix.drawPixel(x, y, *pixelData++);
      }
    }
  }
}

void loop()
{
      matrix.show();
}
