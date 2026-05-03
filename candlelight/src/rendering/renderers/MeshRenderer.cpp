#include "MeshRenderer.h"

#include "memory/MemoryAlgorithms.h"

namespace candle::rendering {
    MeshRenderer::MeshRenderer(core::HGenericMesh mesh, const RenderingContext &renderingContext)
        : BaseObject(renderingContext)
        , m_Mesh(mesh) {

        // Basic mesh setup, todo: replace with EBOs / some opensource implementation
        auto vertices = mesh->GetVertices();
        auto indexTriangles = mesh->GetIndexTriangles();

        size_t resourceSize = indexTriangles.size() * 3 * sizeof(glm::vec3);

        m_Resource = std::make_shared<Resource>();

        m_Resource->Init(renderingContext.GetDevice(), resourceSize, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

        auto resourceName = L"Mesh " + core::to_wstring(m_Resource->GetGUID());
        m_Resource->GetResource()->SetName(resourceName.c_str());

        void* dest = nullptr;
        m_Resource->GetResource()->Map(0, 0, &(dest));

        glm::vec3 *destVerts = static_cast<glm::vec3 *>(dest);

        int stride = 3;
        for (int i = 0; i < indexTriangles.size(); i++) {
            destVerts[i * stride + 0] = vertices[indexTriangles[i][0]].position;
            destVerts[i * stride + 1] = vertices[indexTriangles[i][1]].position;
            destVerts[i * stride + 2] = vertices[indexTriangles[i][2]].position;
        }

        m_Resource->GetResource()->Unmap(0, 0);


        m_DynamicVBV.BufferLocation = m_Resource->GetResource()->GetGPUVirtualAddress();
        m_DynamicVBV.StrideInBytes = sizeof(glm::vec3);
        m_DynamicVBV.SizeInBytes = resourceSize;
    }

    void MeshRenderer::Draw(ComPtr<ID3D12GraphicsCommandList2> commandList) {

        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        commandList->IASetVertexBuffers(0, 1, &m_DynamicVBV);

        // draw call
        commandList->DrawInstanced(3, 1, 0, 0);
    }
}
