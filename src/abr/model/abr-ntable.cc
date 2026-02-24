#include "abr-ntable.h"

#include <limits>

namespace ns3
{
namespace abr
{

NeighborTableEntry::NeighborTableEntry(Ipv4Address neighbor, uint32_t assocTick)
    : m_neighbor(neighbor),
      m_assocTick(assocTick),
      m_lastSeen(Seconds(0))
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

// last seen 기록
void
NeighborTable::NoteNeighbor(Ipv4Address neighbor)
{
    auto it = m_neighborTable.find(neighbor);
    if (it == m_neighborTable.end())
    {
        NeighborTableEntry e(neighbor, 0);
        e.SetLastSeen(Simulator::Now());
        m_neighborTable.emplace(neighbor, e);
        return;
    }
    it->second.SetLastSeen(Simulator::Now());
}

// Timer 기반 tick 증가
void
NeighborTable::IncreaseTick(Ipv4Address neighbor)
{
    auto it = m_neighborTable.find(neighbor);
    if (it == m_neighborTable.end())
    {
        return;
    }

    uint32_t t = it->second.GetAssocTick();
    if (t != std::numeric_limits<uint32_t>::max())
    {
        it->second.SetAssocTick(t + 1);
    }
}

// 일정시간이상 Hello 미수신시 해당 이웃 삭제
void
NeighborTable::Purge(Time expire)
{
    Time now = Simulator::Now();
    for (auto it = m_neighborTable.begin(); it != m_neighborTable.end();)
    {
        if (now - it->second.GetLastSeen() > expire)
        {
            it = m_neighborTable.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool
NeighborTable::DeleteNeighbor(Ipv4Address neighbor)
{
    return m_neighborTable.erase(neighbor) > 0;
}

std::vector<std::pair<Ipv4Address, uint32_t>>
NeighborTable::GetAllNeighbors() const
{
    std::vector<std::pair<Ipv4Address, uint32_t>> neighbors;
    for (const auto& entry : m_neighborTable)
    {
        neighbors.push_back(std::make_pair(entry.first, entry.second.GetAssocTick()));
    }
    return neighbors;
}

void
NeighborTable::Print(std::ostream& os) const
{
    os << "NeighborTable\n";
    os << "Neighbor\tAssocTick\n";
    for (const auto& entry : m_neighborTable)
    {
        os << entry.first << "\t" << entry.second.GetAssocTick() << "\n";
    }
}

Time
NeighborTable::GetLastSeen(Ipv4Address neighbor) const
{
    auto it = m_neighborTable.find(neighbor);
    if (it == m_neighborTable.end())
    {
        return Seconds(0);
    }
    return it->second.GetLastSeen();
}

} // namespace abr
} // namespace ns3