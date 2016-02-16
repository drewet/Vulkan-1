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

#ifndef VKTS_BRDFMATERIAL_HPP_
#define VKTS_BRDFMATERIAL_HPP_

#include <vkts/vkts.hpp>

#include "Material.hpp"

namespace vkts
{

class BRDFMaterial: public Material, public IBRDFMaterial
{

protected:

    ITextureSP baseColor;

    ITextureSP metallic;

    ITextureSP specular;

    ITextureSP roughness;

public:

    BRDFMaterial();
    BRDFMaterial(const BRDFMaterial& other);
    BRDFMaterial(BRDFMaterial&& other) = delete;
    virtual ~BRDFMaterial();

    BRDFMaterial& operator =(const BRDFMaterial& other) = delete;
    BRDFMaterial& operator =(BRDFMaterial && other) = delete;

    //
    // IMaterial
    //

    virtual const std::string& getName() const override;

    virtual void setName(const std::string& name) override;

    virtual const ITextureSP& getEmissive() const override;

    virtual void setEmissive(const ITextureSP& emissive) override;

    virtual const ITextureSP& getAlpha() const override;

    virtual void setAlpha(const ITextureSP& alpha) override;

    virtual const ITextureSP& getDisplacement() const override;

    virtual void setDisplacement(const ITextureSP& displacement) override;

    virtual const ITextureSP& getNormal() const override;

    virtual void setNormal(const ITextureSP& normal) override;

    virtual const IDescriptorPoolSP& getDescriptorPool() const override;

    virtual void setDescriptorPool(const IDescriptorPoolSP& descriptorPool) override;

    virtual IDescriptorSetsSP getDescriptorSets() const override;

    virtual void setDescriptorSets(const IDescriptorSetsSP& descriptorSets) override;

    virtual void updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) override;

    virtual void bindDescriptorSets(const ICommandBuffersSP& cmdBuffer, const VkPipelineLayout layout) const override;

    virtual void bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline) const override;

    //
    // IBRDFMaterial
    //

    virtual const ITextureSP& getBaseColor() const override;

    virtual void setBaseColor(const ITextureSP& baseColor) override;

    virtual const ITextureSP& getMetallic() const override;

    virtual void setMetallic(const ITextureSP& metallic) override;

    virtual const ITextureSP& getSpecular() const override;

    virtual void setSpecular(const ITextureSP& specular) override;

    virtual const ITextureSP& getRoughness() const override;

    virtual void setRoughness(const ITextureSP& roughness) override;

    //
    // ICloneable
    //

    virtual IBRDFMaterialSP clone() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_BRDFMATERIAL_HPP_ */
