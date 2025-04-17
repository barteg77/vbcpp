#include "SoundLoader.h"


RSound* loadSound(const ResourceLocation& resourceLocation)
{
    ALuint buffer = alutCreateBufferFromFile(resourceLocation.getPath().c_str());

    //std::cout << "??? SoundComponend - Constructor" << std::endl;
    //std::cout << "Buffer" << _buffer << std::endl;

    if (buffer == AL_NONE)
    {
        ALenum error = alutGetError ();
        LOG_ERROR("Error loading file: " + std::string(alutGetErrorString(error)));
    }

    RSound* sound = new RSound(resourceLocation.getResourceId(), buffer);
    return sound;
}
