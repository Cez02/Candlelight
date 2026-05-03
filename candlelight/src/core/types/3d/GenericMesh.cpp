#include "GenericMesh.h"

namespace candle::core {
    GenericMesh::GenericMesh(std::vector<Vertex> vertices, std::vector<IndexTriangle> indices)
        : m_Vertices(vertices)
        , m_Triangles(indices) {

    }

    std::vector<Vertex> GenericMesh::GetVertices() {
        return m_Vertices;
    }

    std::vector<IndexTriangle> GenericMesh::GetIndexTriangles() {
        return m_Triangles;
    }
}
