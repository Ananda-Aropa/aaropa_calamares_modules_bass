/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#ifndef PRESETS_CONFIG_H
#define PRESETS_CONFIG_H

#include "locale/TranslatableConfiguration.h"
#include "modulesystem/InstanceKey.h"
#include "utils/NamedEnum.h"

#include <memory>

#include "section.h"

class Config : public QObject
{
    Q_OBJECT

    // Translations, of the module name (for sidebar) and above the list
    Q_PROPERTY( QString titleLabel READ titleLabel NOTIFY titleLabelChanged FINAL )
    Q_PROPERTY( QString subtitleLabel READ subtitleLabel NOTIFY subtitleLabelChanged FINAL )
    Q_PROPERTY( QString sidebarLabel READ sidebarLabel NOTIFY sidebarLabelChanged FINAL )

public:
    Config( QObject* parent = nullptr );
    ~Config() override;

    void setConfigurationMap( const QVariantMap& configurationMap );

    QString titleLabel() const;
    QString subtitleLabel() const;
    QString sidebarLabel() const;

    QList< Section > sections() const { return m_sections; }

    Section selectedSection() const { return m_sections.at( m_selected ); }
    void setSelectedSection( int index ) { m_selected = index; }

    bool nextEnabled() const { return m_nextEnabled; }

    /** @brief Write the selected option lists to global storage
     *
     * Since the config doesn't know what module it is for,
     * pass in an instance key.
     */
    void finalizeGlobalStorage();

Q_SIGNALS:
    void titleLabelChanged( QString label );
    void subtitleLabelChanged( QString label );
    void sidebarLabelChanged( QString label );
    void statusReady();
    void nextReady();

public Q_SLOTS:
    void setNextEnabled();

private Q_SLOTS:
    void retranslate();
    void loadingDone();

private:
    Calamares::Locale::TranslatedString* m_titleLabel = nullptr;
    Calamares::Locale::TranslatedString* m_subtitleLabel = nullptr;
    Calamares::Locale::TranslatedString* m_sidebarLabel = nullptr;

    QList< Section > m_sections = {};
    int m_selected = 0;
    bool m_nextEnabled = false;
};

#endif
