#pragma once

#define DEBUG_MODE 1

#ifdef PLATFORM_WINDOWS
	#ifdef T_DLL
		#ifdef BUILD_MY_DLL
			#define T_API __declspec(dllexport)
			#define IMGUI_API __declspec(dllexport)
		#else 
			#define T_API __declspec(dllimport)
			#define IMGUI_API __declspec(dllimport)
		#endif
	#else
		#ifdef BUILD_MY_DLL
			#define T_API
			#define IMGUI_API
		#else 
			#define T_API
			#define IMGUI_API
		#endif
	#endif
#endif

#define NANO_TO_SEC deltatime/1000000000.0f
#define DEFAULT_WHITE_PIC "asset/pic/white.png"
#define AUTOSAVE_PATH "asset/scene/autosave"
#define BIT(x) (1<<x)
#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)


// keycode, from glfw
/* Printable keys */
#define T_KEY_SPACE              32
#define T_KEY_APOSTROPHE         39  /* ' */
#define T_KEY_COMMA              44  /* , */
#define T_KEY_MINUS              45  /* - */
#define T_KEY_PERIOD             46  /* . */
#define T_KEY_SLASH              47  /* / */
#define T_KEY_0                  48
#define T_KEY_1                  49
#define T_KEY_2                  50
#define T_KEY_3                  51
#define T_KEY_4                  52
#define T_KEY_5                  53
#define T_KEY_6                  54
#define T_KEY_7                  55
#define T_KEY_8                  56
#define T_KEY_9                  57
#define T_KEY_SEMICOLON          59  /* ; */
#define T_KEY_EQUAL              61  /* = */
#define T_KEY_A                  65
#define T_KEY_B                  66
#define T_KEY_C                  67
#define T_KEY_D                  68
#define T_KEY_E                  69
#define T_KEY_F                  70
#define T_KEY_G                  71
#define T_KEY_H                  72
#define T_KEY_I                  73
#define T_KEY_J                  74
#define T_KEY_K                  75
#define T_KEY_L                  76
#define T_KEY_M                  77
#define T_KEY_N                  78
#define T_KEY_O                  79
#define T_KEY_P                  80
#define T_KEY_Q                  81
#define T_KEY_R                  82
#define T_KEY_S                  83
#define T_KEY_T                  84
#define T_KEY_U                  85
#define T_KEY_V                  86
#define T_KEY_W                  87
#define T_KEY_X                  88
#define T_KEY_Y                  89
#define T_KEY_Z                  90
#define T_KEY_LEFT_BRACKET       91  /* [ */
#define T_KEY_BACKSLASH          92  /* \ */
#define T_KEY_RIGHT_BRACKET      93  /* ] */
#define T_KEY_GRAVE_ACCENT       96  /* ` */
#define T_KEY_WORLD_1            161 /* non-US #1 */
#define T_KEY_WORLD_2            162 /* non-US #2 */
/* Function keys */
#define T_KEY_ESCAPE             256
#define T_KEY_ENTER              257
#define T_KEY_TAB                258
#define T_KEY_BACKSPACE          259
#define T_KEY_INSERT             260
#define T_KEY_DELETE             261
#define T_KEY_RIGHT              262
#define T_KEY_LEFT               263
#define T_KEY_DOWN               264
#define T_KEY_UP                 265
#define T_KEY_PAGE_UP            266
#define T_KEY_PAGE_DOWN          267
#define T_KEY_HOME               268
#define T_KEY_END                269
#define T_KEY_CAPS_LOCK          280
#define T_KEY_SCROLL_LOCK        281
#define T_KEY_NUM_LOCK           282
#define T_KEY_PRINT_SCREEN       283
#define T_KEY_PAUSE              284
#define T_KEY_F1                 290
#define T_KEY_F2                 291
#define T_KEY_F3                 292
#define T_KEY_F4                 293
#define T_KEY_F5                 294
#define T_KEY_F6                 295
#define T_KEY_F7                 296
#define T_KEY_F8                 297
#define T_KEY_F9                 298
#define T_KEY_F10                299
#define T_KEY_F11                300
#define T_KEY_F12                301
#define T_KEY_F13                302
#define T_KEY_F14                303
#define T_KEY_F15                304
#define T_KEY_F16                305
#define T_KEY_F17                306
#define T_KEY_F18                307
#define T_KEY_F19                308
#define T_KEY_F20                309
#define T_KEY_F21                310
#define T_KEY_F22                311
#define T_KEY_F23                312
#define T_KEY_F24                313
#define T_KEY_F25                314
#define T_KEY_KP_0               320
#define T_KEY_KP_1               321
#define T_KEY_KP_2               322
#define T_KEY_KP_3               323
#define T_KEY_KP_4               324
#define T_KEY_KP_5               325
#define T_KEY_KP_6               326
#define T_KEY_KP_7               327
#define T_KEY_KP_8               328
#define T_KEY_KP_9               329
#define T_KEY_KP_DECIMAL         330
#define T_KEY_KP_DIVIDE          331
#define T_KEY_KP_MULTIPLY        332
#define T_KEY_KP_SUBTRACT        333
#define T_KEY_KP_ADD             334
#define T_KEY_KP_ENTER           335
#define T_KEY_KP_EQUAL           336
#define T_KEY_LEFT_SHIFT         340
#define T_KEY_LEFT_CONTROL       341
#define T_KEY_LEFT_ALT           342
#define T_KEY_LEFT_SUPER         343
#define T_KEY_RIGHT_SHIFT        344
#define T_KEY_RIGHT_CONTROL      345
#define T_KEY_RIGHT_ALT          346
#define T_KEY_RIGHT_SUPER        347
#define T_KEY_MENU               348

// mouse button code, also form glfw
#define T_MOUSE_BUTTON_1         0
#define T_MOUSE_BUTTON_2         1
#define T_MOUSE_BUTTON_3         2
#define T_MOUSE_BUTTON_4         3
#define T_MOUSE_BUTTON_5         4
#define T_MOUSE_BUTTON_6         5
#define T_MOUSE_BUTTON_7         6
#define T_MOUSE_BUTTON_8         7
#define T_MOUSE_BUTTON_LAST      T_MOUSE_BUTTON_8
#define T_MOUSE_BUTTON_LEFT      T_MOUSE_BUTTON_1
#define T_MOUSE_BUTTON_RIGHT     T_MOUSE_BUTTON_2
#define T_MOUSE_BUTTON_MIDDLE    T_MOUSE_BUTTON_3