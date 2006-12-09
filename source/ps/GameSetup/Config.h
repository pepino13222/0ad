#include "ps/CStr.h"

// flag to disable extended GL extensions until fix found - specifically, crashes
// using VBOs on laptop Radeon cards
extern bool g_NoGLVBO;
// disable FBO extension in case the driver is flaky
extern bool g_NoGLFramebufferObject;
// flag to switch on shadows
extern bool g_Shadows;
// flag to switch on reflective/refractive water
extern bool g_FancyWater;
// flag to switch on fixed frame timing (RC: I'm using this for profiling purposes)
extern bool g_FixedFrameTiming;
extern bool g_VSync;
extern float g_LodBias;
extern float g_Gamma;
extern bool g_EntGraph;
// name of configured render path (depending on OpenGL extensions, this may not be
// the render path that is actually in use right now)
extern CStr g_RenderPath;


extern int g_xres, g_yres;
extern int g_bpp;
extern int g_freq;
extern bool g_Quickstart;
extern CStr g_AutostartMap;

extern CStr g_CursorName;

class CmdLineArgs;
extern void CONFIG_Init(const CmdLineArgs& args);
