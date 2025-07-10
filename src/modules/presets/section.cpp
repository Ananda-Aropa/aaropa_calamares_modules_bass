/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#include <QPainter>
#include <QSvgRenderer>

#include "section.h"

QPixmap
loadSvgAsPixmap( const QString& filePath, QSize size = QSize( 128, 128 ) )
{
    QSvgRenderer svg( filePath );
    QPixmap pixmap( size );
    pixmap.fill( Qt::transparent );

    QPainter painter( &pixmap );
    svg.render( &painter );
    return pixmap;
}

Section::Section( const QVariantMap& map )
    : m_label( map.value( "label" ).toString() )
    , m_description( map.value( "description" ).toString() )
    , m_icon( loadSvgAsPixmap( map.value( "icon" ).toString() ) )
    , m_target( map.value( "target" ).toString() )
{
}

Section::Section( const QString& label, const QString& description, const QString& icon, const QString& target )
    : m_label( label )
    , m_description( description )
    , m_icon( loadSvgAsPixmap( icon ) )
    , m_target( target )
{
}

Section::Section( const QString& label, const QString& description, const QPixmap& icon, const QString& target )
    : m_label( label )
    , m_description( description )
    , m_icon( icon )
    , m_target( target )
{
}
