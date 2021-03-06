#pragma once
//#include <array>
#include "raycaster.h"

class RayCasterFixed : public RayCaster
{
public:
    void Start(uint16_t playerX, uint16_t playerY, int16_t playerA);
    void Trace(uint16_t screenX,
               uint8_t *screenY,
               uint8_t *textureNo,
               uint8_t *textureX,
               uint16_t *textureY,
               uint16_t *textureStep);

    RayCasterFixed();
    ~RayCasterFixed();

private:
    uint16_t _playerX;
    uint16_t _playerY;
    int16_t _playerA;
    uint8_t _viewQuarter;
    uint8_t _viewAngle;

    static void CalculateDistance(uint16_t rayX,
                                  uint16_t rayY,
                                  uint16_t rayA,
                                  int16_t *deltaX,
                                  int16_t *deltaY,
                                  uint8_t *textureNo,
                                  uint8_t *textureX);
    static void LookupHeight(uint16_t distance,
                             uint8_t *height,
                             uint16_t *step);
    static bool IsWall(uint8_t tileX, uint8_t tileY);
    static int16_t MulTan(
        uint8_t value,
        bool inverse,
        uint8_t quarter,
        uint8_t angle,
        auto &lookupTable);  // same as const std::array<uint16_t, TBL_LEN>
    static int16_t AbsTan(uint8_t quarter, uint8_t angle, auto &lookupTable);
    static uint16_t MulU(uint8_t v, uint16_t f);
    static int16_t MulS(uint8_t v, int16_t f);
};
