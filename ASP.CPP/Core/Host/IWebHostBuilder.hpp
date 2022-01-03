#pragma once

#include <memory>
#include <functional>

namespace AspCpp
{
	namespace Extensions
	{
		namespace Hosting
		{
			class IWebHost
			{
			public:
				virtual void Start() = 0;
				virtual void Stop() = 0;
			};

			class IWebHostBuilder
			{
			public:
				template<class TStartup>
				void UseStartup();
				virtual void Stop() = 0;
				virtual std::unique_ptr<IWebHostBuilder> ConfigureServices(std::function<void(std::unique_ptr<AspCpp::Extensions::Hosting::IWebHostBuilder>)> configureServices) = 0;
			};
		}
	}
}