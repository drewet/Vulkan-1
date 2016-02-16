/**
 * VKTS - VulKan ToolS.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) since 2014 Norbert Nopper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SubMesh.hpp"

namespace vkts
{

SubMesh::SubMesh() :
    ISubMesh(), name(""), vertexBuffer(), vertexBufferType(0), numberVertices(0), indicesVertexBuffer(), numberIndices(0), primitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST), brdfMaterial(), phongMaterial(), vertexOffset(-1), normalOffset(-1), bitangentOffset(-1), tangentOffset(-1), texcoordOffset(-1), boneIndices0Offset(-1), boneIndices1Offset(-1), boneWeights0Offset(-1), boneWeights1Offset(-1), numberBonesOffset(-1), strideInBytes(0)
{
}

SubMesh::SubMesh(const SubMesh& other) :
    ISubMesh(), name(other.name), vertexBuffer(other.vertexBuffer), vertexBufferType(other.vertexBufferType), numberVertices(other.numberVertices), indicesVertexBuffer(other.indicesVertexBuffer), numberIndices(other.numberIndices), primitiveTopology(other.primitiveTopology), brdfMaterial(), phongMaterial(), vertexOffset(other.vertexOffset), normalOffset(other.normalOffset), bitangentOffset(other.bitangentOffset), tangentOffset(other.tangentOffset), texcoordOffset(other.texcoordOffset), boneIndices0Offset(other.boneIndices0Offset), boneIndices1Offset(other.boneIndices1Offset), boneWeights0Offset(other.boneWeights0Offset), boneWeights1Offset(other.boneWeights1Offset), numberBonesOffset(other.numberBonesOffset), strideInBytes(other.strideInBytes)
{
    if (other.brdfMaterial.get())
    {
        brdfMaterial = other.brdfMaterial->clone();
    }
    else if (other.phongMaterial.get())
    {
        phongMaterial = other.phongMaterial->clone();
    }
}

SubMesh::~SubMesh()
{
    destroy();
}

//
// ISubMesh
//

const std::string& SubMesh::getName() const
{
    return name;
}

void SubMesh::setName(const std::string& name)
{
    this->name = name;
}

const IBufferObjectSP& SubMesh::getVertexBuffer() const
{
    return vertexBuffer;
}

VkTsVertexBufferType SubMesh::getVertexBufferType() const
{
    return vertexBufferType;
}

void SubMesh::setVertexBuffer(const IBufferObjectSP& vertexBuffer, const VkTsVertexBufferType vertexBufferType)
{
    this->vertexBuffer = vertexBuffer;
    this->vertexBufferType = vertexBufferType;
}

int32_t SubMesh::getNumberVertices() const
{
    return numberVertices;
}

void SubMesh::setNumberVertices(const int32_t numberVertices)
{
    this->numberVertices = numberVertices;
}

const IBufferObjectSP& SubMesh::getIndicesVertexBuffer() const
{
    return indicesVertexBuffer;
}

void SubMesh::setIndicesVertexBuffer(const IBufferObjectSP& indicesVertexBuffer)
{
    this->indicesVertexBuffer = indicesVertexBuffer;
}

int32_t SubMesh::getNumberIndices() const
{
    return numberIndices;
}

void SubMesh::setNumberIndices(const int32_t numberIndices)
{
    this->numberIndices = numberIndices;
}

VkIndexType SubMesh::getIndexType() const
{
    return VK_INDEX_TYPE_UINT32;
}

VkPrimitiveTopology SubMesh::getPrimitiveTopology() const
{
    return primitiveTopology;
}

void SubMesh::setPrimitiveTopology(const VkPrimitiveTopology primitiveTopology)
{
    this->primitiveTopology = primitiveTopology;
}

const IBRDFMaterialSP& SubMesh::getBRDFMaterial() const
{
    return brdfMaterial;
}

void SubMesh::setBRDFMaterial(const IBRDFMaterialSP& brdfMaterial)
{
    this->brdfMaterial = brdfMaterial;
}

const IPhongMaterialSP& SubMesh::getPhongMaterial() const
{
    return phongMaterial;
}

void SubMesh::setPhongMaterial(const IPhongMaterialSP& phongMaterial)
{
    this->phongMaterial = phongMaterial;
}

int32_t SubMesh::getVertexOffset() const
{
    return vertexOffset;
}

void SubMesh::setVertexOffset(const int32_t vertexOffset)
{
    this->vertexOffset = vertexOffset;
}

int32_t SubMesh::getNormalOffset() const
{
    return normalOffset;
}

void SubMesh::setNormalOffset(const int32_t normalOffset)
{
    this->normalOffset = normalOffset;
}

int32_t SubMesh::getBitangentOffset() const
{
    return bitangentOffset;
}

void SubMesh::setBitangentOffset(const int32_t bitangentOffset)
{
    this->bitangentOffset = bitangentOffset;
}

int32_t SubMesh::getTangentOffset() const
{
    return tangentOffset;
}

void SubMesh::setTangentOffset(const int32_t tangentOffset)
{
    this->tangentOffset = tangentOffset;
}

int32_t SubMesh::getTexcoordOffset() const
{
    return texcoordOffset;
}

void SubMesh::setTexcoordOffset(const int32_t texcoordOffset)
{
    this->texcoordOffset = texcoordOffset;
}

int32_t SubMesh::getBoneIndices0Offset() const
{
    return boneIndices0Offset;
}

void SubMesh::setBoneIndices0Offset(const int32_t boneIndices0Offset)
{
    this->boneIndices0Offset = boneIndices0Offset;
}

int32_t SubMesh::getBoneIndices1Offset() const
{
    return boneIndices1Offset;
}

void SubMesh::setBoneIndices1Offset(const int32_t boneIndices1Offset)
{
    this->boneIndices1Offset = boneIndices1Offset;
}

int32_t SubMesh::getBoneWeights0Offset() const
{
    return boneWeights0Offset;
}

void SubMesh::setBoneWeights0Offset(const int32_t boneWeights0Offset)
{
    this->boneWeights0Offset = boneWeights0Offset;
}

int32_t SubMesh::getBoneWeights1Offset() const
{
    return boneWeights1Offset;
}

void SubMesh::setBoneWeights1Offset(const int32_t boneWeights1Offset)
{
    this->boneWeights1Offset = boneWeights1Offset;
}

int32_t SubMesh::getNumberBonesOffset() const
{
    return numberBonesOffset;
}

void SubMesh::setNumberBonesOffset(const int32_t numberBonesOffset)
{
    this->numberBonesOffset = numberBonesOffset;
}

uint32_t SubMesh::getStrideInBytes() const
{
    return strideInBytes;
}

void SubMesh::setStrideInBytes(const uint32_t strideInBytes)
{
    this->strideInBytes = strideInBytes;
}

VkBool32 SubMesh::hasBones() const
{
    return numberBonesOffset >= 0;
}

void SubMesh::updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
    if (brdfMaterial.get())
    {
        brdfMaterial->updateDescriptorSetsRecursive(allWriteDescriptorSetsCount, allWriteDescriptorSets);
    }
    else if (phongMaterial.get())
    {
        phongMaterial->updateDescriptorSetsRecursive(allWriteDescriptorSetsCount, allWriteDescriptorSets);
    }
}

void SubMesh::bindIndexBuffer(const ICommandBuffersSP& cmdBuffer) const
{
    if (!indicesVertexBuffer.get())
    {
        return;
    }

    if (!indicesVertexBuffer->getBuffer().get())
    {
        return;
    }

    if (!cmdBuffer.get())
    {
        return;
    }

    vkCmdBindIndexBuffer(cmdBuffer->getCommandBuffer(), indicesVertexBuffer->getBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void SubMesh::bindVertexBuffers(const ICommandBuffersSP& cmdBuffer) const
{
    if (!vertexBuffer.get())
    {
        return;
    }

    if (!vertexBuffer->getBuffer().get())
    {
        return;
    }

    if (!cmdBuffer.get())
    {
        return;
    }

    VkDeviceSize offsets[1] = {0};

    VkBuffer buffers[1] = {vertexBuffer->getBuffer()->getBuffer()};

    vkCmdBindVertexBuffers(cmdBuffer->getCommandBuffer(), 0, 1, buffers, offsets);
}

void SubMesh::drawIndexed(const ICommandBuffersSP& cmdBuffer) const
{
    if (!cmdBuffer.get())
    {
        return;
    }

    vkCmdDrawIndexed(cmdBuffer->getCommandBuffer(), getNumberIndices(), 1, 0, 0, 0);
}

void SubMesh::bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline) const
{
    if (brdfMaterial.get())
    {
        brdfMaterial->bindDrawIndexedRecursive(cmdBuffer, graphicsPipeline);
    }
    else if (phongMaterial.get())
    {
        phongMaterial->bindDrawIndexedRecursive(cmdBuffer, graphicsPipeline);
    }
    else
    {
        return;
    }

    bindIndexBuffer(cmdBuffer);
    bindVertexBuffers(cmdBuffer);
    drawIndexed(cmdBuffer);
}

//
// ICloneable
//

ISubMeshSP SubMesh::clone() const
{
    return ISubMeshSP(new SubMesh(*this));
}

//
// IDestroyable
//

void SubMesh::destroy()
{
    if (brdfMaterial.get())
    {
        brdfMaterial->destroy();
    }

    if (phongMaterial.get())
    {
        phongMaterial->destroy();
    }

    // Not destroying the index and vertex buffer by purpose.

    // Not destroying the graphics pipeline by purpose.
}

} /* namespace vkts */
