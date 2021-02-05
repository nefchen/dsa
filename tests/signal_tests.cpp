/*
 * Created on 03.02.2021 by nefchen.
 */

#include <catch2/catch_test_macros.hpp>
#include <memory>

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
        REQUIRE(s1.count_connections() == 1);

        s1.connect([]() {});
        REQUIRE(s1.count_connections() == 2);

        s1.emit();
        REQUIRE(s1.count_connections() == 1);
    }
    s1.emit();

    REQUIRE(s1.count_connections() == 0);
}

TEST_CASE("signal-connecting-inside-slot")
{
    comm::Signal s1;
    std::vector<comm::Lifetime> lifetimes;
    int count{0};

    auto slot{
        [&s1, &lifetimes, &count]() {
            count++;
            lifetimes.push_back(s1.connect([&count](){ count++; }));
        }
    };

    lifetimes.push_back(s1.connect(slot));

    s1.emit();

    REQUIRE(count == 2);
}

TEST_CASE("signal-emiting-inside-slot")
{
    comm::Signal s1;
    int count{0};

    auto slot{
        [&s1, &count]() { count++; s1.emit(); }
    };
    auto lf{s1.connect(slot)};

    s1.emit();

    REQUIRE(count == 1);
}

TEST_CASE("signal-copying-no-dispatcher")
{
    comm::Signal s1;
    comm::Signal s2;
    int count{0};

    auto lf{s1.connect([&count]() { count++; })};

    s2 = s1;
    s1.emit();
    REQUIRE(count == 1);

    s2.emit();
    REQUIRE(count == 1);

    auto s3{s1};
    s3.emit();
    REQUIRE(count == 1);
}

TEST_CASE("signal-moving-no-dispatcher")
{
    comm::Signal s1;
    comm::Signal s2;
    int count{0};

    auto lf{s1.connect([&count]() { count++; })};

    s2 = std::move(s1);
    s1.emit();
    REQUIRE(count == 0);

    s2.emit();
    REQUIRE(count == 1);

    auto s3{std::move(s2)};

    s2.emit();
    REQUIRE(count == 1);

    s3.emit();
    REQUIRE(count == 2);
}

TEST_CASE("signal-with-dispatcher")
{
    auto dispatcher{std::make_shared<comm::Dispatcher>()};
    comm::Signal s1{dispatcher};
    int count{0};

    auto lf{s1.connect([&count]() { count++; })};

    s1.emit();
    REQUIRE(count == 0);

    dispatcher->emit();
    REQUIRE(count == 1);
}

TEST_CASE("signal-emit-dispatcher-in-slot")
{
    auto dispatcher{std::make_shared<comm::Dispatcher>()};
    comm::Signal s1{dispatcher};
    int count{0};

    auto lf{
        s1.connect(
            [&count, dispatcher]() {
                count++;
                dispatcher->emit();
            }
        )
    };

    s1.emit();
    dispatcher->emit();
    REQUIRE(count == 1);
}

TEST_CASE("signal-copying-with-dispatcher")
{
    auto dispatcher{std::make_shared<comm::Dispatcher>()};
    comm::Signal s1{dispatcher};
    comm::Signal s2;
    int count{0};

    auto lf1{s1.connect([&count]() { count++; })};
    auto lf2{s2.connect([&count]() { count++; })};

    s1.emit();
    REQUIRE(count == 0);

    s2.emit();
    REQUIRE(count == 1);

    // Dispatcher is copied.
    s2 = s1;

    s2.emit();
    REQUIRE(count == 1);

    // Now dispatcher holds s1 and s2 emits.
    dispatcher->emit();
    REQUIRE(count == 3);

    auto s3{s1};
    auto lf3{s3.connect([&count]() { count++; })};
    s3.emit();
    REQUIRE(count == 3);

    // Now dispatcher holds s3.
    dispatcher->emit();
    REQUIRE(count == 4);
}

TEST_CASE("signal-moving-with-dispatcher")
{
    auto dispatcher{std::make_shared<comm::Dispatcher>()};
    comm::Signal s1{dispatcher};
    comm::Signal s2;
    int count{0};

    auto lf1{s1.connect([&count]() { count++; })};

    s1.emit();
    REQUIRE(count == 0);

    // s2 has no slots.
    s2.emit();
    REQUIRE(count == 0);

    // Dispatcher is *copied* even in move.
    s2 = std::move(s1);

    s2.emit();
    REQUIRE(count == 0);

    // Now dispatcher holds s1's slot two times.
    dispatcher->emit();
    REQUIRE(count == 2);

    auto s3{std::move(s2)};
    s3.emit();
    REQUIRE(count == 2);

    // Now dispatcher holds s3.
    dispatcher->emit();
    REQUIRE(count == 3);
}

