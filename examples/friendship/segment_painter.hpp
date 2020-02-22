#ifndef SEGMENT_PAINTER_DEFINED
#define SEGMENT_PAINTER_DEFINED

#include "painter.hpp"

namespace painters {

define_method_inline(
    segment_painters,
    void, paintObject, (Painter& painter, const geometries::Segment& arc))
{
    ++painter.counter;
    std::cout << "#" << painter.counter;
}

} // namespace painters

#endif
