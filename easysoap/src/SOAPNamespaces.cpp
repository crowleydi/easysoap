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

const SOAPQName SOAPEnc::root("root", SOAP_ENC);
const SOAPQName SOAPEnc::Array("Array", SOAP_ENC);
const SOAPQName SOAPEnc::arrayType("arrayType", SOAP_ENC);
const SOAPQName SOAPEnc::offset("offset", SOAP_ENC);
const SOAPQName SOAPEnc::position("position", SOAP_ENC);
const SOAPQName SOAPEnc::base64("base64", SOAP_ENC);

const SOAPQName SOAPEnv::Body("Body", SOAP_ENV);
const SOAPQName SOAPEnv::Header("Header", SOAP_ENV);
const SOAPQName SOAPEnv::Envelope("Envelope", SOAP_ENV);
const SOAPQName SOAPEnv::Fault("Fault", SOAP_ENV);
const SOAPQName SOAPEnv::encodingStyle("encodingStyle", SOAP_ENV);
const SOAPQName SOAPEnv::mustUnderstand("mustUnderstand", SOAP_ENV);
const SOAPQName SOAPEnv::actor("actor", SOAP_ENV);

const SOAPQName XMLSchemaInstance::nil("nil", SOAP_XSI);
const SOAPQName XMLSchemaInstance::type("type", SOAP_XSI);

