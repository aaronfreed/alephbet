/*

	Copyright (C) 1991-2001 and beyond by Bungie Studios, Inc.
	and the "Aleph One" developers.
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	This license is contained in the file "COPYING",
	which is included with this source code; it is available online at
	http://www.gnu.org/licenses/gpl.html

	XML-Data-Block-Parser Implementation
	by Loren Petrich,
	September 14, 2000
	
Oct 24, 2001 (Loren Petrich):
	Added "SourceName" field for indicating source of XML data for easier debugging

Jan 25, 2002 (Br'fin (Jeremy Parsons)):
	Added CopyCStringToPascal for Carbon, in place of c2pstr
*/


#include <string.h>
#include "cseries.h"
#include "XML_DataBlock.h"


#ifdef mac
// Marathon-engine dialog boxes:
const short FatalErrorAlert = 128;
const short NonFatalErrorAlert = 129;
#endif


// Gets some XML data to parse
bool XML_DataBlock::GetData()
{
	// Check...
	assert(Buffer);
	assert(BufLen > 0);
	 
	// Only one buffer
	LastOne = true;

	return true;
}


// Reports a read error
void XML_DataBlock::ReportReadError()
{
	const char *Name = SourceName ? SourceName : "[]";

#ifdef mac
	psprintf(ptemporary,"Error in reading data/resources from object %s",Name);
	ParamText(ptemporary,0,0,0);
	Alert(FatalErrorAlert,NULL);
	ExitToShell();
#endif
	
#ifdef SDL
	fprintf(stderr, "Error in reading data/resources from object %s\n",Name);
	exit(1);
#endif
}


// Reports an XML parsing error
void XML_DataBlock::ReportParseError(const char *ErrorString, int LineNumber)
{
	const char *Name = SourceName ? SourceName : "[]";

#ifdef mac
	psprintf(ptemporary,"XML parsing error: %s at line %d in object %s",ErrorString,LineNumber,Name);
	ParamText(ptemporary,0,0,0);
	Alert(FatalErrorAlert,NULL);
	ExitToShell();
#endif

#ifdef SDL
	fprintf(stderr, "XML parsing error: %s at line %d in object %s\n", ErrorString, LineNumber, Name);
#endif
}


const int MaxErrorsToShow = 7;


// Reports an interpretation error
void XML_DataBlock::ReportInterpretError(const char *ErrorString)
{
#ifdef mac
# if defined(USE_CARBON_ACCESSORS)
	CopyCStringToPascal(ErrorString, (unsigned char *)temporary);
# else
	strncpy(temporary,ErrorString,255);
	c2pstr(temporary);
# endif
	ParamText(ptemporary,0,0,0);
	if (GetNumInterpretErrors() < MaxErrorsToShow)
		Alert(NonFatalErrorAlert,NULL);
#endif

#ifdef SDL
	if (GetNumInterpretErrors() < MaxErrorsToShow)
		fprintf(stderr, ErrorString);
#endif
}

// Requests aborting of parsing (reasonable if there were lots of errors)
bool XML_DataBlock::RequestAbort()
{
	return (GetNumInterpretErrors() >= MaxErrorsToShow);
}
