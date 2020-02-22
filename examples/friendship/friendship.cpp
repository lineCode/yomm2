// Copyright (c) 2020 Jean-Louis Leroy
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This is based on sample code provided by Github user matpen in
// https://github.com/jll63/yomm2/issues/7

#include <yorel/yomm2/cute.hpp>

#include <iostream>
#include <memory>
#include <string>

#include "geometries.hpp"
#include "painter.hpp"

using yorel::yomm2::virtual_;
using std::cout;

register_class(geometries::Geometry);
register_class(geometries::Segment, geometries::Geometry);
register_class(geometries::Arc, geometries::Segment);
register_class(geometries::Edge, geometries::Segment);
register_class(geometries::Shape, geometries::Geometry);
register_class(geometries::Square, geometries::Shape);
register_class(geometries::Circle, geometries::Shape);

int main() {
    yorel::yomm2::update_methods();

    const geometries::Geometry& arc = geometries::Arc();
    const geometries::Geometry& edge = geometries::Edge();
    const geometries::Geometry& square = geometries::Square();
    const geometries::Geometry& circle = geometries::Circle();

    painters::Painter painter;
    painter.paint(arc);
    painter.paint(edge);
    painter.paint(square);
    painter.paint(circle);
}
