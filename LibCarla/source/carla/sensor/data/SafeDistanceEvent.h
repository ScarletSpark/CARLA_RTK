#pragma once

#include "carla/rpc/ActorId.h"
#include "carla/sensor/data/Array.h"
#include "carla/sensor/SensorData.h"

namespace carla {
namespace sensor {
namespace data {

	class SafeDistanceEvent : public Array<rpc::ActorId> {
		public:

		explicit SafeDistanceEvent(RawData &&data) : Array<rpc::ActorId>(data.size(),std::move(data)) {}
	};

} // namespace data
} // namespace sensor
} // namespace carla