#include <catch2/catch_test_macros.hpp>
#include "maestro/theory/Version.h"

TEST_CASE("kVersion is non-empty", "[version]")
{
    REQUIRE(std::string(maestro::theory::kVersion).size() > 0);
}
