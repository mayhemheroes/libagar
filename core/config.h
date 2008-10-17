/*	Public domain	*/

#include <agar/begin.h>

typedef struct ag_object AG_Config;

__BEGIN_DECLS
extern AG_Config *agConfig;
extern AG_ObjectClass agConfigClass;

extern int agKbdDelay, agKbdRepeat;
extern int agMouseDblclickDelay, agMouseSpinDelay, agMouseSpinIval;
extern int agTextComposition, agTextBidi, agTextAntialiasing, agTextTabWidth,
	   agTextBlinkRate, agTextAntialiasing, agTextCache, agPageIncrement,
	   agTextSymbols;
extern int agIdleThresh;
extern int agScreenshotQuality;
extern int agWindowAnySize;
extern int agMsgDelay;

int  AG_ConfigInit(AG_Config *, Uint);
int  AG_ConfigFile(const char *, const char *, const char *, char *, size_t)
		   BOUNDED_ATTRIBUTE(__string__, 4, 5);
int  AG_CreateDataDir(void);
int  AG_ConfigSave(void);
int  AG_ConfigLoad(void);
void AG_SetCfgString(const char *, const char *, ...);
__END_DECLS

#define AG_CfgUint(k) AG_GetUint(agConfig,(k))
#define AG_CfgInt(k) AG_GetInt(agConfig,(k))
#define AG_CfgBool(k) AG_GetBool(agConfig,(k))
#define AG_CfgString(k) AG_GetString(agConfig,(k))
#define AG_CfgStringCopy(k,b,s) AG_GetStringCopy(agConfig,(k),(b),(s))

#define AG_SetCfgUint(k,v) AG_SetUint(agConfig,(k),(v))
#define AG_SetCfgInt(k,v) AG_SetInt(agConfig,(k),(v))
#define AG_SetCfgBool(k,v) AG_SetBool(agConfig,(k),(v))

#include <agar/close.h>
