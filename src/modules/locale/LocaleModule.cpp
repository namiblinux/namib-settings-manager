/*
 *  This file is part of Namib Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Namib Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Namib Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Namib Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LocaleCommon.h"
#include "LocaleModule.h"
#include "MsmCommon.h"
#include "ui_LocaleModule.h"

#include <KAboutData>
#include <KAuth>
#include <KAuthAction>
#include <KPluginFactory>

#include <QtCore/QTranslator>
#include <QtWidgets/QMenu>

#include <QDebug>

K_PLUGIN_FACTORY( MsmLocaleFactory,
                  registerPlugin<LocaleModule>( LocaleCommon::getName() ); )

LocaleModule::LocaleModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::LocaleModule ),
    m_enabledLocalesModel( new EnabledLocalesModel ),
    m_languageListViewDelegate( new LanguageListViewDelegate )
{
    Q_INIT_RESOURCE( translations );
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( LocaleCommon::getName(),
                                            LocaleCommon::getTitle(),
                                            PROJECT_VERSION,
                                            LocaleCommon::getDescription(),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "(c) 2014 - 2017 Namib Settings Manager developers" );
    aboutData->addAuthor( "Ramon Buldó",
                          QStringLiteral( "ramon@manjaro.org" ) );
    aboutData->addAuthor( "Roland Singer",
                          QStringLiteral( "roland@manjaro.org" ) );
    aboutData->setCustomAuthorText( QString(), MsmCommon::getBugReportLink() );
    setAboutData( aboutData );
    setButtons( KCModule::Default | KCModule::Apply );

    ui->setupUi( this );

    // Set Models
    ui->localeListView->setModel( m_enabledLocalesModel );
    ui->addressComboBox->setModel( m_enabledLocalesModel );
    ui->collateComboBox->setModel( m_enabledLocalesModel );
    ui->ctypeComboBox->setModel( m_enabledLocalesModel );
    ui->identificationComboBox->setModel( m_enabledLocalesModel );
    ui->langComboBox->setModel( m_enabledLocalesModel );
    ui->measurementComboBox->setModel( m_enabledLocalesModel );
    ui->messagesComboBox->setModel( m_enabledLocalesModel );
    ui->monetaryComboBox->setModel( m_enabledLocalesModel );
    ui->nameComboBox->setModel( m_enabledLocalesModel );
    ui->numericComboBox->setModel( m_enabledLocalesModel );
    ui->paperComboBox->setModel( m_enabledLocalesModel );
    ui->telephoneComboBox->setModel( m_enabledLocalesModel );
    ui->timeComboBox->setModel( m_enabledLocalesModel );

    ui->buttonAdd->setIcon( QIcon::fromTheme( "list-add" ) );
    ui->buttonRemove->setIcon( QIcon::fromTheme( "list-remove" ) );
    // Set localeListView delegate
    ui->localeListView->setItemDelegate( m_languageListViewDelegate );

    // Connect top buttons signal/slot
    connect( ui->buttonRemove, &QPushButton::clicked,
             this, &LocaleModule::removeLocale );
    connect( ui->buttonAdd, &QPushButton::clicked,
             this, &LocaleModule::addLocale );

    // Hide restore top button only usefull for non kcm module, use kmodule buttom button
    ui->buttonRestore->setVisible( false );

    // Connect "System Locales" tab signal/slots
    connect( ui->localeListView->selectionModel(), &QItemSelectionModel::currentRowChanged,
             this, &LocaleModule::disableRemoveButton );
    connect( ui->localeListView, &QListView::doubleClicked,
             [=] ( const QModelIndex &index )
    {
        m_enabledLocalesModel->setLang( index );
        m_enabledLocalesModel->setLanguage( index );
        m_enabledLocalesModel->setCtype( index );
        m_enabledLocalesModel->setCollate( index );
        m_enabledLocalesModel->setMessages( index );
        m_enabledLocalesModel->setAddress( index );
        m_enabledLocalesModel->setIdentification( index );
        m_enabledLocalesModel->setMeasurement( index );
        m_enabledLocalesModel->setMonetary( index );
        m_enabledLocalesModel->setName( index );
        m_enabledLocalesModel->setNumeric( index );
        m_enabledLocalesModel->setPaper( index );
        m_enabledLocalesModel->setTelephone( index );
        m_enabledLocalesModel->setTime( index );
    } );

    // Connect "Detailed Settings" tab signal/slots
    connect( ui->addressComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setAddress( index );
    } );
    connect( ui->collateComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setCollate( index );
    } );
    connect( ui->ctypeComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setCtype( index );
    } );
    connect( ui->identificationComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setIdentification( index );
    } );
    connect( ui->langComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setLang( index );
    } );
    connect( ui->measurementComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setMeasurement( index );
    } );
    connect( ui->messagesComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setMessages( index );
    } );
    connect( ui->monetaryComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setMonetary( index );
    } );
    connect( ui->nameComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setName( index );
    } );
    connect( ui->numericComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setNumeric( index );
    } );
    connect( ui->paperComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setPaper( index );
    } );
    connect( ui->telephoneComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setTelephone( index );
    } );
    connect( ui->timeComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = m_enabledLocalesModel->index( row, 0 );
        m_enabledLocalesModel->setTime( index );
    } );

    connect( m_enabledLocalesModel, &QAbstractListModel::dataChanged,
             [=] ( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int>& roles )
    {
        Q_UNUSED( bottomRight );
        if ( roles.contains( EnabledLocalesModel::AddressRole ) )
        {
            ui->addressComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::CollateRole ) )
        {
            ui->collateComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::CtypeRole ) )
        {
            ui->ctypeComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::IdentificationRole ) )
        {
            ui->identificationComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::LangRole ) )
        {
            ui->langComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::MeasurementRole ) )
        {
            ui->measurementComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::MonetaryRole ) )
        {
            ui->monetaryComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::MessagesRole ) )
        {
            ui->messagesComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::NameRole ) )
        {
            ui->nameComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::NumericRole ) )
        {
            ui->numericComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::PaperRole ) )
        {
            ui->paperComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::TelephoneRole ) )
        {
            ui->telephoneComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::TimeRole ) )
        {
            ui->timeComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
    } );

    // Context menu for the list view in "System Locales" tab
    connect( ui->localeListView, &QListView::customContextMenuRequested,
             [=] ( const QPoint &pos )
    {
        QMenu localeListViewMenu;
        localeListViewMenu.addAction( ui->setRegionAndFormatsAction );
        localeListViewMenu.addAction( ui->setRegionAction );
        localeListViewMenu.addAction( ui->setFormatsAction );
        localeListViewMenu.exec( ui->localeListView->mapToGlobal( pos ) );
    } );

    connect( ui->setRegionAndFormatsAction, &QAction::triggered,
             [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        m_enabledLocalesModel->setLang( index );
        m_enabledLocalesModel->setLanguage( index );
        m_enabledLocalesModel->setCtype( index );
        m_enabledLocalesModel->setCollate( index );
        m_enabledLocalesModel->setMessages( index );
        m_enabledLocalesModel->setAddress( index );
        m_enabledLocalesModel->setIdentification( index );
        m_enabledLocalesModel->setMeasurement( index );
        m_enabledLocalesModel->setMonetary( index );
        m_enabledLocalesModel->setName( index );
        m_enabledLocalesModel->setNumeric( index );
        m_enabledLocalesModel->setPaper( index );
        m_enabledLocalesModel->setTelephone( index );
        m_enabledLocalesModel->setTime( index );
    } );

    connect( ui->setRegionAction, &QAction::triggered,
             [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        m_enabledLocalesModel->setLang( index );
        m_enabledLocalesModel->setLanguage( index );
        m_enabledLocalesModel->setCtype( index );
        m_enabledLocalesModel->setCollate( index );
        m_enabledLocalesModel->setMessages( index );
    } );

    connect( ui->setFormatsAction, &QAction::triggered,
             [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        m_enabledLocalesModel->setAddress( index );
        m_enabledLocalesModel->setIdentification( index );
        m_enabledLocalesModel->setMeasurement( index );
        m_enabledLocalesModel->setMonetary( index );
        m_enabledLocalesModel->setName( index );
        m_enabledLocalesModel->setNumeric( index );
        m_enabledLocalesModel->setPaper( index );
        m_enabledLocalesModel->setTelephone( index );
        m_enabledLocalesModel->setTime( index );
    } );
}


LocaleModule::~LocaleModule()
{
    delete ui;
    delete m_enabledLocalesModel;
    delete m_languageListViewDelegate;
}


void
LocaleModule::load()
{
    ui->buttonRemove->setDisabled( true );
    m_enabledLocalesModel->init();
    m_enabledLocalesModel->updateSystemLocales();
    m_isLocaleListModified = false;
    m_isSystemLocalesModified = false;
}


void
LocaleModule::save()
{
    if ( m_isLocaleListModified || m_isSystemLocalesModified )
    {

        QVariantMap args;
        args["isLocaleListModified"] = m_isLocaleListModified;
        args["isSystemLocalesModified"] = m_isSystemLocalesModified;
        args["locales"] = m_enabledLocalesModel->locales();

        QStringList localeList;
        if ( !m_enabledLocalesModel->lang().isEmpty() )
            localeList << QString( "LANG=%1" ).arg( m_enabledLocalesModel->lang() );
        if ( !m_enabledLocalesModel->language().isEmpty() )
            localeList << QString( "LANGUAGE=%1" ).arg( m_enabledLocalesModel->language() );
        if ( !m_enabledLocalesModel->ctype().isEmpty() )
            localeList << QString( "LC_CTYPE=%1" ).arg( m_enabledLocalesModel->ctype() );
        if ( !m_enabledLocalesModel->numeric().isEmpty() )
            localeList << QString( "LC_NUMERIC=%1" ).arg( m_enabledLocalesModel->numeric() );
        if ( !m_enabledLocalesModel->time().isEmpty() )
            localeList << QString( "LC_TIME=%1" ).arg( m_enabledLocalesModel->time() );
        if ( !m_enabledLocalesModel->collate().isEmpty() )
            localeList << QString( "LC_COLLATE=%1" ).arg( m_enabledLocalesModel->collate() );
        if ( !m_enabledLocalesModel->monetary().isEmpty() )
            localeList << QString( "LC_MONETARY=%1" ).arg( m_enabledLocalesModel->monetary() );
        if ( !m_enabledLocalesModel->messages().isEmpty() )
            localeList << QString( "LC_MESSAGES=%1" ).arg( m_enabledLocalesModel->messages() );
        if ( !m_enabledLocalesModel->paper().isEmpty() )
            localeList << QString( "LC_PAPER=%1" ).arg( m_enabledLocalesModel->paper() );
        if ( !m_enabledLocalesModel->name().isEmpty() )
            localeList << QString( "LC_NAME=%1" ).arg( m_enabledLocalesModel->name() );
        if ( !m_enabledLocalesModel->address().isEmpty() )
            localeList << QString( "LC_ADDRESS=%1" ).arg( m_enabledLocalesModel->address() );
        if ( !m_enabledLocalesModel->telephone().isEmpty() )
            localeList << QString( "LC_TELEPHONE=%1" ).arg( m_enabledLocalesModel->telephone() );
        if ( !m_enabledLocalesModel->measurement().isEmpty() )
            localeList << QString( "LC_MEASUREMENT=%1" ).arg( m_enabledLocalesModel->measurement() );
        if ( !m_enabledLocalesModel->identification().isEmpty() )
            localeList << QString( "LC_IDENTIFICATION=%1" ).arg( m_enabledLocalesModel->identification() );

        args["localeList"] = localeList;

        // TODO: Progress UI
        KAuth::Action installAction( QLatin1String( "org.namib.msm.locale.save" ) );
        installAction.setHelperId( QLatin1String( "org.namib.msm.locale" ) );
        installAction.setArguments( args );
        installAction.setTimeout( 60000 );
        KAuth::ExecuteJob* job = installAction.execute();
        if ( job->exec() )
            qDebug() << "Locale changes succesfully set";
        else
            qDebug() << "Failed to set locale";

        load();
    }
}


void
LocaleModule::defaults()
{
    this->load();
}


void
LocaleModule::addLocale()
{
    SelectLocalesDialog dialog( this );
    dialog.exec();

    if ( !dialog.localeAdded() )
        return;

    QString localeCode = dialog.getLocale();
    if ( m_enabledLocalesModel->insertLocale( m_enabledLocalesModel->rowCount( QModelIndex() ), 1, localeCode ) )
    {
        m_isLocaleListModified = true;
        emit changed();
    }
}


void
LocaleModule::removeLocale()
{
    QModelIndex localeCurrentIndex = ui->localeListView->currentIndex();
    if ( localeCurrentIndex.isValid() )
    {
        if ( m_enabledLocalesModel->removeLocale( localeCurrentIndex.row(), 1 ) )
        {
            ui->localeListView->setCurrentIndex( QModelIndex() );
            m_isLocaleListModified = true;
            emit changed();
        }
    }
}


/*
 * Disables the remove button if only one locale is in the list or no locale is selected
 */
void
LocaleModule::disableRemoveButton( const QModelIndex& current, const QModelIndex& previous )
{
    if ( m_enabledLocalesModel->rowCount( QModelIndex() ) == 1 )
        ui->buttonRemove->setDisabled( true );
    else
    {
        if ( current.isValid() && !previous.isValid() )
            ui->buttonRemove->setDisabled( false );
        else if ( !current.isValid() && previous.isValid() )
            ui->buttonRemove->setDisabled( true );
    }
}

#include "LocaleModule.moc"
