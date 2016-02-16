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

#include "PhongMaterial.hpp"

namespace vkts
{

PhongMaterial::PhongMaterial() :
    Material(), IPhongMaterial(), ambient(), diffuse(), specular(), specularShininess()
{
}

PhongMaterial::PhongMaterial(const PhongMaterial& other) :
    Material(other), IPhongMaterial(), ambient(other.ambient), diffuse(other.diffuse), specular(other.specular), specularShininess(other.specularShininess)
{
    // Textures cannot be cloned, just replaced.

    //

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_AMBIENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->ambient->getSampler()->getSampler(), this->ambient->getImageView()->getImageView(), this->ambient->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_DIFFUSE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->diffuse->getSampler()->getSampler(), this->diffuse->getImageView()->getImageView(), this->diffuse->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specular->getSampler()->getSampler(), this->specular->getImageView()->getImageView(), this->specular->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR_SHININESS - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specularShininess->getSampler()->getSampler(), this->specularShininess->getImageView()->getImageView(), this->specularShininess->getImage()->getImageLayout());
}

PhongMaterial::~PhongMaterial()
{
    destroy();
}

//
// IMaterial
//

const std::string& PhongMaterial::getName() const
{
    return name;
}

void PhongMaterial::setName(const std::string& name)
{
    this->name = name;
}

const ITextureSP& PhongMaterial::getEmissive() const
{
    return emissive;
}

void PhongMaterial::setEmissive(const ITextureSP& emissive)
{
    this->emissive = emissive;

    if (this->emissive.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->emissive->getSampler()->getSampler(), this->emissive->getImageView()->getImageView(), this->emissive->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getAlpha() const
{
    return alpha;
}

void PhongMaterial::setAlpha(const ITextureSP& alpha)
{
    this->alpha = alpha;

    if (this->alpha.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_ALPHA - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->alpha->getSampler()->getSampler(), this->alpha->getImageView()->getImageView(), this->alpha->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getDisplacement() const
{
    return displacement;
}

void PhongMaterial::setDisplacement(const ITextureSP& displacement)
{
    this->displacement = displacement;

    if (this->displacement.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_DISPLACEMENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->displacement->getSampler()->getSampler(), this->displacement->getImageView()->getImageView(), this->displacement->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getNormal() const
{
    return normal;
}

void PhongMaterial::setNormal(const ITextureSP& normal)
{
    this->normal = normal;

    if (this->normal.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_NORMAL - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->normal->getSampler()->getSampler(), this->normal->getImageView()->getImageView(), this->normal->getImage()->getImageLayout());
    }
}

const IDescriptorPoolSP& PhongMaterial::getDescriptorPool() const
{
    return descriptorPool;
}

void PhongMaterial::setDescriptorPool(const IDescriptorPoolSP& descriptorPool)
{
    this->descriptorPool = descriptorPool;
}

IDescriptorSetsSP PhongMaterial::getDescriptorSets() const
{
    return descriptorSets;
}

void PhongMaterial::setDescriptorSets(const IDescriptorSetsSP& descriptorSets)
{
    this->descriptorSets = descriptorSets;
}

void PhongMaterial::updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
    for (uint32_t i = 0; i < VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT; i++)
    {
    	allWriteDescriptorSets[writeDescriptorSets[i].dstBinding] = writeDescriptorSets[i];
    }

    for (uint32_t i = 0; i < VKTS_BINDING_UNIFORM_BINDING_COUNT; i++)
    {
    	if (i >= allWriteDescriptorSetsCount)
    	{
    		break;
    	}

    	allWriteDescriptorSets[i].dstSet = descriptorSets->getDescriptorSets()[0];
    }

    descriptorSets->updateDescriptorSets(allWriteDescriptorSetsCount, allWriteDescriptorSets, 0, nullptr);
}

void PhongMaterial::bindDescriptorSets(const ICommandBuffersSP& cmdBuffer, const VkPipelineLayout layout) const
{
    if (!cmdBuffer.get())
    {
        return;
    }

    if (!descriptorSets.get())
    {
        return;
    }

    vkCmdBindDescriptorSets(cmdBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSets->getDescriptorSets()[0], 0, nullptr);
}

void PhongMaterial::bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline) const
{
    bindDescriptorSets(cmdBuffer, graphicsPipeline->getLayout());
}

//
// IPhongMaterial
//

const ITextureSP& PhongMaterial::getAmbient() const
{
    return ambient;
}

void PhongMaterial::setAmbient(const ITextureSP& ambient)
{
    this->ambient = ambient;

    if (this->ambient.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_AMBIENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->ambient->getSampler()->getSampler(), this->ambient->getImageView()->getImageView(), this->ambient->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getDiffuse() const
{
    return diffuse;
}

void PhongMaterial::setDiffuse(const ITextureSP& diffuse)
{
    this->diffuse = diffuse;

    if (this->diffuse.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_DIFFUSE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->diffuse->getSampler()->getSampler(), this->diffuse->getImageView()->getImageView(), this->diffuse->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getSpecular() const
{
    return specular;
}

void PhongMaterial::setSpecular(const ITextureSP& specular)
{
    this->specular = specular;

    if (this->specular.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specular->getSampler()->getSampler(), this->specular->getImageView()->getImageView(), this->specular->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getSpecularShininess() const
{
    return specularShininess;
}

void PhongMaterial::setSpecularShininess(const ITextureSP& specularShininess)
{
    this->specularShininess = specularShininess;

    if (this->specularShininess.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR_SHININESS - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specularShininess->getSampler()->getSampler(), this->specularShininess->getImageView()->getImageView(), this->specularShininess->getImage()->getImageLayout());
    }
}

//
// ICloneable
//

IPhongMaterialSP PhongMaterial::clone() const
{
    return IPhongMaterialSP(new PhongMaterial(*this));
}

//
// IDestroyable
//

void PhongMaterial::destroy()
{
    if (ambient.get())
    {
        ambient->destroy();
    }

    if (diffuse.get())
    {
        diffuse->destroy();
    }

    if (specular.get())
    {
        specular->destroy();
    }

    if (specularShininess.get())
    {
        specularShininess->destroy();
    }

    destroyMaterial();
}

} /* namespace vkts */
