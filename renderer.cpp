#include "renderer.h"
#include <math.h>
#include "raycaster_data.h"

void Renderer::ShowFPS(uint16_t fps, uint32_t *fb)
{
    for (int i = 1; fps > 0; i++, fps /= 10) {
        int pos = SCREEN_WIDTH - 15 * i;
        for (int j = 0; j < 4; j++) {
            uint32_t *lb = fb + pos + j * 2 + SCREEN_WIDTH;
            for (int k = 0; k < 16; k++, lb += SCREEN_WIDTH) {
                int ptr = k / 2;
                if (g_number[fps % 10][ptr * 4 + j] == 1) {
                    uint32_t *tb = lb;
                    for (int x = 0; x < 2; x++) {
                        *tb = 0x88444444;
                        tb++;
                    }
                }
            }
        }
    }
}

void Renderer::TraceFrame(Game *g, uint32_t *fb)
{
    _rc->Start(static_cast<uint16_t>(g->playerX * 256.0f),
               static_cast<uint16_t>(g->playerY * 256.0f),
               static_cast<int16_t>(g->playerA / (2.0f * M_PI) * 1024.0f));

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        uint8_t sso;
        uint8_t tc;
        uint8_t tn;
        uint16_t tso;
        uint16_t tst;
        uint32_t *lb = fb + x;

        _rc->Trace(x, &sso, &tn, &tc, &tso, &tst);

        const auto tx = static_cast<int>(tc >> 2);
        int16_t ws = HORIZON_HEIGHT - sso;
        if (ws < 0) {
            ws = 0;
            sso = HORIZON_HEIGHT;
        }
        uint16_t to = tso;
        uint16_t ts = tst;

        for (int y = 0; y < ws; y++) {
            *lb = GetARGB(96 + (HORIZON_HEIGHT - y));
            lb += SCREEN_WIDTH;
        }

        for (int y = 0; y < sso * 2; y++) {
            // paint texture pixel
            auto ty = static_cast<int>(to >> 10);
            auto tv = g_texture8[(ty << 6) + tx];

            to += ts;

            if (tn == 1 && tv > 0) {
                // dark wall
                tv >>= 1;
            }
            *lb = GetARGB(tv);
            lb += SCREEN_WIDTH;
        }

        for (int y = 0; y < ws; y++) {
            *lb = GetARGB(96 + (HORIZON_HEIGHT - (ws - y)));
            lb += SCREEN_WIDTH;
        }
    }
}
