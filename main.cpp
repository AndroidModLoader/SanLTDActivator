#include <mod/amlmod.h>

MYMOD(net.rusjj.gtasa.sanltd, GTA:SA SanLTD Activator, 1.1, RusJJ)

#ifdef AML32
    #define BYVER(__for32, __for64) (__for32)
#else
    #define BYVER(__for32, __for64) (__for64)
#endif

struct RwTexture;

void* hGTASA;
uintptr_t pGTASA;

uintptr_t sanltd_fonts_db;
uintptr_t pFontSprite[3]; // 0 - font2, 1 - font1, 2 - primary font (CSprite's)
RwTexture* (*SetSpriteTexture1)(uintptr_t* sprite, const char* texture);
RwTexture* (*SetSpriteTexture2)(uintptr_t* sprite, const char* texture, const char* mask);
uintptr_t (*LoadTextureDB)(const char* dbFile, bool fullLoad, int txdbFormat);
void (*RegisterTextureDB)(uintptr_t dbPtr);

DECL_HOOKv(AddStandartTexture)
{
    AddStandartTexture();
    if(pFontSprite[0] != 0 && pFontSprite[1] != 0)
    {
        SetSpriteTexture2(&pFontSprite[0], "font2_sanltd", "font2m");
        SetSpriteTexture2(&pFontSprite[1], "font1_sanltd", "font1m");
    }
}
DECL_HOOKv(InitialiseRenderWare)
{
    InitialiseRenderWare();
    sanltd_fonts_db = LoadTextureDB("sanltd_fonts", false, 5);
    RegisterTextureDB(sanltd_fonts_db);
}

extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");
    hGTASA = dlopen("libGTASA.so", RTLD_LAZY);

    SET_TO(pFontSprite, aml->GetSym(hGTASA, "_ZN5CFont6SpriteE"));
    SET_TO(SetSpriteTexture1, aml->GetSym(hGTASA, "_ZN9CSprite2d10SetTextureEPc"));
    SET_TO(SetSpriteTexture2, aml->GetSym(hGTASA, "_ZN9CSprite2d10SetTextureEPcS0_"));
    SET_TO(LoadTextureDB, aml->GetSym(hGTASA, "_ZN22TextureDatabaseRuntime4LoadEPKcb21TextureDatabaseFormat"));
    SET_TO(RegisterTextureDB, aml->GetSym(hGTASA, "_ZN22TextureDatabaseRuntime8RegisterEPS_"));

    aml->Write(pGTASA + BYVER(0x2A5C78, 0x73B13D), (uintptr_t)"SanLTD", 7);
    aml->Write(pGTASA + BYVER(0x5A8904, 0x764641) + 0x5, (uintptr_t)"SANFT", 5);
    aml->Write(pGTASA + BYVER(0x61EC30, 0x76128D), (uintptr_t)"SANLTD", 6);

    #ifdef AML32
    HOOKPLT(AddStandartTexture, pGTASA + 0x6747CC);
    HOOKPLT(InitialiseRenderWare, pGTASA + 0x66F2D0);
    #else
    HOOK(AddStandartTexture, aml->GetSym(hGTASA, "_ZN5CFont18AddStandardTextureEv"));
    HOOK(InitialiseRenderWare, aml->GetSym(hGTASA, "_ZN5CGame20InitialiseRenderWareEv"));
    #endif
}