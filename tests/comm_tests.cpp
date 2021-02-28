/*
 * Created on 12.02.2021 by nefchen.
 */

#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "comm/comm.hpp"
#include "views/view.hpp"
#include "types/basic.hpp"


TEST_CASE("using-autodelete-lifetime")
{
    // Lifetimes need to be disconnected with the specific
    // signal, this micro-management is tedious when many
    // signals are used.

    comm::Signal s1;
    comm::Signal<u8> s2;
    comm::Signal<double> s3;
    comm::Signal<i16> s4;

    {
        auto lf1{s1.connect([](){})};
        auto lf2{s2.connect([](u8 p){})};
        auto lf3{s3.connect([](double p){})};
        auto lf4{s4.connect([](i16 p){})};

        // Explicit disconnect.
        s1.disconnect(std::move(lf1));
        s2.disconnect(std::move(lf2));
        s3.disconnect(std::move(lf3));
        s4.disconnect(std::move(lf4));

        REQUIRE(s1.count_connections() == 0);
        REQUIRE(s2.count_connections() == 0);
        REQUIRE(s3.count_connections() == 0);
        REQUIRE(s4.count_connections() == 0);
    }

    auto lf1{comm::bind_autodelete_lifetime(s1.connect([](){}), s1)};
    auto lf2{comm::bind_autodelete_lifetime(s2.connect([](u8 p){}), s2)};
    auto lf3{comm::bind_autodelete_lifetime(s3.connect([](double p){}), s3)};
    auto lf4{comm::bind_autodelete_lifetime(s4.connect([](i16 p){}), s4)};

    // Implicit disconnect.
    lf1();
    lf2();
    lf3();
    lf4();

    REQUIRE(s1.count_connections() == 0);
    REQUIRE(s2.count_connections() == 0);
    REQUIRE(s3.count_connections() == 0);
    REQUIRE(s4.count_connections() == 0);
}

