#include <chrono>
#include "lock.h"

mlock::CLock::CLock(uint64_t count)
{
    m_count = count;
}

void mlock::CLock::await(uint64_t nanosecs)
{
    std::unique_lock<std::mutex> lck(m_lock);
    if (0 == m_count){
        return;
    }
    if (nanosecs > 0) {
        m_cv.wait_for(lck, std::chrono::nanoseconds(nanosecs));
    } else {
        m_cv.wait(lck);
    }
}

uint64_t mlock::CLock::get_count()
{
    std::unique_lock<std::mutex> lck(m_lock);
    return m_count;
}

uint64_t mlock::CLock::set_count(uint64_t count )
{
    std::unique_lock<std::mutex> lck(m_lock);
    uint64_t res = m_count;
    m_count = count;
    if (m_count == 0 )
    {
        m_cv.notify_all();
    }
    return res;
}

uint64_t mlock::CLock::clear_count( )
{
    std::unique_lock<std::mutex> lck(m_lock);
    uint64_t res = m_count;
    if (m_count != 0 )
    {
        m_count = 0;
        m_cv.notify_all();
    }
    return res;
}

void mlock::CLock::count_down()
{
    std::unique_lock<std::mutex> lck(m_lock);
    if (0 == m_count)
    {
        return;
    }
    --m_count;
    if (0 == m_count)
    {
        m_cv.notify_all();
    }
}
