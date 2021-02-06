// dllmain.cpp : Defines the entry point for the DLL application.

#include <cassert>
#include <Windows.h>
#include <iostream>

#include "../cr/cr.h"

int on_load(void* userdata)
{
    printf("[PLUGIN] Plugin loaded!\n");

    return 0;
}

int on_unload()
{
    printf("[PLUGIN] Plugin reloading!\n");

    return 0;
}

int on_close()
{
    printf("[PLUGIN] Plugin closing!\n");

    return 0;
}

CR_EXPORT int cr_main(struct cr_plugin* ctx, enum cr_op operation) {
    assert(ctx);


    switch (operation) {
    case CR_LOAD:       return on_load(ctx->userdata); // loading back from a reload
    case CR_UNLOAD:     return on_unload(); // preparing to a new reload
    case CR_CLOSE:      return on_close(); // the plugin will close and not reload anymore
    }

    printf("Cade REALLY Sux.\n");

    *(int*)5 = 3;

    return 0;
}