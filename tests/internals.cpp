#include <iostream>
#include <type_traits>

#include <yorel/yomm2.hpp>

#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

using std::is_same;
namespace yomm2 = yorel::yomm2;
using yomm2::virtual_;

struct matrix {
    virtual ~matrix() {}
};

struct dense_matrix : matrix {};
struct diagonal_matrix : matrix {};

using yorel::yomm2::init_class_info;
using yorel::yomm2::global_class_registry;
init_class_info<matrix> i0(global_class_registry());
init_class_info<dense_matrix, matrix> i2(global_class_registry());
init_class_info<diagonal_matrix, matrix> i3(global_class_registry());

// int main()
// {
//     yorel::yomm2::update_methods();

//     const matrix& dense = dense_matrix();
//     const matrix& diag = diagonal_matrix();
//     times(dense, dense);
//     times(2, dense);
//     times(dense, 2);
//     times(diag, dense);
//     times(diag, diag);
// }

namespace test_internals {

struct test;
auto& registry = yomm2::registry::get<test>();

YOMM2_DECLARE_(test, void, times, virtual_<const matrix&>, virtual_<const matrix&>);
YOMM2_DECLARE_(test, void, times, double, virtual_<const matrix&>);
YOMM2_DECLARE_(test, void, times, virtual_<const matrix&>, double);

YOMM2_DEFINE(void, times, const matrix&, const matrix&) {
} YOMM2_END;

YOMM2_DEFINE(void, times, const diagonal_matrix&, const diagonal_matrix&) {
} YOMM2_END;

YOMM2_DEFINE(void, times, double a, const diagonal_matrix& m) {
} YOMM2_END;

YOMM2_DEFINE(void, times, double a, const matrix& m) {
} YOMM2_END;

YOMM2_DEFINE(void, times, const diagonal_matrix& m, double a) {
} YOMM2_END;

YOMM2_DEFINE(void, times, const matrix& m, double a) {
} YOMM2_END;

BOOST_AUTO_TEST_CASE(registration)
{
    BOOST_TEST(registry.methods.size() == 3);
}

}
