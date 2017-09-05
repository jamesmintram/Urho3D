//
// Copyright (c) 2008-2017 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../../Precompiled.h"

#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsImpl.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../IO/Log.h"

#include "../../DebugNew.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>

bgfx::VertexDecl createDecl(Urho3D::PODVector<Urho3D::VertexElement> const &elements)
{
    bgfx::VertexDecl decl;
    
    decl.begin();
    
    static bgfx::Attrib::Enum attribMap[] = {
        bgfx::Attrib::Position,
        bgfx::Attrib::Normal,
        bgfx::Attrib::Bitangent,
        bgfx::Attrib::Tangent,
        
        bgfx::Attrib::TexCoord0,
        bgfx::Attrib::Color0,
        bgfx::Attrib::Weight,
        bgfx::Attrib::Indices,
        
        bgfx::Attrib::TexCoord7
    };
    static bgfx::AttribType::Enum typeMap[] = {
        bgfx::AttribType::Int16,
        bgfx::AttribType::Float,
        bgfx::AttribType::Float,
        bgfx::AttribType::Float,
        bgfx::AttribType::Float,
        bgfx::AttribType::Uint8,
        bgfx::AttribType::Uint8
    };
    static int countMap[] = {
        1,
        1,
        2,
        3,
        4,
        4,
        4
    };
    
    int texIdx = 0;
    int colIdx = 0;
    
    for (int idx = 0; idx < elements.Size(); idx++)
    {
        const Urho3D::VertexElementSemantic sem = elements[idx].semantic_;
        const Urho3D::VertexElementType typ = elements[idx].type_;
        
        const bool normalised = typ == Urho3D::TYPE_UBYTE4_NORM;
        const bgfx::AttribType::Enum bgfxType = typeMap[typ];
        const int count = countMap[typ];
        
        if (sem ==  Urho3D::SEM_TEXCOORD)
        {
            decl.add((bgfx::Attrib::Enum)(bgfx::Attrib::TexCoord0 + texIdx), count, bgfxType, normalised);
            texIdx++;
        }
        else if(sem ==  Urho3D::SEM_COLOR)
        {
            decl.add((bgfx::Attrib::Enum)(bgfx::Attrib::Color0 + colIdx), count, bgfxType, normalised);
            colIdx++;
        }
        else
        {
            decl.add(attribMap[sem], count, bgfxType, normalised);
        }
    }
    
    decl.end();

    return decl;
}

namespace Urho3D
{

	void VertexBuffer::OnDeviceLost()
	{

	}

	void VertexBuffer::OnDeviceReset()
	{

	}

	void VertexBuffer::Release()
	{
        if (dynamic_)
        {
            bgfx::DynamicVertexBufferHandle vbo; vbo.idx = object_.name_;
            if (bgfx::isValid(vbo))
            {
                bgfx::destroy(vbo);
            }
        }
        else
        {
            bgfx::VertexBufferHandle vbo; vbo.idx = object_.name_;
            if (bgfx::isValid(vbo))
            {
                bgfx::destroy(vbo);
            }
        }
        
        object_.name_ = BGFX_INVALID_HANDLE;
	}

	bool VertexBuffer::SetData(const void* data)
	{
        bgfx::VertexDecl decl = createDecl(this->elements_);
        if (dynamic_)
        {
            //TODO: Check if object name is valid - if so then we are just uploading some new darta
            bgfx::DynamicVertexBufferHandle vbo = bgfx::createDynamicVertexBuffer(vertexCount_, decl);
            this->object_.name_= vbo.idx;
        }
        else
        {
            //TODO: Check that out object_name is invalid as we can only set data once
            bgfx::Memory const *data = bgfx::alloc(decl.getSize(this->vertexCount_));
            bx::memCopy(data->data, data, data->size);
            
            bgfx::VertexBufferHandle vbo = bgfx::createVertexBuffer(data, decl);
            this->object_.name_= vbo.idx;
        }
        
		return true;
	}

	bool VertexBuffer::SetDataRange(const void* data, unsigned start, unsigned count, bool discard)
	{
        //TODO: Implement
		//memcpy(&((char*)object_.ptr_)[start], data, count);
		return true;
	}

	void* VertexBuffer::Lock(unsigned start, unsigned count, bool discard)
	{
        //TODO: Implement
        return nullptr;
		//return &((char*)object_.ptr_)[start];
	}

	void VertexBuffer::Unlock()
	{
		
	}

	bool VertexBuffer::Create()
	{
        
		Release();
        
		if (!vertexCount_ || elements_.Empty())
			return true;

		if (graphics_)
		{

		}
		return true;
	}

	bool VertexBuffer::UpdateToGPU()
	{
		return true;
	}

	void* VertexBuffer::MapBuffer(unsigned start, unsigned count, bool discard)
	{
        //TODO: Implement
        return nullptr;
        
		//return &((char*)object_.ptr_)[start];
	}

	void VertexBuffer::UnmapBuffer()
	{
	}

}
