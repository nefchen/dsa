/*
 * Created on 12.02.2021 by nefchen.
 */

#include <catch2/catch_test_macros.hpp>

#include "widgets/widget.hpp"
#include "comm/comm.hpp"
#include "views/view.hpp"
#include "types/basic.hpp"
#include "types/input.hpp"


TEST_CASE("widget-copy-assign")
{
    view::Widget w1{nullptr};
    {
        view::Widget w2{nullptr};

        w2.m_lifetimes.push_back(
            comm::unsafe::bind_autodelete_lifetime(
                w2.m_hovered.connect([](Point p, input::MouseHover h){}),
                w2.m_hovered
            )
        );

        REQUIRE(w2.m_lifetimes.size() == 1);

        // Copy w2 to w1.
        w1 = w2;

        REQUIRE(w2.m_lifetimes.size() == 1);
    }

    // AutodeleteLifetimes *must not* be copied as their
    // inner signal is already discarded.
    REQUIRE(w1.m_lifetimes.size() == 0);
}

TEST_CASE("widget-copy-construct")
{
    view::Widget w2{nullptr};

    w2.m_lifetimes.push_back(
        comm::unsafe::bind_autodelete_lifetime(
            w2.m_hovered.connect([](Point p, input::MouseHover h){}),
            w2.m_hovered
        )
    );

    REQUIRE(w2.m_lifetimes.size() == 1);

    // Copy w2 to w1.
    auto w1{w2};

    REQUIRE(w2.m_lifetimes.size() == 1);

    // AutodeleteLifetimes *must not* be copied as their
    // inner signal is already discarded.
    REQUIRE(w1.m_lifetimes.size() == 0);
}

TEST_CASE("widget-move-assign")
{
    view::Widget w1{nullptr};

    REQUIRE(w1.m_lifetimes.size() == 0);

    {
        view::Widget w2{nullptr};

        w2.m_lifetimes.push_back(
            comm::unsafe::bind_autodelete_lifetime(
                w2.m_hovered.connect([](Point p, input::MouseHover h){}),
                w2.m_hovered
            )
        );

        REQUIRE(w2.m_lifetimes.size() == 1);

        // Copy w2 to w1.
        w1 = std::move(w2);

        REQUIRE(w2.m_lifetimes.size() == 0);
    }

    // AutodeleteLifetimes *must* be moved.
    REQUIRE(w1.m_lifetimes.size() == 1);

    // Test for crash.
    for (auto& lifetime_delete : w1.m_lifetimes)
    {
        lifetime_delete();
    }
}

TEST_CASE("widget-move-construct")
{
    view::Widget w2{nullptr};

    w2.m_lifetimes.push_back(
        comm::unsafe::bind_autodelete_lifetime(
            w2.m_hovered.connect([](Point p, input::MouseHover h){}),
            w2.m_hovered
        )
    );

    REQUIRE(w2.m_lifetimes.size() == 1);

    // Copy w2 to w1.
    auto w1{std::move(w2)};

    REQUIRE(w2.m_lifetimes.size() == 0);

    // AutodeleteLifetimes *must* be moved.
    REQUIRE(w1.m_lifetimes.size() == 1);

    // Test for crash.
    for (auto& lifetime_delete : w1.m_lifetimes)
    {
        lifetime_delete();
    }
}

