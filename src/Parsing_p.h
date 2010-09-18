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

#ifndef ECHONEST_PARSING_P_H
#define ECHONEST_PARSING_P_H

#include "Config.h"

#include <QXmlStreamReader>
#include "Song.h"

class QNetworkReply;

namespace Echonest
{
namespace Parser
{
    /**
    * Internal helper parsing functions for QXmlStreamParser
    */

    void checkForErrors( QNetworkReply* reply ) throw( ParseError );
    
    // read the start element and then the status element, throwing 
    //  if the result code is not Success
    void readStatus( QXmlStreamReader& xml ) throw( ParseError );
    
    // parses a <songs></songs> block and turns them into a list of Song object
    QVector< Song > parseSongList( QXmlStreamReader& xml ) throw( ParseError );
   
    // parses a <song></song> block
    Song parseSong( QXmlStreamReader& xml ) throw( ParseError );
    
    // parses a <track></track> block
    Track parseTrack( QXmlStreamReader& xml ) throw( ParseError );
    
    // parses an <audio_summary> chunk
    AudioSummary parseAudioSummary( QXmlStreamReader& xml ) throw( ParseError );
}
}

#endif