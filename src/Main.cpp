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


#include "GosDelay.h"


/*
 * Called by the host to create plug-in instance.
 */
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new GosDelay(audioMaster);
}
