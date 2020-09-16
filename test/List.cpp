#include "Test.hpp"
#include "List.hpp"

struct TestListNode : TSLinkedNode<TestListNode> {
    uint32_t index = 0;
};

TEST_CASE("TSList", "[list]") {
    SECTION("constructs correctly") {
        STORM_LIST(TestListNode) list;
        REQUIRE(list.Head() == nullptr);
    }
}

TEST_CASE("TSList::LinkToHead", "[list]") {
    SECTION("links node to head correctly") {
        STORM_LIST(TestListNode) list;

        auto node = new TestListNode();
        list.LinkToHead(node);

        REQUIRE(list.Head() == node);

        delete(node);
    }
}
