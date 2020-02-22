#ifndef GEOMETRY_DEFINED
#define GEOMETRY_DEFINED

namespace geometries {

class Geometry
{
public:
    virtual ~Geometry() {}
};

class Segment: public Geometry {
};

class Arc : public Segment {
};

class Edge : public Segment {
};

class Shape : public Geometry {
};

class Square : public Shape {
};

class Circle : public Shape {
};

} // namespace geometries

#endif
