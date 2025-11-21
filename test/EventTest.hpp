#include "test/Test.hpp"
#include "storm/Core.hpp"
#include "storm/Event.hpp"

#include <deque>
#include <sstream>

struct EventHandlerCalledWith {
    int handler;
    void* data;
};

extern std::deque<EventHandlerCalledWith> EventHandlerCallResults;

struct EventHandlerTest {
    EventHandlerTest() {
        EventHandlerCallResults.clear();
        StormDestroy();
    }

    static void RegisterCall(int handler, void* data) {
        EventHandlerCalledWith calledWith = {handler, data};
        EventHandlerCallResults.push_back(calledWith);
    }

    static size_t NumCalls() {
        return EventHandlerCallResults.size();
    }

    static EventHandlerCalledWith CallResult() {
        if (EventHandlerCallResults.empty()) {
            return {-1, nullptr};
        }

        EventHandlerCalledWith result = EventHandlerCallResults.front();
        EventHandlerCallResults.pop_front();
        return result;
    }
};

void STORMAPI TestEventHandler1(void* data);
void STORMAPI TestEventHandler2(void* data);
void STORMAPI TestEventHandler3(void* data);
void STORMAPI TestEventHandler4(void* data);

// Helpers for comparing EventHandlerCalledWith structs
std::ostream& operator <<(std::ostream& os, EventHandlerCalledWith const& value);

template <class T>
class EventHandlerCalledWithMatcher : public Catch::MatcherBase<T> {
private:
  T cmp;

public:
  EventHandlerCalledWithMatcher(T arg) : cmp(arg) {}

  bool match(T const& in) const override {
    return cmp.handler == in.handler && cmp.data == in.data;
  }

  std::string describe() const override {
    std::ostringstream ss;
    ss << "equals " << cmp;
    return ss.str();
  }
};

EventHandlerCalledWithMatcher<EventHandlerCalledWith> MatchesCall(EventHandlerCalledWith arg);
