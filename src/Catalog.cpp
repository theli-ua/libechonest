/****************************************************************************************
 * Copyright (c) 2010 Leo Franchi <lfranchi@kde.org>                                    *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#include "Catalog.h"

#include "Catalog_p.h"
#include "Parsing_p.h"

Echonest::Catalog::Catalog()
    : d( new CatalogData )
{

}

Echonest::Catalog::Catalog(const QString& name)
    : d( new CatalogData )
{
    d->name = name;
}


Echonest::Catalog::Catalog( const QByteArray& id )
    : d( new CatalogData )
{
    d->id = id;
}


Echonest::Catalog::Catalog( const Echonest::Catalog& catalog )
    : d( catalog.d )
{
}

Echonest::Catalog& Echonest::Catalog::operator=( const Echonest::Catalog& other )
{
    d = other.d;
    return *this;
}

Echonest::Catalog::~Catalog()
{
}

Echonest::CatalogArtists Echonest::Catalog::artists() const
{
    return d->artists;
}

void Echonest::Catalog::setArtists(const Echonest::CatalogArtists& artists)
{
    d->artists = artists;
}

QByteArray Echonest::Catalog::id() const
{
    return d->id;
}

void Echonest::Catalog::setId(const QByteArray& id)
{
    d->id = id;
}

QString Echonest::Catalog::name() const
{
    return d->name;
}

void Echonest::Catalog::setName(const QString& name)
{
    d->name = name;
}

int Echonest::Catalog::resolved() const
{
    return d->resolved;
}

void Echonest::Catalog::setResolved(int resolved)
{
    d->resolved = resolved;
}

int Echonest::Catalog::pendingTickets() const
{
//     return d->pending_tickets;
    return 0;
}

void Echonest::Catalog::setPendingTickets(int pending)
{
//     d->pending_tickets = pending;
}

Echonest::CatalogSongs Echonest::Catalog::songs() const
{
    return d->songs;
}

void Echonest::Catalog::setSongs(const Echonest::CatalogSongs& songs)
{
    d->songs = songs;
}

int Echonest::Catalog::total() const
{
    return d->total;
}

void Echonest::Catalog::setTotal(int total)
{
    d->total = total;
}

Echonest::CatalogTypes::Type Echonest::Catalog::type() const
{
    return d->type;
}

void Echonest::Catalog::setType(Echonest::CatalogTypes::Type type)
{
    d->type = type;
}

QNetworkReply* Echonest::Catalog::create(const QString& name, Echonest::CatalogTypes::Type type)
{
    QUrl url = Echonest::baseGetQuery( "catalog", "create" );
    url.addQueryItem( QLatin1String( "name" ), name );
    url.addEncodedQueryItem( "type", Echonest::catalogTypeToLiteral( type ) );
    
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, QLatin1String( "multipart/form-data" ) );
    qDebug() << "Creating catalog::create URL" << url;
    return Echonest::Config::instance()->nam()->post( request, QByteArray() );
}

QNetworkReply* Echonest::Catalog::deleteCatalog() const
{
    QUrl url = Echonest::baseGetQuery( "catalog", "delete" );
    Q_ASSERT( !d->isEmpty() );
    url.addEncodedQueryItem( "id", d->id );
    
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, QLatin1String( "multipart/form-data" ) );
    qDebug() << "Creating catalog::delete URL" << url;
    return Echonest::Config::instance()->nam()->post( request, QByteArray() );
}

QNetworkReply* Echonest::Catalog::list(int results, int start)
{
    QUrl url = Echonest::baseGetQuery( "catalog", "list" );
    addLimits( url, results, start );
    
    return Echonest::Config::instance()->nam()->get( QNetworkRequest( url ) );    
}

QNetworkReply* Echonest::Catalog::profile() const
{
    QUrl url = Echonest::baseGetQuery( "catalog", "profile" );
    if( !d->id.isEmpty() )
        url.addEncodedQueryItem( "id", d->id );
    else if( !d->name.isEmpty() )
        url.addQueryItem( QLatin1String( "name" ), d->name );
    else
        Q_ASSERT_X( false, "Catalog", "Not enough information!" );
    
    return Echonest::Config::instance()->nam()->get( QNetworkRequest( url ) );    
}

QNetworkReply* Echonest::Catalog::status(const QByteArray& ticket)
{
    QUrl url = Echonest::baseGetQuery( "catalog", "status" );
    Q_ASSERT( !d->isEmpty() );
    url.addEncodedQueryItem( "id", d->id );
    
    return Echonest::Config::instance()->nam()->get( QNetworkRequest( url ) );    
}

QNetworkReply* Echonest::Catalog::update(const Echonest::CatalogEntryList& entries) const
{
    QUrl url = Echonest::baseGetQuery( "catalog", "update" );
    Q_ASSERT( !d->isEmpty() );
    url.addEncodedQueryItem( "id", d->id );
    return Echonest::Catalog::updatePrivate( url, entries );
}

QNetworkReply* Echonest::Catalog::updateAndCreate(const Echonest::CatalogEntryList& entries)
{
    QUrl url = Echonest::baseGetQuery( "catalog", "update" );
    return Echonest::Catalog::updatePrivate( url, entries );
}

QNetworkReply* Echonest::Catalog::readArtistCatalog(Echonest::Artist::ArtistInformation info, int results, int start) const
{
    QUrl url = Echonest::baseGetQuery( "catalog", "read" );
    Artist::addQueryInformation( url, info );
    
    return readPrivate( url, results, start ); 
}

QNetworkReply* Echonest::Catalog::readSongCatalog(Echonest::Song::SongInformation info, int results, int start) const
{
    QUrl url = Echonest::baseGetQuery( "catalog", "read" );
    Song::addQueryInformation( url, info );
    
    return readPrivate( url, results, start ); 
}

QPair< QString, QByteArray > Echonest::Catalog::parseDelete( QNetworkReply* reply ) throw( Echonest::ParseError )
{
    QPair< QString, QByteArray > asd;
    Echonest::Parser::checkForErrors( reply );
    
    QXmlStreamReader xml( reply->readAll() );
    
    Echonest::Parser::readStatus( xml );
    
    // TODO, after create works :)
    return asd;
}

Echonest::Catalogs Echonest::Catalog::parseList(QNetworkReply* reply) throw( Echonest::ParseError )
{
    Echonest::Parser::checkForErrors( reply );
    QXmlStreamReader xml( reply->readAll() );
    Echonest::Parser::readStatus( xml );
    
    Echonest::Catalogs catalogs = Echonest::Catalogs()/* Echonest::Parser::parseCatalogList( xml )*/;
    
    return catalogs;
}

