#include <iostream>

#include "geometries.hpp"
#include "painter.hpp"

namespace painters {

define_method(
    shape_painters,
    void, paintObject, (Painter& painter, const geometries::Shape& shape))
{
    ++painter.counter;
    static int counter;
    ++counter;
    std::cout << "#" << painter.counter << " #" << counter << " ";
}

define_method(
    shape_painters,
    void, paintObject, (Painter& painter, const geometries::Square& square))
{
    method_definition(shape_painters, void, (Painter&, const geometries::Shape&))(painter, square);
    std::cout << "painting square\n";
}

define_method(
    shape_painters,
    void, paintObject, (Painter& painter, const geometries::Circle& circle))
{
    method_definition(shape_painters, void, (Painter&, const geometries::Shape&))(painter, circle);
    std::cout << "painting Circle\n";
}

} // namespace painters
