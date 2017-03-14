/*
 * GoS'95  gosDelay
 *   ________        _________________.________
 *  /  _____/  ____ /   _____/   __   \   ____/
 * /   \  ___ /  _ \\_____  \\____    /____  \
 * \    \_\  (  <_> )        \  /    //       \
 *  \______  /\____/_______  / /____//______  /
 *         \/              \/               \/
 * Copyright (c) 2009 by Alessandro Fraschetti
 * (gos95@gommagomma.net). All Rights Reserved.
 *
 * gosDelay:
 *
 * For more information about gosDelay visit:
 *     http://gommagomma.net/gos95/vst
 * This code comes with ABSOLUTELY NO WARRANTY.
 */


#ifndef __PROGRAM_H__
#define __PROGRAM_H__


#include "audioeffectx.h"


/*
 * enumerates all parameters that are accessible to the user.
 */
enum
{
    // Global
    kNumPrograms = 7,

    // Parameters Tags
    kDelayTime = 0,
	kDelayLevel,
	kFeedback,

    kNumParams
};


/*
 * Incapsulates a complete set of parameters
 * that refer to the current state of the plug-in.
 */
class Program
{
friend class GosDelay;
public:
    Program();
    ~Program() {}

private:
    float fDelayTime;
    float fDelayLevel;
	float fFeedback;
    char name[kVstMaxProgNameLen+1];
};


#endif /* !defined(__PROGRAM_H__) */