void Echonest::Catalog::parseProfile(QNetworkReply* ) throw( Echonest::ParseError )
{

}

void Echonest::Catalog::parseRead(QNetworkReply* ) throw( Echonest::ParseError )
{

}

Echonest::CatalogStatus Echonest::Catalog::parseStatus(QNetworkReply* ) throw( Echonest::ParseError )
{
    return Echonest::CatalogStatus();
}

QByteArray Echonest::Catalog::parseTicket(QNetworkReply* ) throw( Echonest::ParseError )
{
    return QByteArray();
}

QNetworkReply* Echonest::Catalog::updatePrivate( QUrl& url, const Echonest::CatalogEntryList& entries)
{
    url.addEncodedQueryItem( "data_type", "json" );
    
    //     QByteArray payload = Generator::catalogEntriesToJson( entries );
    QByteArray payload;
    return Echonest::Config::instance()->nam()->post( QNetworkRequest( url ), payload );
}

void Echonest::Catalog::addLimits(QUrl& url, int results, int start)
{
    if( results != 30 )
        url.addEncodedQueryItem( "results", QString::number( results ).toLatin1() );
    if( start > -1 )
        url.addEncodedQueryItem( "start", QString::number( start ).toLatin1() );
}

QNetworkReply* Echonest::Catalog::readPrivate(QUrl& url, int results, int start) const
{
    Q_ASSERT( !d->isEmpty() );
    url.addEncodedQueryItem( "id", d->id );
    addLimits( url, results, start );
    
    return Echonest::Config::instance()->nam()->get( QNetworkRequest( url ) ); 
}