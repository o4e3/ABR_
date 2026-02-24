#include "abr-ntable.h"

#include <limits>

namespace ns3
{
namespace abr
{

NeighborTableEntry::NeighborTableEntry(Ipv4Address neighbor, uint32_t assocTick)
    : m_neighbor(neighbor),
      m_assocTick(assocTick)
{
}

NeighborTableEntry::~NeighborTableEntry() = default;

NeighborTable::NeighborTable() = default;

bool
NeighborTable::HasNeighbor(Ipv4Address neighbor)
{
    return m_neighborTable.find(neighbor) != m_neighborTable.end();
}

uint32_t
NeighborTable::GetAssocTick(Ipv4Address neighbor)
{
    auto it = m_neighborTable.find(neighbor);
    if (it == m_neighborTable.end())
    {
        return 0; // unknown neighbor
    }
    return it->second.GetAssocTick();
}

void
NeighborTable::InsertTick(Ipv4Address neighbor)
{
    auto it = m_neighborTable.find(neighbor);

    // 처음 본 이웃이면 tick=1로 시작
    if (it == m_neighborTable.end())
    {
        m_neighborTable.emplace(neighbor, NeighborTableEntry(neighbor, 1));
        return;
    }

    // 이미 있으면 tick++
    uint32_t t = it->second.GetAssocTick();
    if (t != std::numeric_limits<uint32_t>::max())
    {
        it->second.SetAssocTick(t + 1);
    }
}

bool
NeighborTable::DeleteNeighbor(Ipv4Address neighbor)
{
    return m_neighborTable.erase(neighbor) > 0;
}

} // namespace abr
} // namespace ns3