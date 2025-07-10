/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#ifndef PRESETSVIEWSTEP_H
#define PRESETSVIEWSTEP_H

#include "Config.h"

#include "GlobalStorage.h"
#include "JobQueue.h"

#include "DllMacro.h"
#include "utils/PluginFactory.h"
#include "viewpages/ViewStep.h"

#include <QVariant>

class PresetsPage;

class PLUGINDLLEXPORT PresetsViewStep : public Calamares::ViewStep
{
    Q_OBJECT

public:
    explicit PresetsViewStep( QObject* parent = nullptr );
    ~PresetsViewStep() override;

    QString prettyName() const override;

    QWidget* widget() override;

    bool isNextEnabled() const override;
    bool isBackEnabled() const override;

    bool isAtBeginning() const override;
    bool isAtEnd() const override;

    Calamares::JobList jobs() const override;

    void onActivate() override;

    void onLeave() override;

    void setConfigurationMap( const QVariantMap& configurationMap ) override;

    void updateNextEnabled( bool enabled );

public slots:
    void nextIsReady();

private:
    Config m_config;

    PresetsPage* m_widget;
    bool m_nextEnabled = true;
};

CALAMARES_PLUGIN_FACTORY_DECLARATION( PresetsViewStepFactory )

#endif  // PRESETSVIEWSTEP_H
