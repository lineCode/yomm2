#ifndef PAINTER_DEFINED
#define PAINTER_DEFINED

#include "geometries.hpp"

#include <yorel/yomm2/cute.hpp>

namespace painters {

method_container(segment_painters);
method_container(shape_painters);

class Painter
{
public:
    void paint(const geometries::Geometry& geometry);
    int painted() const;
private:
    int counter = 0;
    friend_method(segment_painters);
    friend_method(shape_painters, void, (Painter&, const geometries::Shape&));
};

// Implements paint
declare_method(
    void,
    paintObject,
    (Painter&, yorel::yomm2::virtual_<const geometries::Geometry&>));

inline void Painter::paint(const geometries::Geometry& geometry) {
    paintObject(*this, geometry);
}

inline int Painter::painted() const {
    return counter;
}

} // namespace painters

#endif
