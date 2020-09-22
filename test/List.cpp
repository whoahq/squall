#include "List.hpp"
#include "Test.hpp"

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

        delete node;
    }

    SECTION("links node to tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node = new TestListNode();
        list.LinkToTail(node);

        REQUIRE(list.Tail() == node);

        delete node;
    }

    SECTION("links nodes to head and tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToHead(node1);

        auto node2 = new TestListNode();
        list.LinkToTail(node2);

        REQUIRE(list.Head() == node1);
        REQUIRE(list.Tail() == node2);

        delete node1;
        delete node2;
    }

    SECTION("links multiple nodes to head correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToHead(node1);

        auto node2 = new TestListNode();
        list.LinkToHead(node2);

        REQUIRE(list.Head() == node2);
        REQUIRE(list.Tail() == node1);

        delete node1;
        delete node2;
    }
}
