#include "SoundLoader.h"


RSound* loadSound(const ResourceId& resourceId, const std::string& fileName)
{
    ALuint buffer = alutCreateBufferFromFile(fileName.c_str());

    //std::cout << "??? SoundComponend - Constructor" << std::endl;
    //std::cout << "Buffer" << _buffer << std::endl;

    if (buffer == AL_NONE)
    {
        ALenum error = alutGetError ();
        LOG_ERROR("Error loading file: " + std::string(alutGetErrorString(error)));
    }

    RSound* sound = new RSound(resourceId, buffer);
    return sound;
}
