#ifndef RSOUND_H_INCLUDED
#define RSOUND_H_INCLUDED


/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "Resource.h"
#include "ResourceId.h"


class RSound : public Resource
{
    private:
        ALuint _buffer;

    public:
        RSound(const ResourceId& resourceId, ALuint buffer)
            : Resource(RT_SOUND, resourceId),
            _buffer(buffer)
        {

        }

        ~RSound()
        {
            alDeleteBuffers(1, &_buffer);
        }

        ALuint getBuffer()
        {
            return _buffer;
        }

};


#endif // RSOUND_H_INCLUDED
