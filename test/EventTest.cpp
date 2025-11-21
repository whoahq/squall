#include "EventTest.hpp"

std::deque<EventHandlerCalledWith> EventHandlerCallResults;

void STORMAPI TestEventHandler1(void* data) {
    EventHandlerTest::RegisterCall(1, data);
}

void STORMAPI TestEventHandler2(void* data) {
    EventHandlerTest::RegisterCall(2, data);
}

void STORMAPI TestEventHandler3(void* data) {
    EventHandlerTest::RegisterCall(3, data);
}

void STORMAPI TestEventHandler4(void* data) {
    EventHandlerTest::RegisterCall(4, data);
}

std::ostream& operator <<(std::ostream& os, EventHandlerCalledWith const& value) {
    os << "{ TestEventHandler" << value.handler << ", " << value.data << " }";
    return os;
}

EventHandlerCalledWithMatcher<EventHandlerCalledWith> MatchesCall(EventHandlerCalledWith arg) {
  return { arg };
}
