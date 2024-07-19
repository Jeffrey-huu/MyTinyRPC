#ifndef ROCKET_NET_TIMER_H
#define ROCKET_NET_TIMER_H

#include <map>
#include "src/common/mutex.h"
#include "src/net/fd_event.h"
#include "src/net/timer_event.h"

namespace MyTinyRPC {
    class Timer : public FdEvent {
        public:
            Timer();
            ~Timer();

            void addTimerEvent(TimerEvent::s_ptr event);
            void deleteTimerEvent(TimerEvent::s_ptr event);
            void onTimer(); // 当发送了 IO 事件后，eventloop 会执行这个回调函数

        private:
            void setHeadArriveTime();
            std::multimap<int64_t, TimerEvent::s_ptr> m_pending_events; // arrive_time : time_event
            Mutex m_mutex;
    };
}
#endif