#include "contra/packet.hpp"

namespace contra {

std::vector<uint8_t> SerializePacket(const Packet&) { return {}; }
Packet DeserializePacket(const std::vector<uint8_t>&) { return {}; }

}  // namespace contra
