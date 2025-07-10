/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#include "SectionItem.h"

SectionItem::SectionItem( const Section& section, QWidget* parent )
    : QFrame( parent )
    , m_section( section )
{
    setObjectName( "SectionItem" );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    // setFixedSize( 100, 140 );
    setMinimumSize( 100, 240 );
    setStyleSheet( "QFrame#SectionItem { border: 1px solid gray; border-radius: 12px; }" );

    QBoxLayout* containerLayout = new QBoxLayout( QBoxLayout::TopToBottom, this );
    containerLayout->setContentsMargins( 20, 20, 20, 20 );
    QWidget* container = new QWidget( this );

    QVBoxLayout* layout = new QVBoxLayout( container );

    QLabel* m_iconLabel = new QLabel( container );
    m_iconLabel->setAlignment( Qt::AlignCenter );

    QPixmap pixmap = section.icon();
    m_iconLabel->setPixmap( pixmap.scaled( 128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );

    QSpacerItem* spacer1 = new QSpacerItem( 10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum );

    QLabel* m_titleLabel = new QLabel( section.label(), container );
    m_titleLabel->setAlignment( Qt::AlignCenter );
    m_titleLabel->setStyleSheet( "font-size: 16pt;" );

    QSpacerItem* spacer2 = new QSpacerItem( 10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum );

    QLabel* m_descriptionLabel = new QLabel( section.description(), container );
    m_descriptionLabel->setWordWrap( true );
    m_descriptionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    layout->addWidget( m_iconLabel );
    layout->addItem( spacer1 );
    layout->addWidget( m_titleLabel );
    layout->addItem( spacer2 );
    layout->addWidget( m_descriptionLabel );
    container->setLayout( layout );
    containerLayout->addWidget( container );
    containerLayout->setAlignment( Qt::AlignCenter );
}

void
SectionItem::setSelected( bool selected )
{
    m_selected = selected;
    setStyleSheet( QString( "QFrame#SectionItem { border: 1px solid %1; border-radius: 12px; }" )
                       .arg( selected ? "lightblue" : "gray" ) );
}

void
SectionItem::mousePressEvent( QMouseEvent* event )
{
    emit clicked( this );
}
