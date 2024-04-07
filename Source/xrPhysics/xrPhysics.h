#pragma once
#ifdef SHIPPING
#define XRPHYSICS_API
#else
#ifdef XRPHYSICS_EXPORTS
#define XRPHYSICS_API __declspec(dllexport)
#else
#define XRPHYSICS_API __declspec(dllimport)
#endif
#endif

void XRPHYSICS_API PhysicsInitialize();
