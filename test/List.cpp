#include "storm/List.hpp"
#include "test/Test.hpp"

struct TestListNode : TSLinkedNode<TestListNode> {
    uint32_t index = 0;
};

struct TestExplicitListNode {
    uint32_t index = 0;
    TSLink<TestExplicitListNode> m_explicitLink;
};

TEST_CASE("TSList", "[list]") {
    SECTION("constructs correctly") {
        STORM_LIST(TestListNode) list;
        REQUIRE(list.Head() == nullptr);
    }

    SECTION("constructs copy correctly") {
        STORM_LIST(TestListNode) list;
        auto node = new TestListNode();
        list.LinkToHead(node);

        STORM_LIST(TestListNode) listCopy(list);

        // List copy constructor does not transfer ownership of nodes. It merely copies the link
        // offset from the source list and initializes the list terminator.
        REQUIRE(list.Head() == node);
        REQUIRE(listCopy.Head() == nullptr);
    }
}

TEST_CASE("TSList::Head", "[list]") {
    SECTION("returns nullptr for uninitialized list") {
        STORM_LIST(TestListNode) list;
        REQUIRE(list.Head() == nullptr);
    }
}

TEST_CASE("TSList::IsEmpty", "[list]") {
    SECTION("returns true if just initialized") {
        STORM_LIST(TestListNode) list;
        CHECK(list.IsEmpty());
    }

    SECTION("returns false if there are linked items") {
        STORM_LIST(TestListNode) list;
        list.NewNode(STORM_LIST_TAIL, 0, 0);
        CHECK_FALSE(list.IsEmpty());
    }

    SECTION("returns true after clearing a populated list") {
        STORM_LIST(TestListNode) list;
        list.NewNode(STORM_LIST_TAIL, 0, 0);
        list.Clear();
        CHECK(list.IsEmpty());
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

    SECTION("links node to head after linking node to tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToTail(node1);

        auto node2 = new TestListNode();
        list.LinkToHead(node2);

        REQUIRE(list.Head() == node2);
        REQUIRE(list.Tail() == node1);

        delete node1;
        delete node2;
    }
}

TEST_CASE("TSList::LinkToTail", "[list]") {
    SECTION("links node to tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node = new TestListNode();
        list.LinkToTail(node);

        REQUIRE(list.Tail() == node);

        delete node;
    }

    SECTION("links multiple nodes to tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToTail(node1);

        auto node2 = new TestListNode();
        list.LinkToTail(node2);

        REQUIRE(list.Head() == node1);
        REQUIRE(list.Tail() == node2);

        delete node1;
        delete node2;
    }

    SECTION("links node to tail after linking node to head correctly") {
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
}

TEST_CASE("TSList::Tail", "[list]") {
    SECTION("returns nullptr for uninitialized list") {
        STORM_LIST(TestListNode) list;
        REQUIRE(list.Tail() == nullptr);
    }
}

TEST_CASE("TSExplicitList", "[list]") {
    SECTION("constructs correctly") {
        STORM_EXPLICIT_LIST(TestExplicitListNode, m_explicitLink) list;
        REQUIRE(list.Head() == nullptr);
    }

    SECTION("constructs copy correctly") {
        STORM_EXPLICIT_LIST(TestExplicitListNode, m_explicitLink) list;
        auto node = new TestExplicitListNode();
        list.LinkToHead(node);

        STORM_EXPLICIT_LIST(TestExplicitListNode, m_explicitLink) listCopy(list);

        // Explicit list copy constructor does not transfer ownership of nodes. It merely calls
        // the copy constructor on the list base class, which only copies the link offset from
        // the source list and initializes the list terminator.
        REQUIRE(list.Head() == node);
        REQUIRE(listCopy.Head() == nullptr);
    }
}
