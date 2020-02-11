#ifndef __LIBHOOK_H__
#define __LIBHOOK_H__

#ifdef LIBHOOK_API_EXPORT
#define LIBHOOK_API __declspec(dllexport)
#else
#define LIBHOOK_API __declspec(dllimport)
#endif

typedef void(*libhook_mouse_callback)(int evt, int x, int y);

LIBHOOK_API void libhook_start();
LIBHOOK_API void libhook_stop();
LIBHOOK_API void libhook_set_mouse_callback(libhook_mouse_callback);

#endif