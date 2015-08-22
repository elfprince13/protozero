
#include <test.hpp>

TEST_CASE("read repeated packed bool field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_bool/data-empty");

        protozero::pbf_reader item(buffer);

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_bool/data-one");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_pair = item.get_packed_bool();
        REQUIRE(!item.next());

        REQUIRE(it_pair.first != it_pair.second);
        REQUIRE(*it_pair.first);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_bool/data-many");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_pair = item.get_packed_bool();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(it != it_pair.second);
        REQUIRE(*it++);
        REQUIRE(*it++);
        REQUIRE(! *it++);
        REQUIRE(*it++);
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_bool/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_bool(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed bool field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        bool data[] = { true };
        pw.add_packed_bool(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_bool/data-empty"));
    }

    SECTION("one") {
        bool data[] = { true };
        pw.add_packed_bool(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_bool/data-one"));
    }

    SECTION("many") {
        bool data[] = { true, true, false, true };
        pw.add_packed_bool(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_bool/data-many"));
    }

}
