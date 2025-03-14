#ifndef ROADPROFILELOADER_H_INCLUDED
#define ROADPROFILELOADER_H_INCLUDED


#include "RRoadProfile.h"


#include <glm/glm.hpp>


namespace tinyxml2
{
	class XMLElement;
}


class RoadProfileLoader
{
	static constexpr const char* PROFILE_FILE_NAME = "profile.xml";

	private:

	public:
		static RRoadProfile* loadRoadProfile(const ResourceId& resourceId, const std::string& fileName);

};


#endif // ROADPROFILELOADER_H_INCLUDED
