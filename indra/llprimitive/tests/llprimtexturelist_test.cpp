/**
 * Copyright 2026 Finalverse contributors.
 * SPDX-License-Identifier: LGPL-2.1-only
 */
#include "linden_common.h"
#include "lltut.h"
#include "../llprimtexturelist.h"
#include "../lltextureentry.h"

namespace tut
{
    struct texture_list_test {};
    typedef test_group<texture_list_test> texture_list_group;
    typedef texture_list_group::object texture_list_object;
    texture_list_group texture_list_tests("LLPrimTextureList");

    template<> template<>
    void texture_list_object::test<1>()
    {
        set_test_name("Self-copy preserves an owned texture entry");
        LLPrimTextureList textures;
        textures.setSize(1);
        const LLUUID id = LLUUID::generateNewID();
        textures.setID(0, id);
        textures.setScale(0, 2.0f, 3.0f);
        ensure_equals(textures.copyTexture(0, *textures.getTexture(0)), TEM_CHANGE_TEXTURE);
        ensure_equals(textures.getTexture(0)->getID(), id);
        ensure_equals(textures.getTexture(0)->getScaleS(), 2.0f);
        ensure_equals(textures.getTexture(0)->getScaleT(), 3.0f);
    }

    template<> template<>
    void texture_list_object::test<2>()
    {
        set_test_name("Copying the null sentinel creates a default entry");
        LLPrimTextureList textures;
        textures.setSize(1);
        textures.setID(0, LLUUID::generateNewID());
        ensure_equals(textures.copyTexture(0, LLTextureEntry::null), TEM_CHANGE_TEXTURE);
        ensure(textures.getTexture(0) != nullptr);
        ensure_equals(textures.getTexture(0)->getID(), LLUUID::null);
    }
}
