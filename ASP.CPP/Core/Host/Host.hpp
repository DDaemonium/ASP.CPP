#pragma once

namespace AspCpp
{
	namespace Extensions
	{
		namespace Hpsting
		{
			#include "IHostBuilder.hpp"
			#include <memory>

			class Host
			{
			public:
				virtual std::unique_ptr<AspCpp::Extensions::Hpsting::IHostBuilder> CreateDefaultBuilder() = 0;
				virtual std::unique_ptr<AspCpp::Extensions::Hpsting::IHostBuilder> CreateDefaultBuilder(int argc, char* argv[]) = 0;
			};
		}
	}
}