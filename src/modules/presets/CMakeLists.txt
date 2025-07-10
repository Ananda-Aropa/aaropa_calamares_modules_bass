# === This file is part of Calamares - <https://calamares.io> ===
#
#   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
#   SPDX-License-Identifier: GPL-3.0-or-later
#
calamares_add_plugin(presets
    TYPE viewmodule
    EXPORT_MACRO PLUGINDLLEXPORT_PRO
    SOURCES
        section.cpp
        Config.cpp
        SectionItem.cpp
        PresetsViewStep.cpp
        PresetsPage.cpp
    UI
        presets.ui
    LINK_PRIVATE_LIBRARIES
        ${qtname}::Widgets
        ${qtname}::Svg
    SHARED_LIB
)

if(TARGET ${kfname}::CoreAddons)
    #calamares_add_test(
    #    dataimgtest
    #    SOURCES Tests.cpp Config.cpp
    #    LIBRARIES ${qtname}::Widgets ${qtname}::Gui ${kfname}::CoreAddons
    #)
endif()
