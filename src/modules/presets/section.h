/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#ifndef PRESETS_SECTION_H
#define PRESETS_SECTION_H

#include <QObject>
#include <QPixmap>
#include <QString>
#include <QVariantMap>

class Section
{
public:
    Section( const QVariantMap& map );
    Section( const QString& label, const QString& description, const QString& icon, const QString& target );
    Section( const QString& label, const QString& description, const QPixmap& icon, const QString& target );
    ~Section() {}
    QString label() const { return m_label; }
    QString description() const { return m_description; }
    QPixmap icon() const { return m_icon; }
    QString target() const { return m_target; }

private:
    QString m_label;
    QString m_description;
    QPixmap m_icon;
    QString m_target;
};

#endif  // PRESETS_SECTION_H
