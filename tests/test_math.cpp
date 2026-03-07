#include <doctest/doctest.h>
#include <glm/glm.hpp>

TEST_CASE("Testing Vector3 Math") {
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 5.0f, 6.0f);

    SUBCASE("Vector Addition") {
        auto result = a + b;
        CHECK(result.x == 5.0f);
        CHECK(result.y == 7.0f);
        CHECK(result.z == 9.0f);
    }

    SUBCASE("Dot Product") {
        float dot = glm::dot(a, b);
        CHECK(dot == doctest::Approx(32.0f)); // Use Approx for floats!
    }
}