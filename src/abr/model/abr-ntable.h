#ifndef ABR_NTABLE_H
#define ABR_NTABLE_H

#include "ns3/ipv4-route.h"
#include "ns3/ipv4.h"
#include "ns3/net-device.h"
#include "ns3/output-stream-wrapper.h"
#include "ns3/timer.h"

#include <cassert>
#include <map>
#include <stdint.h>
#include <sys/types.h>

namespace ns3
{
namespace abr
{

class NeighborTableEntry
{
  public:
    /**
     * constructor
     * \param neigbor
     * \param assocTick
     */
    NeighborTableEntry(Ipv4Address neighbor, uint32_t assocTick);
    ~NeighborTableEntry(); // 소멸자

    Ipv4Address GetNeighbor() const
    {
        return m_neighbor;
    }

    uint32_t GetAssocTick() const
    {
        return m_assocTick;
    }

    void SetAssocTick(uint32_t assocTick)
    {
        m_assocTick = assocTick;
    }

    void IncAssocTick()
    {
        m_assocTick++;
    }

  private:
    Ipv4Address m_neighbor;
    uint32_t m_assocTick;
};

class NeighborTable
{
  public:
    NeighborTable();

    bool HasNeighbor(Ipv4Address neighbor);      // 이웃인지 확인
    uint32_t GetAssocTick(Ipv4Address neighbor); // 이웃과의 tick 반환
    void InsertTick(Ipv4Address neighbor);       // tick 삽입
    bool DeleteNeighbor(Ipv4Address neighbor);   // 이웃을 벗어나면 삭제

    void Clear()
    {
        m_neighborTable.clear();
    } // 이웃 테이블 초기화

  private:
    std::map<Ipv4Address, NeighborTableEntry> m_neighborTable; // 이웃 테이블
};

} // namespace abr
} // namespace ns3

#endif