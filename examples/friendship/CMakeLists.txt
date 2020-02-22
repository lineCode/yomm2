# Copyright (c) 2018-2020 Jean-Louis Leroy
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt
# or copy at http://www.boost.org/LICENSE_1_0.txt)

add_executable(friendship friendship.cpp shape_painters.cpp segment_painter.cpp arc_painter.cpp edge_painter.cpp)
target_link_libraries (friendship yomm2)
