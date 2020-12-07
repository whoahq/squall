#include "storm/Queue.hpp"
#include "test/Test.hpp"

struct TestPriority : TSTimerPriority<uint32_t> {
};

struct TestElement {
    TestPriority m_priority;
};

TEST_CASE("TSPriorityQueue", "[queue]") {
    SECTION("constructs correctly") {
        auto queue = TSPriorityQueue<TestElement>(offsetof(TestElement, m_priority));
        REQUIRE(queue.Dequeue() == nullptr);
    }
}
