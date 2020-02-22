#include <iostream>

#include <yorel/yomm2/cute.hpp>

#include "geometries.hpp"
#include "segment_painter.hpp"

namespace painters {

define_method(
    segment_painters,
    void, paintObject, (Painter& painter, const geometries::Edge& edge))
{
    method_definition(
        segment_painters, void, (Painter&, const geometries::Segment&))(
            painter, edge);
    std::cout << " " << "painting edge\n";
}

} // namespace painters
