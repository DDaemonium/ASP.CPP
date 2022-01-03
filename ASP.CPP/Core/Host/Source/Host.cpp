#include "../Host.hpp"
#include <memory>

namespace AspCpp 
{
	namespace Extensions
	{
		namespace Hosting
		{
			std::unique_ptr<AspCpp::Extensions::Hosting::IHostBuilder> Host::CreateDefaultBuilder()
			{
				return nullptr;
			}

			std::unique_ptr<AspCpp::Extensions::Hosting::IHostBuilder> Host::CreateDefaultBuilder(int argc, char* argv[])
			{
				return nullptr;
			}
		}
	}
}