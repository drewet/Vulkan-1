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

#ifndef VKTS_FREETYPEFONT_HPP_
#define VKTS_FREETYPEFONT_HPP_

#include <vkts/vkts.hpp>

#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>

namespace vkts
{

typedef struct _FontCacheEntry
{

    char32_t character;
    size_t size;

    int32_t width;
    int32_t height;

    int32_t advanceX;
    int32_t advanceY;
    int32_t offsetX;
    int32_t offsetY;

    IImageDataSP glyphImageData;

    ITextureSP texture;

} FontCacheEntry;

class FreeTypeFont: public IFont
{

private:

    const IInitialResourcesSP initialResources;

    const FT_Library& library;
    FT_Face face;

    std::multimap<char32_t, FontCacheEntry> cache;

    SmartPointerVector<IImageSP> allStageImages;
    SmartPointerVector<IBufferSP> allStageBuffers;
    SmartPointerVector<IDeviceMemorySP> allStageDeviceMemories;

    FontCacheEntry* getGlyph(const char32_t character, const size_t size);

    FontCacheEntry* getGlyph(const ICommandBuffersSP& cmdBuffer, const char32_t character, const size_t size);

public:

    FreeTypeFont() = delete;
    FreeTypeFont(const IInitialResourcesSP& initialResources, const FT_Library& library, const FT_Face& face);
    FreeTypeFont(const FreeTypeFont& other) = delete;
    FreeTypeFont(FreeTypeFont&& other) = delete;
    virtual ~FreeTypeFont();

    FreeTypeFont& operator =(const FreeTypeFont& other) = delete;
    FreeTypeFont& operator =(FreeTypeFont && other) = delete;

    //
    // IFont
    //

    virtual VkBool32 prepareText(const ICommandBuffersSP& cmdBuffer, const std::u32string& text, const size_t size) override;

    virtual VkBool32 renderText(const ICommandBuffersSP& cmdBuffer, const std::u32string& text, const size_t size, const glm::vec4& color) override;

    virtual int32_t getTextLength(const std::u32string& text, const size_t size) override;

    virtual void destroyStageResources() override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_FREETYPEFONT_HPP_ */
