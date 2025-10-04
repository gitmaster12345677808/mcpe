#include "CraftingPacket.hpp"

void CraftingPacket::handle(const RakNet::RakNetGUID& guid, NetEventCallback* pCallback)
{
    pCallback->handle(guid, this);
}

void CraftingPacket::read(RakNet::BitStream* stream) {
    stream->Read(playerId);
    int rows, cols;
    stream->Read(rows);
    stream->Read(cols);
    grid.resize(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            stream->Read(grid[i][j]);
    stream->Read(resultItemId);
    stream->Read(resultCount);
}

void CraftingPacket::write(RakNet::BitStream* stream) {
    stream->Write((uint8_t)PACKET_CRAFTING);
    stream->Write(playerId);
    stream->Write((int)grid.size());
    stream->Write(grid.empty() ? 0 : (int)grid[0].size());
    for (const auto& row : grid)
        for (int id : row)
            stream->Write(id);
    stream->Write(resultItemId);
    stream->Write(resultCount);
}
