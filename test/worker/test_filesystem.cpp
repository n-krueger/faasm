#include <catch/catch.hpp>

#include "utils.h"

#include <util/func.h>
#include <util/config.h>
#include <util/strings.h>


namespace tests {
    TEST_CASE("Test gedents", "[worker]") {
        cleanSystem();

        // Note, our test function adds an extra comma, hence the blank
        std::vector<std::string> expected = {
                "",  "..",
                "etc", "funcs", "include", "lib",
                "libfakeLibA.so", "libfakeLibA.so.o",
                "libfakeiLibA.wast", "libfakeiLibB.wast",
                "share"
        };

        util::SystemConfig &conf = util::getSystemConfig();
        conf.unsafeMode = "on";

        message::Message msg;
        msg.set_user("demo");
        msg.set_function("getdents");
        msg.set_resultkey("getdents_test");

        const std::string result = execFunctionWithStringResult(msg);
        std::vector<std::string> actual = util::tokeniseString(result, ',');

        for(auto e : expected) {
            REQUIRE(std::find(actual.begin(), actual.end(), e) != actual.end());
        }

        conf.reset();
    }

    TEST_CASE("Test listdir", "[worker]") {
        cleanSystem();

        message::Message msg;
        msg.set_user("demo");
        msg.set_function("listdir");
        msg.set_resultkey("listdir_test");

        execFunction(msg);
    }

    TEST_CASE("Test errno with stat64", "[worker]") {
        cleanSystem();

        util::SystemConfig &conf = util::getSystemConfig();
        conf.unsafeMode = "on";

        message::Message msg;
        msg.set_user("demo");
        msg.set_function("errno");
        msg.set_resultkey("errno_test");

        // Will fail if invalid
        execFunction(msg);

        conf.reset();
    }

    TEST_CASE("Test demo functions", "[worker]") {
        cleanSystem();

        std::string funcName;

        SECTION("fcntl") {
            funcName = "fcntl";
        }

        SECTION("fread") {
            funcName = "fread";
        }

        SECTION("fstat") {
            funcName = "fstat";
        }

        message::Message msg;
        msg.set_user("demo");
        msg.set_function(funcName);
        msg.set_resultkey(funcName + "_test");

        execFunction(msg);
    }
}