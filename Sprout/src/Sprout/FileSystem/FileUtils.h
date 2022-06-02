#pragma once

#include <string>

namespace Sprout
{
	class FileUtils
	{
	public:
		static std::string ReadFile(const std::string& path);
	};
}