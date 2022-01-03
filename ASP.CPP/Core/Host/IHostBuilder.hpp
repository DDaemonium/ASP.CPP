#pragma once

#include "IHost.hpp"
#include "IWebHostBuilder.hpp"
#include <memory>
#include <functional>

namespace AspCpp
{
	namespace Extensions
	{
		namespace Hosting
		{
			class IHostBuilder
			{
			public:
				virtual std::unique_ptr<AspCpp::Extensions::Hosting::IHost> Build() = 0;
				virtual std::unique_ptr<IHostBuilder> ConfigureWebHostDefaults(std::function<void(std::unique_ptr<AspCpp::Extensions::Hosting::IWebHostBuilder>)> configure) = 0;
			};
		}
	}
}


