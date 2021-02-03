/*
 * Created on 03.02.2021 by nefchen.
 */

#include <catch2/catch_test_macros.hpp>

#include "signals.hpp"


TEST_CASE("signal-slot-is-bound-by-lifetime")
{
    int count{0};
    int expected_count{20};

    comm::Signal s1;

    {
        auto lifetime{s1.connect([&count]() { count++; })};

        for (int i = 0; i < expected_count; ++i)
        {
            s1.emit();
        }
    }
    s1.emit();

    REQUIRE(count == expected_count);
}

TEST_CASE("signal-removes-expired-slots-after-emit")
{
    comm::Signal s1;

    {
        auto lifetime{s1.connect([]() {})};

        s1.emit();
        REQUIRE(s1.m_slots.size() == 1);

        s1.connect([]() {});
        REQUIRE(s1.m_slots.size() == 2);

        s1.emit();
        REQUIRE(s1.m_slots.size() == 1);
    }
    s1.emit();

    REQUIRE(s1.m_slots.size() == 0);
}

