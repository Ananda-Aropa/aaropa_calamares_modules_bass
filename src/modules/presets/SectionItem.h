/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#ifndef PRESETS_SECTIONITEM_H
#define PRESETS_SECTIONITEM_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

#include "section.h"
#include <QMouseEvent>

class SectionItem : public QFrame {
    Q_OBJECT
public:
    explicit SectionItem(const Section &section, QWidget *parent = nullptr);
    void setSelected(bool selected);
    const Section &section() const { return m_section; };

signals:
    void clicked(SectionItem *widget);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Section m_section;
    bool m_selected = false;
};

#endif // PRESETS_SECTIONITEM_H
