/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2016 Luca Giambonini <almack@chakraos.org>
 *   SPDX-FileCopyrightText: 2016 Lisa Vitolo <shainer@chakraos.org>
 *   SPDX-FileCopyrightText: 2017 Kyle Robbertze  <krobbertze@gmail.com>
 *   SPDX-FileCopyrightText: 2017-2018 2020, Adriaan de Groot <groot@kde.org>
 *   SPDX-FileCopyrightText: 2023 Vladislav Nepogodin <nepogodin.vlad@gmail.com>
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện (BlissLabs) <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */


#include <QNetworkReply>
#include <QUrl>
#include <QVariantList>

#include "OptionsPage.h"
#include "OptionsViewStep.h"

#include "GlobalStorage.h"
#include "JobQueue.h"

#include "network/Manager.h"
#include "utils/RAII.h"
#include "utils/Yaml.h"

#include "utils/Logger.h"
#include "utils/Retranslator.h"
#include "utils/Variant.h"

CALAMARES_PLUGIN_FACTORY_DEFINITION( OptionsViewStepFactory, registerPlugin< OptionsViewStep >(); )

OptionsViewStep::OptionsViewStep( QObject* parent )
    : Calamares::ViewStep( parent )
    , m_widget( new OptionsPage( &m_config ) )
    , m_nextEnabled( false )
{
    connect( &m_config, &Config::statusReady, this, &OptionsViewStep::nextIsReady );

    m_config.model()->setUpdateNextCall( [ this ]( bool enabled ) { this->updateNextEnabled( enabled ); } );
}


OptionsViewStep::~OptionsViewStep()
{
    if ( m_widget && m_widget->parent() == nullptr )
    {
        m_widget->deleteLater();
    }
}


QString
OptionsViewStep::prettyName() const
{
    return m_config.sidebarLabel();
}


QWidget*
OptionsViewStep::widget()
{
    return m_widget;
}


bool
OptionsViewStep::isNextEnabled() const
{
    return !m_config.required() || m_nextEnabled;
}


bool
OptionsViewStep::isBackEnabled() const
{
    return true;
}


bool
OptionsViewStep::isAtBeginning() const
{
    return true;
}


bool
OptionsViewStep::isAtEnd() const
{
    return true;
}


Calamares::JobList
OptionsViewStep::jobs() const
{
    return Calamares::JobList();
}


void
OptionsViewStep::fetch( const QUrl& url )
{
    if ( !url.isValid() )
    {
        ( &m_config )->setStatus( Config::Status::FailedBadConfiguration );
        cDebug() << "Invalid URL" << url;
        return;
    }

    using namespace Calamares::Network;

    cDebug() << "Options loading groups from" << url;
    QNetworkReply* reply = Manager().asynchronousGet(
        url,
        RequestOptions( RequestOptions::FakeUserAgent | RequestOptions::FollowRedirect, std::chrono::seconds( 30 ) ) );

    if ( !reply )
    {
        cDebug() << Logger::SubEntry << "Request failed immediately.";
        // If nobody sets a different status, this will remain
        ( &m_config )->setStatus( Config::Status::FailedBadConfiguration );
    }
    else
    {
        // When the network request is done, **then** we might
        // do the next item from the queue, so don't call fetchNext() now.
        m_reply = reply;
        connect( reply, &QNetworkReply::finished, this, &OptionsViewStep::dataArrived );
    }
}

void
OptionsViewStep::dataArrived()
{
    if ( !m_reply || !m_reply->isFinished() )
    {
        cWarning() << "Options data called too early.";
        ( &m_config )->setStatus( Config::Status::FailedInternalError );
        return;
    }

    cDebug() << "Options group data received" << m_reply->size() << "bytes from" << m_reply->url();

    cqDeleter< QNetworkReply > d { m_reply };

    // If m_required is *false* then we still say we're ready
    // even if the reply is corrupt or missing.
    if ( m_reply->error() != QNetworkReply::NoError )
    {
        cWarning() << "unable to fetch options option lists.";
        cDebug() << Logger::SubEntry << "Options reply error: " << m_reply->error();
        cDebug() << Logger::SubEntry << "Request for url: " << m_reply->url().toString()
                 << " failed with: " << m_reply->errorString();
        ( &m_config )->setStatus( Config::Status::FailedNetworkError );
        return;
    }

    QByteArray yamlData = m_reply->readAll();
    try
    {
        auto groups = ::YAML::Load( yamlData.constData() );

        if ( groups.IsSequence() )
        {
            ( &m_config )->loadGroupList( Calamares::YAML::sequenceToVariant( groups ) );
            ( &m_config )->setStatus( Config::Status::Ok );
            ( &m_config )->loadingDone();
        }
        else if ( groups.IsMap() )
        {
            auto map = Calamares::YAML::mapToVariant( groups );
            ( &m_config )->loadGroupList( map.value( "groups" ).toList() );
            ( &m_config )->setStatus( Config::Status::Ok );
            ( &m_config )->loadingDone();
        }
        else
        {
            cWarning() << "Options groups data does not form a sequence.";
        }
    }
    catch ( ::YAML::Exception& e )
    {
        Calamares::YAML::explainException( e, yamlData, "options groups data" );
        ( &m_config )->setStatus( Config::Status::FailedBadData );
    }
}

void
OptionsViewStep::onActivate()
{
    auto* gs = Calamares::JobQueue::instance() ? Calamares::JobQueue::instance()->globalStorage() : nullptr;
    if ( gs && gs->contains( "presets" ) )
    {
        QVariantMap presets = gs->value( "presets" ).toMap();
        cDebug() << "Loading options from " << presets;
        const QString url = presets.value( "selection" ).toString();
        cDebug() << "Loading options from" << url;
        if ( !url.isEmpty() )
        {
            fetch( QUrl { url } );
        }
    }
    m_widget->onActivate();
}

void
OptionsViewStep::onLeave()
{
    m_config.finalizeGlobalStorage();
}

void
OptionsViewStep::nextIsReady()
{
    m_nextEnabled = true;
    emit nextStatusChanged( true );
}

void
OptionsViewStep::setConfigurationMap( const QVariantMap& configurationMap )
{
    m_config.setConfigurationMap( configurationMap );
}

void
OptionsViewStep::updateNextEnabled( bool enabled )
{
    m_nextEnabled = enabled;
    emit nextStatusChanged( enabled );
}
