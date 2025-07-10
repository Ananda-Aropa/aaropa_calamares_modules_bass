/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2025 Bùi Gia Viện <shadichy@blisslabs.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#include <QHBoxLayout>
#include <QList>
#include <QVBoxLayout>

#include "PresetsPage.h"

#include "ui_presets.h"

#include "utils/Logger.h"
#include "utils/Retranslator.h"
#include "utils/Yaml.h"

PresetsPage::PresetsPage( Config* c, QWidget* parent )
    : QWidget( parent )
    , m_config( c )
    , ui( new Ui::Page_Presets )
    , m_widgets( QList< SectionItem* >() )
{
    ui->setupUi( this );
    connect( c,
             &Config::titleLabelChanged,
             [ ui = this->ui ]( const QString title )
             {
                 ui->label->setVisible( !title.isEmpty() );
                 ui->label->setText( title );
             } );
    connect( c,
             &Config::subtitleLabelChanged,
             [ ui = this->ui ]( const QString title )
             {
                 ui->warn->setVisible( !title.isEmpty() );
                 ui->warn->setText( title );
             } );
    ui->container->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    connect( c, &Config::statusReady, this, &PresetsPage::showSelections );
}

PresetsPage::~PresetsPage() {}

void
PresetsPage::onSectionClicked( SectionItem* widget )
{
    for ( auto w : m_widgets )
    {
        w->setSelected( w == widget );
    }
    m_config->setSelectedSection( m_widgets.indexOf( widget ) );
}

void
PresetsPage::showSelections()
{
    QList< Section > sections = m_config->sections();
    QVBoxLayout* mainLayout = ui->presets_layout;
    mainLayout->setSpacing( 30 );
    mainLayout->setContentsMargins( 10, 10, 10, 10 );

    int count = sections.size();
    int rows = qCeil( count / 3.0 );
    int perRow = qCeil( count / static_cast< double >( rows ) );
    int index = 0;

    for ( int r = 0; r < rows; ++r )
    {
        QHBoxLayout* rowLayout = new QHBoxLayout;
        rowLayout->setSpacing( 30 );
        rowLayout->setAlignment( Qt::AlignCenter );
        for ( int c = 0; c < perRow && index < count; ++c, ++index )
        {
            SectionItem* widget = new SectionItem( sections[ index ] );
            m_widgets.append( widget );
            rowLayout->addWidget( widget );
            rowLayout->setStretch( c, 1 );
            connect( widget, &SectionItem::clicked, this, &PresetsPage::onSectionClicked );
        }
        QWidget* container = new QWidget();
        container->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        container->setLayout( rowLayout );
        mainLayout->addWidget( container );
        mainLayout->setStretch( r, 1 );
    }
}
