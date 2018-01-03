#include "svbb_test_config.hpp"
#include "svbb/csv.hpp"
#include "svbb/string_view_conversion.hpp"
#include <iostream>

namespace {
using namespace svbb;
TEST_CASE("find_column_index")
{
    REQUIRE_FALSE(find_column_index(tokenize("", ','), ""));
    REQUIRE(*find_column_index(tokenize(" ", ','), " ") == 0);
    REQUIRE(*find_column_index(tokenize("a", ','), "a") == 0);
    REQUIRE(*find_column_index(tokenize("a,b", ','), "b") == 1);
}

TEST_CASE("find_column_indices")
{
    const auto indices = find_column_indices(tokenize("city,color,population,beer", ','),
                                             "population", "city", "goals", "beer");

    REQUIRE(indices.size() == 4);
    REQUIRE(indices[0] == 2);
    REQUIRE(indices[1] == 0);
    REQUIRE_FALSE(indices[2]);
    REQUIRE(indices[3] == 3);
}

TEST_CASE("parse_csv_header")
{
    const auto header = parse_csv_header(tokenize("city,color,population,beer", ','), "population",
                                         "city", "goals", "beer");

    REQUIRE(header.size() == 4);
    REQUIRE(header[0] == 1);
    REQUIRE_FALSE(header[1]);
    REQUIRE(header[2] == 0);
    REQUIRE(header[3] == 3);
}

TEST_CASE("parse_csv_line")
{
    const auto header =
        parse_csv_header(tokenize("city,color,population,beer", ','), "population", "city", "beer");

    string_view_concatination<3> fields;
    parse_csv_line(tokenize("berlin,blue,42,alot", ','), fields, header);
    REQUIRE(fields[0] == "42");
    REQUIRE(fields[1] == "berlin");
    REQUIRE(fields[2] == "alot");
}

struct mytype
{
    mytype() = default;
    explicit mytype(string_view sv) : str{sv.begin(), sv.end()} {}
    mytype& operator=(string_view sv)
    {
        str.assign(sv.begin(), sv.end());
        return *this;
    }

    std::string str;
};

TEST_CASE("parse_csv_line values")
{
    const auto header =
        parse_csv_header(tokenize("city,color,population,beer", ','), "population", "city", "beer");

    int pop;
    mytype city;
    std::string beer;
    parse_csv_line(tokenize("berlin,0,42,22.2", ','), header, pop, city, beer);
    REQUIRE(pop == 42);
    REQUIRE(city.str == "berlin");
    REQUIRE(beer == "22.2");
}
} // namespace
