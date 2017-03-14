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


#include "Program.h"


/*
 * Incapsulates a complete set of parameters
 * that refer to the current state of the plug-in.
 */
Program::Program()
{
    // default Program Values
    fDelayTime = .0f;
    fDelayLevel = .0f;
	fFeedback = .0f;

    vst_strncpy(name, "Init", kVstMaxProgNameLen);
}
