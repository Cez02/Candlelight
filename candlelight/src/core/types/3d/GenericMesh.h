#pragma once
#include <array>
#include <memory>
#include <vector>

#include "BaseRawObject.h"
#include "IndexTriangle.h"
#include "Vertex.h"

namespace candle::core {

    class GenericMesh : BaseRawObject {
        std::vector<Vertex> m_Vertices;
        std::vector<IndexTriangle> m_Triangles;

    public:
        GenericMesh(std::vector<Vertex> vertices, std::vector<IndexTriangle> indices);

        std::vector<Vertex> GetVertices();
        std::vector<IndexTriangle> GetIndexTriangles();

    };

    typedef std::shared_ptr<GenericMesh> HGenericMesh;
}
