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

#ifndef VKTS_IMATERIAL_HPP_
#define VKTS_IMATERIAL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IMaterial: public IDestroyable
{

public:

    IMaterial() :
        IDestroyable()
    {
    }

    virtual ~IMaterial()
    {
    }

    virtual const std::string& getName() const = 0;

    virtual void setName(const std::string& name) = 0;

    virtual const ITextureSP& getEmissive() const = 0;

    virtual void setEmissive(const ITextureSP& emissive) = 0;

    virtual const ITextureSP& getAlpha() const = 0;

    virtual void setAlpha(const ITextureSP& alpha) = 0;

    virtual const ITextureSP& getDisplacement() const = 0;

    virtual void setDisplacement(const ITextureSP& displacement) = 0;

    virtual const ITextureSP& getNormal() const = 0;

    virtual void setNormal(const ITextureSP& normal) = 0;

    virtual const IDescriptorPoolSP& getDescriptorPool() const = 0;

    virtual void setDescriptorPool(const IDescriptorPoolSP& descriptorPool) = 0;

    virtual IDescriptorSetsSP getDescriptorSets() const = 0;

    virtual void setDescriptorSets(const IDescriptorSetsSP& descriptorSets) = 0;

    virtual void updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) = 0;

    virtual void bindDescriptorSets(const ICommandBuffersSP& cmdBuffer, const VkPipelineLayout layout) const = 0;

    virtual void bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline) const = 0;

};

typedef std::shared_ptr<IMaterial> IMaterialSP;

} /* namespace vkts */

#endif /* VKTS_IMATERIAL_HPP_ */
