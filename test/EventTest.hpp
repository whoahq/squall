#include "test/Test.hpp"
#include "storm/Event.hpp"

#include <deque>
#include <sstream>


void SEvtCleanExtraDataForTests();


struct EventHandlerCalledWith {
    int handler;
    void* data;
};

static std::deque<EventHandlerCalledWith> EventHandlerCallResults;

struct EventHandlerTest {
    EventHandlerTest() {
        EventHandlerCallResults.clear();
        SEvtDestroy();
        SEvtCleanExtraDataForTests();
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


static void STORMAPI TestEventHandler1(void* data) {
    EventHandlerTest::RegisterCall(1, data);
}

static void STORMAPI TestEventHandler2(void* data) {
    EventHandlerTest::RegisterCall(2, data);
}

static void STORMAPI TestEventHandler3(void* data) {
    EventHandlerTest::RegisterCall(3, data);
}

static void STORMAPI TestEventHandler4(void* data) {
    EventHandlerTest::RegisterCall(4, data);
}


// Helpers for comparing EventHandlerCalledWith structs
std::ostream& operator <<(std::ostream& os, EventHandlerCalledWith const& value) {
    os << "{ TestEventHandler" << value.handler << ", " << value.data << " }";
    return os;
}

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

EventHandlerCalledWithMatcher<EventHandlerCalledWith> MatchesCall(EventHandlerCalledWith arg) {
  return { arg };
}
