#include <math.h>
#include <array>
#include "raycaster.h"

#ifndef HAS_TABLES
constexpr auto f_tan(std::size_t i)
{
    return static_cast<uint16_t>((256.0f * tan(i * M_PI_2 / 256.0f)));
}
constexpr auto f_cotan(std::size_t i)
{
    if (i == 0)
        return static_cast<uint16_t>(0);
    return static_cast<uint16_t>((256.0f / tan(i * M_PI_2 / 256.0f)));
}
constexpr auto f_sin(std::size_t i)
{
    return static_cast<uint8_t>(256.0f * sin(i / 1024.0f * 2 * M_PI));
}
constexpr auto f_cos(std::size_t i)
{
    if (i == 0)
        return static_cast<uint8_t>(0);
    return static_cast<uint8_t>(256.0f * cos(i / 1024.0f * 2 * M_PI));
}
constexpr auto f_deltaAngle(std::size_t i)
{
    float deltaAngle = atanf(((int16_t) i - SCREEN_WIDTH / 2.0f) /
                             (SCREEN_WIDTH / 2.0f) * M_PI / 4);
    int16_t da = static_cast<int16_t>(deltaAngle / M_PI_2 * 256.0f);
    if (da < 0)
        da += 1024;
    return static_cast<uint16_t>(da);
}
constexpr auto f_nearHeight(std::size_t i)
{
    return static_cast<uint8_t>(
        (INV_FACTOR_INT / (((i << 2) + MIN_DIST) >> 2)) >> 2);
}
constexpr auto f_farHeight(std::size_t i)
{
    return static_cast<uint8_t>(
        (INV_FACTOR_INT / (((i << 5) + MIN_DIST) >> 5)) >> 5);
}
constexpr auto f_nearStep(std::size_t i)
{
    auto txn =
        ((INV_FACTOR_INT / (((i * 4.0f) + MIN_DIST) / 4.0f)) / 4.0f) * 2.0f;
    if (txn != 0)
        return static_cast<uint16_t>((256 / txn) * 256);
}
constexpr auto f_farStep(std::size_t i)
{
    auto txf =
        ((INV_FACTOR_INT / (((i * 32.0f) + MIN_DIST) / 32.0f)) / 32.0f) * 2.0f;
    if (txf != 0)
        return static_cast<uint16_t>((256 / txf) * 256);
}
constexpr auto f_overflowOffset(std::size_t i)
{
    if (i == 0)
        return static_cast<uint16_t>(0);
    auto txs = ((INV_FACTOR_INT / (float) (i / 2.0f)));
    auto ino = (txs - SCREEN_HEIGHT) / 2;
    auto ret = ino * (256 / txs) * 256;
    return static_cast<uint16_t>(ret < 0 ? 65537 + ret : ret);
}
constexpr auto f_overflowStep(std::size_t i)
{
    if (i == 0)
        return static_cast<uint16_t>(0);
    auto txs = ((INV_FACTOR_INT / (float) (i / 2.0f)));
    return static_cast<uint16_t>((256 / txs) * 256);
}

template <typename T, std::size_t N, typename Generator>
constexpr std::array<T, N> make_array(Generator f)
{
    std::array<T, N> table = {};
    for (std::size_t i = 0; i != N; ++i) {
        table[i] = f(i);
    }
    return table;
}

constexpr static auto LOOKUP_TBL g_tan = make_array<uint16_t, TBL_LEN>(f_tan);
constexpr static auto LOOKUP_TBL g_cotan =
    make_array<uint16_t, TBL_LEN>(f_cotan);
constexpr static auto LOOKUP_TBL g_sin = make_array<uint8_t, TBL_LEN>(f_sin);
constexpr static auto LOOKUP_TBL g_cos = make_array<uint8_t, TBL_LEN>(f_cos);
constexpr static auto LOOKUP_TBL g_nearHeight =
    make_array<uint8_t, TBL_LEN>(f_nearHeight);
constexpr static auto LOOKUP_TBL g_farHeight =
    make_array<uint8_t, TBL_LEN>(f_farHeight);
constexpr static auto LOOKUP_TBL g_nearStep =
    make_array<uint16_t, TBL_LEN>(f_nearStep);
constexpr static auto LOOKUP_TBL g_farStep =
    make_array<uint16_t, TBL_LEN>(f_farStep);
constexpr static auto LOOKUP_TBL g_overflowOffset =
    make_array<uint16_t, TBL_LEN>(f_overflowOffset);
constexpr static auto LOOKUP_TBL g_overflowStep =
    make_array<uint16_t, TBL_LEN>(f_overflowStep);
constexpr static auto LOOKUP_TBL g_deltaAngle =
    make_array<uint16_t, SCREEN_WIDTH>(f_deltaAngle);
#endif  // ! HAS_TABLES
