#include <common.h>
#include <gpu.h>
#include "tools.h"
#include <layer.h>
#define rectCount ((int *)0x1f80011c)[0]
#define tileInfoP (((u_char *)*(int *)0x1f80000c))
#define layoutP ((u_char *)*(int *)0x1f800004)
#define screenDataP ((ushort *)*(int *)0x1f800008)
#define layoutWidth ((u_char *)0x80172224)[0]
#define layoutHeight ((u_char *)0x80173a28)[0]
#define tempPrimP ((int *)0x1f800108)[0]
void DrawDebugText(ushort x, ushort y, byte clut, char *textP, ...);
void MenuCheck()
{
    if (tools.opended)
    {
        DrawDebugText(4, 4, 0, "Show Collision %u", tools.enableCollision);
        if ((buttonsPressed & PAD_CROSS) != 0)
        {
            struct camStruct *bgInfo = 0x801419b0;
            tools.enableCollision ^= 1;
            bgInfo[0].update = true;
            bgInfo[1].update = true;
            bgInfo[2].update = true;
        }
        if ((buttonsPressed & PAD_SELECT) != 0)
        {
            tools.opended = false;
        }
    }
    else
    {
        if ((buttonsPressed & PAD_SELECT) != 0)
        {
            tools.opended = true;
            tools.mode = 0;
            tools.mode2 = 0;
            tools.mode3 = 0;
            cursor = 0;
        }
        void (*runVarious)() = 0x80021158;
        runVarious();
    }
}
DR_TPAGE collisionTPage[2];
void DrawCollision()
{
    struct camStruct *bgInfo = 0x801419b0;
    DR_TPAGE *tpages = &collisionTPage[buffer];
    void *startP = tpages;
    SetDrawTPage(tpages, 0, 0, GetTPageValue(0, 0, 0xF, 1));
    AddPrim(&drawP->ot[1], tpages);
    int layerX = (int)bgInfo[0].X;
    int layerY = (int)bgInfo[0].Y;

    for (size_t y = 0; y < 16; y++)
    {
        for (size_t x = 0; x < 21; x++)
        {
            /* code */
            int x16 = layerX + x * 16;
            int y16 = layerY + y * 16;

            if ((uint)x16 > ((layoutWidth - 0) << 8))
            {
                x16 = (layoutWidth - 0) << 8;
            }
            if ((uint)y16 > ((layoutHeight - 0) << 8))
            {
                y16 = (layoutHeight - 0) << 8;
            }

            uint offset = (x16 >> 8) + (y16 >> 8) * layoutWidth;
            int screenId = layoutP[offset];
            ushort tileVal = screenDataP[screenId * 0x100 + ((x16 & 0xF0) >> 4) + (y16 & 0xF0)];

            if (tileVal != 0)
            {
                if (rectCount > 999)
                {
                    return;
                }
                ushort tileId = tileVal & 0x3FFF;

                /*Get Various Tile Id Info*/
                u_char collision = tileInfoP[tileId * 4] & 0x3F;

                SPRT_16 *rect = tempPrimP;

                /*RAW Textured Rectangle*/
                *(u_char *)((int)&rect->tag + 3) = 3;
                rect->code = 0x7D;

                if (collision != 0x3F)
                {
                    setUV0(rect, (collision & 0xF) * 16, (collision & 0xF0) + 192);
                }
                else
                {
                    setUV0(rect, 240, 176);
                }
                setClut(rect, 0x100, 0x1F8);
                /*Calculate Draw Location*/
                int innerX = x16 & 0xF;
                int innerY = y16 & 0xF;
                setXY0(rect, x * 16 - innerX, y * 16 - innerY);

                AddPrim(startP, rect);

                rectCount += 1;
                tempPrimP += sizeof(SPRT_16);
            }
        }
    }
}

#undef rectCount
#undef tileInfoP
#undef layoutP
#undef screenDataP
#undef layoutWidth
#undef layoutHeight
#undef tempPrimP