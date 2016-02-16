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

#include <vkts/vkts.hpp>

#include "FreeTypeFont.hpp"

namespace vkts
{

static std::mutex g_fontMutex;

static FT_Library g_library = nullptr;

VkBool32 VKTS_APIENTRY fontInit()
{
    std::lock_guard<std::mutex> logLockGuard(g_fontMutex);

    if (g_library)
    {
        return VK_FALSE;
    }

    return FT_Init_FreeType(&g_library) == 0;
}

IFontSP VKTS_APIENTRY fontCreate(const IInitialResourcesSP& initialResources, const std::string& filename)
{
    std::lock_guard<std::mutex> logLockGuard(g_fontMutex);

    if (!initialResources.get() || !g_library || !filename.c_str())
    {
        return IFontSP();
    }

    FT_Face face = nullptr;

    if (FT_New_Face(g_library, filename.c_str(), 0, &face))
    {
        return IFontSP();
    }

    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
    {
        FT_Done_Face(face);

        return IFontSP();
    }

    auto newInstance = new FreeTypeFont(initialResources, g_library, face);

    if (!newInstance)
    {
        FT_Done_Face(face);

        return IFontSP();
    }

    return IFontSP(newInstance);
}

void VKTS_APIENTRY fontTerminate()
{
    std::lock_guard<std::mutex> logLockGuard(g_fontMutex);

    if (g_library)
    {
        FT_Done_FreeType(g_library);

        g_library = nullptr;
    }
}

}
