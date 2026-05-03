#pragma once

#include <memory>

#include "BaseObject.h"
#include "GenericMesh.h"
#include "RenderingContext.h"
#include "Resource.h"

namespace candle::rendering {
    class MeshRenderer : core::BaseObject<RenderingContext> {
        core::HGenericMesh m_Mesh;
        HResource m_Resource;
        D3D12_VERTEX_BUFFER_VIEW m_DynamicVBV;

    public:
        MeshRenderer(core::HGenericMesh mesh, const RenderingContext& renderingContext);

        void Draw(ComPtr<ID3D12GraphicsCommandList2> commandList);
    };

    typedef std::shared_ptr<MeshRenderer> HMeshRenderer;
}
