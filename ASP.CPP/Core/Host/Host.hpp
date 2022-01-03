#pragma once

#include "IHostBuilder.hpp"
#include <memory>

namespace AspCpp
{
	namespace Extensions
	{
		namespace Hosting
		{
			class Host
			{
			public:
				static std::unique_ptr<AspCpp::Extensions::Hosting::IHostBuilder> CreateDefaultBuilder();
				static std::unique_ptr<AspCpp::Extensions::Hosting::IHostBuilder> CreateDefaultBuilder(int argc, char* argv[]);
			};
		}
	}
}