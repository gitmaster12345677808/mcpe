#pragma once
#include "network/Packet.hpp"
#include <vector>

class CraftingPacket : public Packet {
public:
    int playerId;
    std::vector<std::vector<int>> grid; // 2D grid of item IDs
    int resultItemId;
    int resultCount;

    CraftingPacket() : playerId(0), resultItemId(0), resultCount(0) {}

    virtual void read(RakNet::BitStream* stream) override;
    virtual void write(RakNet::BitStream* stream) override;
    virtual void handle(const RakNet::RakNetGUID& guid, NetEventCallback* pCallback) override;
};
