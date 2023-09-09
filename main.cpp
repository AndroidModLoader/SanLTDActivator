#include <mod/amlmod.h>

MYMOD(net.rusjj.gtasa.sanltd, GTA:SA SanLTD Activator, 1.2, RusJJ)

#ifdef AML32
    #define BYVER(__for32, __for64) (__for32)
#else
    #define BYVER(__for32, __for64) (__for64)
#endif

struct RwTexture;
struct TextureDatabaseRuntime;

uintptr_t pGTASA;
void* hGTASA;

TextureDatabaseRuntime* pSanLTDTextureDatabase = NULL;
RwTexture **pFontTextures; // RwTexture*[3] (originally CSprite[3]), 0 - font2, 1 - font1, 2 - primary font

TextureDatabaseRuntime* (*LoadTextureDB)(const char* dbFilename, bool fullLoad, int txdbFormat);
void (*RegisterTextureDB)(TextureDatabaseRuntime* dbPtr);
void (*UnregisterTextureDB)(TextureDatabaseRuntime* dbPtr);
void (*RwTextureDestroy)(RwTexture* texturePtr);
int (*GetEntry)(TextureDatabaseRuntime *,char const*, bool*);
RwTexture* (*GetRWTexture)(TextureDatabaseRuntime *, int);

inline RwTexture* GetTextureFromTexDB(TextureDatabaseRuntime* texdb, const char* name)
{
    bool hasSiblings;
    return GetRWTexture(texdb, GetEntry(texdb, name, &hasSiblings));
}
inline void SetFontTexture(RwTexture*& tex, const char* texture, const char* mask) // mask is useless?
{
    if(tex != NULL)
    {
        RwTextureDestroy(tex);
        tex = NULL;
    }
    if(texture && mask)
    {
        tex = GetTextureFromTexDB(pSanLTDTextureDatabase, texture);
    }
}

DECL_HOOKv(AddStandartTexture)
{
    AddStandartTexture();
    if(pFontTextures[0] != NULL && pFontTextures[1] != NULL)
    {
        if(!pSanLTDTextureDatabase) pSanLTDTextureDatabase = LoadTextureDB("sanltd_fonts", true, 6);
        SetFontTexture(pFontTextures[0], "font2_sanltd", "font2m");
        SetFontTexture(pFontTextures[1], "font1_sanltd", "font1m");
    }
}

extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");
    hGTASA = aml->GetLibHandle("libGTASA.so");

    SET_TO(pFontTextures,       aml->GetSym(hGTASA, "_ZN5CFont6SpriteE"));
    SET_TO(LoadTextureDB,       aml->GetSym(hGTASA, "_ZN22TextureDatabaseRuntime4LoadEPKcb21TextureDatabaseFormat"));
    SET_TO(RegisterTextureDB,   aml->GetSym(hGTASA, "_ZN22TextureDatabaseRuntime8RegisterEPS_"));
    SET_TO(UnregisterTextureDB, aml->GetSym(hGTASA, "_ZN22TextureDatabaseRuntime10UnregisterEPS_"));
    SET_TO(RwTextureDestroy,    aml->GetSym(hGTASA, "_Z16RwTextureDestroyP9RwTexture"));
    SET_TO(GetEntry,            aml->GetSym(hGTASA, "_ZN22TextureDatabaseRuntime8GetEntryEPKcRb"));
    SET_TO(GetRWTexture,        aml->GetSym(hGTASA, "_ZN22TextureDatabaseRuntime12GetRWTextureEi"));

    if(aml->HasMod("net.rusjj.gtasa.utils"))
    {
        aml->Write(pGTASA + BYVER(0x2A5C78, 0x73B13D), "SanLTD", 7);
    }
    else
    {
        aml->Write(pGTASA + BYVER(0x2A5C78, 0x73B13D), "FEL_RUS", 7);
    }
    aml->Write(pGTASA + BYVER(0x5A8904, 0x764641) + 0x5, "SANFT", 5);
    aml->Write(pGTASA + BYVER(0x61EC30, 0x76128D), "SANLTD", 6);

    HOOKPLT(AddStandartTexture, pGTASA + BYVER(0x6747CC, 0x846110));
}