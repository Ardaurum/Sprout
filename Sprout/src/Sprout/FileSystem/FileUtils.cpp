#include "spch.h"
#include "FileUtils.h"
#include <fstream>

namespace Sprout
{
	std::string FileUtils::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				SPROUT_CORE_ERROR("Could not read from file at {0}!", path);
			}
		}
		else
		{
			SPROUT_CORE_ERROR("Failed loading file at: {0}!", path);
		}

		return result;
	}
}