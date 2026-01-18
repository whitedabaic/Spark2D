#pragma once
#include <string>

namespace SPARK_CORE::ECS
{
	struct Identification 
	{
		std::string name{ "GameObject" }, group{ "Default" };
		int32_t entity_id{ -1 };
	};
}