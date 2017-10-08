#ifndef __CHAMS_H
#define __CHAMS_H

#include <d3d11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>//generateshader
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

char *state;
ID3D11RasterizerState * rwState;
ID3D11RasterizerState * rsState;

enum eDepthState
{
    ENABLED,
    DISABLED,
    READ_NO_WRITE,
    NO_READ_NO_WRITE,
    _DEPTH_COUNT
};

ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];

void SetDepthStencilState(eDepthState aState)
{
    pContext->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
}

class Chams
{

    public:
    void Init(ID3D11Device* pDevice)
    {
        //create depthstencilstate
        D3D11_DEPTH_STENCIL_DESC  stencilDesc;
        stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        stencilDesc.StencilEnable = true;
        stencilDesc.StencilReadMask = 0xFF;
        stencilDesc.StencilWriteMask = 0xFF;
        stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        stencilDesc.DepthEnable = true;
        stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::ENABLED)]);

        stencilDesc.DepthEnable = false;
        stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::DISABLED)]);

        stencilDesc.DepthEnable = false;
        stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        stencilDesc.StencilEnable = false;
        stencilDesc.StencilReadMask = UINT8(0xFF);
        stencilDesc.StencilWriteMask = 0x0;
        pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::NO_READ_NO_WRITE)]);

        stencilDesc.DepthEnable = true;
        stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; //
        stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
        stencilDesc.StencilEnable = false;
        stencilDesc.StencilReadMask = UINT8(0xFF);
        stencilDesc.StencilWriteMask = 0x0;

        stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
        stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
        stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

        stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
        stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
        stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
        stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
        pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::READ_NO_WRITE)]);
    }
};

#endif
