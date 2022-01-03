#pragma once

namespace AspCpp
{
	namespace Extensions
	{
		namespace Hosting
		{
			class IHost
			{
			public:
				virtual void Start() = 0;
				virtual void Stop() = 0;
			};
		}
	}
}


