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
 *
 * $Id$
 */


#include <easysoap/SOAP.h>
#include <easysoap/SOAPNamespaces.h>

#include "es_namespaces.h"

USING_EASYSOAP_NAMESPACE

const char *SOAPEnc::base = SOAP_ENC;

const SOAPQName SOAPEnc::root("root", SOAPEnc::base);
const SOAPQName SOAPEnc::Array("Array", SOAPEnc::base);
const SOAPQName SOAPEnc::arrayType("arrayType", SOAPEnc::base);
const SOAPQName SOAPEnc::offset("offset", SOAPEnc::base);
const SOAPQName SOAPEnc::position("position", SOAPEnc::base);
const SOAPQName SOAPEnc::base64("base64", SOAPEnc::base);

const char *SOAPEnv::base = SOAP_ENV;
const SOAPQName SOAPEnv::Body("Body", SOAPEnv::base);
const SOAPQName SOAPEnv::Header("Header", SOAPEnv::base);
const SOAPQName SOAPEnv::Envelope("Envelope", SOAPEnv::base);
const SOAPQName SOAPEnv::Fault("Fault", SOAPEnv::base);
const SOAPQName SOAPEnv::encodingStyle("encodingStyle", SOAPEnv::base);
const SOAPQName SOAPEnv::mustUnderstand("mustUnderstand", SOAPEnv::base);
const SOAPQName SOAPEnv::actor("actor", SOAPEnv::base);

const char *XMLSchema1999::xsi = SOAP_XSI_1999;
const char *XMLSchema1999::xsd = SOAP_XSD_1999;
const SOAPQName XMLSchema1999::null("null", XMLSchema1999::xsi);
const SOAPQName XMLSchema1999::type("type", XMLSchema1999::xsi);

const char *XMLSchema2001::xsi = SOAP_XSI_2001;
const char *XMLSchema2001::xsd = SOAP_XSD_2001;
const SOAPQName XMLSchema2001::nil("nil", XMLSchema2001::xsi);
const SOAPQName XMLSchema2001::type("type", XMLSchema2001::xsi);

