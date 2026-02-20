#include "abr-ntable.h"

namespace ns3
{
namespace abr
{

NeighborTableEntry::NeighborTableEntry(Ipv4Address neighbor, uint32_t assocTick)
    : m_neighbor(neighbor),
      m_assocTick(assocTick)
{
}

NeighborTableEntry::~NeighborTableEntry()
{
}

NeighborTable::NeighborTable()
{
}

bool
NeighborTable::HasNeighbor(Ipv4Address neighbor)
{
    return m_neighborTable.find(neighbor) != m_neighborTable.end();
}

uint32_t
NeighborTable::GetAssocTick(Ipv4Address neighbor)
{
    // TODO
}

void
NeighborTable::InsertTick(Ipv4Address neighbor)
{
    // TODO
}

bool
DeleteNeighbor(Ipv4Address neighbor)
{
    // TODO
}

} // namespace abr
} // namespace ns3