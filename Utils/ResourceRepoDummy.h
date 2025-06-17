#ifndef RESOURCEREPODUMMY_H_INCLUDED
#define RESOURCEREPODUMMY_H_INCLUDED

#include "ResourceRepo.h"
#include "RSound.h"

/**
 * Provides dummy resources, to be used in case lack of
 * resource cannot be handled.
 */
class ResourceRepoDummy : public ResourceRepo {
    std::string getPropertiesString() const override { return "type:dummy"; }

public:
    /**
     * ResourceRepoDummy constructor
     */
    explicit ResourceRepoDummy()
    : ResourceRepo("VBCPP DUMMY RESOURCES")
    {}

    ~ResourceRepoDummy() override {}

private:
    std::unique_ptr<RSound> loadSound(const ResourceId& resourceId) override {
        return std::make_unique<RSound>(resourceId, ALuint(0u));
    }
};

#endif // RESOURCEREPODUMMY_H_INCLUDED