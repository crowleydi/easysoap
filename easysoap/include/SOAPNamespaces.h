/* 
 * EasySoap++ - A C++ library for SOAP (Simple Object Access Protocol)
 * Copyright (C) 2001 David Crowley; SciTegic, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef __SOAPNamesSpaces_h__
#define __SOAPNamesSpaces_h__

#define FULL_SOAP_ENV "http://schemas.xmlsoap.org/soap/envelope/"
#define FULL_SOAP_ENC "http://schemas.xmlsoap.org/soap/encoding/"
#define FULL_SOAP_XSI_1999 "http://www.w3.org/1999/XMLSchema-instance"
#define FULL_SOAP_XSD_1999 "http://www.w3.org/1999/XMLSchema"
#define FULL_SOAP_XSI_2001 "http://www.w3.org/2001/XMLSchema-instance"
#define FULL_SOAP_XSD_2001 "http://www.w3.org/2001/XMLSchema"
#define FULL_SOAP_XSI FULL_SOAP_XSI_1999
#define FULL_SOAP_XSD FULL_SOAP_XSD_1999

#define TAG_SOAP_ENV "SOAP-ENV"
#define TAG_SOAP_ENC "SOAP-ENC"
#define TAG_SOAP_XSD "xsd"
#define TAG_SOAP_XSI "xsi"

#define PARSER_NS_SEP "#"

#endif // __SOAPNamesSpaces_h__
