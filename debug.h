
#ifndef __DEBUG_H__
#define __DEBUG_H__

#define IS_DEBUG        1

#if IS_DEBUG
#define DMSG(msg)       Serial.print(msg)
#else
#define DMSG(msg)
#endif

#endif

