#include <common.h>
#include <gpu.h>
#include <layer.h>
#include "tools.h"
#define rectCount ((int*)0x1f80011c)[0]
#define tempPrimP ((int*)0x1f800108)[0]
#define rectBufferAddress 0x8015d9d0

void DrawCollision();

void NewDrawBackground()
{
    struct camStruct * bgInfo = 0x801419b0;
    rectCount = 0;
    tempPrimP = rectBufferAddress + buffer * 0x4000;
    if(!tools.enableCollision)
    {
        for (size_t i = 0; i < 3; i++)
        {
            if(bgInfo[i].update){
                DumpLayerScreens(i);
            }
        }
        DumpActiveScreens();

        for (size_t i = 0; i < 3; i++)
        {
            if(bgInfo[i].display)
            {
                ResetLayerPointers(i);
                DrawLayer(i);
                AssignLayer(i);
            }
        }
        
        
    }else
    {
        DrawCollision();
    }

}
#undef rectCount
#undef tempPrimP