/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#include "Config.h"

#include "GlobalStorage.h"
#include "JobQueue.h"
#include "compat/Variant.h"
#include "utils/Logger.h"
#include "utils/Retranslator.h"
#include "utils/Variant.h"

Config::Config( QObject* parent )
    : QObject( parent )
{
    CALAMARES_RETRANSLATE_SLOT( &Config::retranslate );
}

Config::~Config() {}

void
Config::retranslate()
{
    emit titleLabelChanged( titleLabel() );
    emit subtitleLabelChanged( subtitleLabel() );
    emit sidebarLabelChanged( sidebarLabel() );
}

QString
Config::titleLabel() const
{
    return m_titleLabel ? m_titleLabel->get() : QString();
}

QString
Config::subtitleLabel() const
{
    return m_subtitleLabel ? m_subtitleLabel->get() : QString();
}

QString
Config::sidebarLabel() const
{
    return m_sidebarLabel ? m_sidebarLabel->get() : QString();
}

void
Config::loadingDone()
{
    emit statusReady();
}


void
Config::setConfigurationMap( const QVariantMap& configurationMap )
{
    bool bogus = false;
    auto label = Calamares::getSubMap( configurationMap, "label", bogus );
    static const char className[] = "PresetsViewStep";
    if ( label.contains( "sidebar" ) )
    {
        m_sidebarLabel = new Calamares::Locale::TranslatedString( label, "sidebar", className );
    }
    if ( label.contains( "title" ) )
    {
        m_titleLabel = new Calamares::Locale::TranslatedString( label, "title", className );
    }
    if ( label.contains( "subtitle" ) )
    {
        m_subtitleLabel = new Calamares::Locale::TranslatedString( label, "subtitle", className );
    }
    if ( configurationMap.contains( "entries" )
         && Calamares::typeOf( configurationMap.value( "entries" ) ) == Calamares::ListVariantType )
    {
        const auto entries = configurationMap.value( "entries" ).toList();
        for ( const QVariant& entryV : entries )
        {
            if ( Calamares::typeOf( entryV ) != Calamares::MapVariantType )
            {
                continue;
            }
            m_sections.append( Section( entryV.toMap() ) );
        }
    }
    loadingDone();
}

void
Config::finalizeGlobalStorage()
{
    auto* gs = Calamares::JobQueue::instance() ? Calamares::JobQueue::instance()->globalStorage() : nullptr;
    if ( gs )
    {
        QVariantMap m;
        m.insert( "selection", m_sections.at( m_selected ).target());
        gs->insert( "presets", m );
    }
}
