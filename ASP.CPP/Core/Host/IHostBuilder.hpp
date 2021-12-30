#pragma once

namespace AspCpp
{
	namespace Extensions
	{
		namespace Hpsting
		{
			class IHostBuilder
			{
			public:
				virtual IHostBuilder CreateDefaultBuilder() = 0;
				virtual IHostBuilder CreateDefaultBuilder(int argc, char* argv[]) = 0;
			};

			class Host
			{
			public:
				virtual IHostBuilder CreateDefaultBuilder() = 0;
				virtual IHostBuilder CreateDefaultBuilder(int argc, char* argv[]) = 0;
			};
		}
	}
}


