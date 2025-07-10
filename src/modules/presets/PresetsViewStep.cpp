/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#include "PresetsViewStep.h"
#include "PresetsPage.h"

#include "ViewManager.h"

#include "utils/Logger.h"
#include "utils/QtCompat.h"
#include "utils/Retranslator.h"
#include "utils/Variant.h"
#include "widgets/TranslationFix.h"

#include <QMessageBox>

CALAMARES_PLUGIN_FACTORY_DEFINITION( PresetsViewStepFactory, registerPlugin< PresetsViewStep >(); )

PresetsViewStep::PresetsViewStep( QObject* parent )
    : Calamares::ViewStep( parent )
    , m_widget( new PresetsPage( &m_config ) )
    , m_nextEnabled( true )
{
    connect( &m_config, &Config::statusReady, this, &PresetsViewStep::nextIsReady );
}

PresetsViewStep::~PresetsViewStep()
{
    if ( m_widget && m_widget->parent() == nullptr )
    {
        m_widget->deleteLater();
    }
}

QString
PresetsViewStep::prettyName() const
{
    return m_config.sidebarLabel();
}


QWidget*
PresetsViewStep::widget()
{
    return m_widget;
}


bool
PresetsViewStep::isNextEnabled() const
{
    return true;
}


bool
PresetsViewStep::isBackEnabled() const
{
    return true;
}


bool
PresetsViewStep::isAtBeginning() const
{
    return true;
}


bool
PresetsViewStep::isAtEnd() const
{
    return true;
}


Calamares::JobList
PresetsViewStep::jobs() const
{
    return Calamares::JobList();
}


void
PresetsViewStep::onActivate()
{
}

void
PresetsViewStep::onLeave()
{
    m_config.finalizeGlobalStorage();
}

void
PresetsViewStep::nextIsReady()
{
    m_nextEnabled = true;
    emit nextStatusChanged( true );
}

void
PresetsViewStep::setConfigurationMap( const QVariantMap& configurationMap )
{
    m_config.setConfigurationMap( configurationMap );
}

void
PresetsViewStep::updateNextEnabled( bool enabled )
{
    m_nextEnabled = enabled;
    emit nextStatusChanged( enabled );
}
