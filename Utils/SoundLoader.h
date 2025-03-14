#ifndef SOUNDLOADER_H_INCLUDED
#define SOUNDLOADER_H_INCLUDED


#include <cstdio>

/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "RSound.h"
#include "ResourceLocation.h"


RSound* loadSound(const ResourceId& resourceId, const std::string& fileName);


#endif // SOUNDLOADER_H_INCLUDED
