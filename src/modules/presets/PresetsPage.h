/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#ifndef PRESETSPAGE_H
#define PRESETSPAGE_H

#include "Config.h"
#include "SectionItem.h"

#include "locale/TranslatableConfiguration.h"

#include <QString>
#include <QWidget>

#include <memory>

namespace Ui
{
class Page_Presets;
}  // namespace Ui

class PresetsPage : public QWidget
{
    Q_OBJECT
public:
    PresetsPage( Config* config, QWidget* parent = nullptr );
    ~PresetsPage() override;

    void showSelections();

private slots:
    void onSectionClicked(SectionItem *widget);

private:
    Config* m_config;
    Ui::Page_Presets* ui;
    QList<SectionItem *> m_widgets;
};

#endif  // PRESETSPAGE_H
