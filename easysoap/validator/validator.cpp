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

//
//
//
//   CGI Program for the Userland SOAP 1.1 Validator Web App
//   http://validator.soapware.org/
//
//
//

#include <SOAP.h>
#include <SOAPCGIHandler.h>

//
//
// Quick method for writing a SOAP Fault
//
// TODO:  Fix things so we can write a Fault
// out the correct way....
//
void
WriteFault(const char *actor, const char *str)
{
	printf("Content-Type: text/xml; charset=\"UTF-8\"\n\n");

	printf("<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\">");
	printf("<SOAP-ENV:Body>");
	printf("<SOAP-ENV:Fault>");
	printf("<faultactor>%s</faultactor>", actor);
	printf("<faultstring>%s</faultstring>", str);
	printf("</SOAP-ENV:Fault>");
	printf("</SOAP-ENV:Body>");
	printf("</SOAP-ENV:Envelope>");
}

//
// validator1.countTheEntities (s)
// returns struct
//
//
// This handler takes a single parameter named s, a string,
// that contains any number of predefined entities, namely <, >,
// &, ' and ".
//
// Your handler must return a struct that contains five fields,
// all numbers: ctLeftAngleBrackets, ctRightAngleBrackets,
// ctAmpersands, ctApostrophes, ctQuotes. 
//
// To validate, the numbers must be correct.
//

void
countTheEntities(const SOAPMethod& req, SOAPMethod& resp)
{
	const SOAPString& s = req.GetParameter("s").GetString();
	const char *str = s;

	int ctAmpersands = 0;
	int ctApostrophes = 0;
	int ctLeftAngleBrackets = 0;
	int ctQuotes = 0;
	int ctRightAngleBrackets = 0;

	while (*str)
	{
		switch (*str++)
		{
		case '&':
			++ctAmpersands;
			break;
		case '\'':
			++ctApostrophes;
			break;
		case '\"':
			++ctQuotes;
			break;
		case '<':
			++ctLeftAngleBrackets;
			break;
		case '>':
			++ctRightAngleBrackets;
			break;
		default:
			break;
		}
	}

	resp.SetName("countTheEntitiesResponse");
	SOAPParameter& result = resp.AddParameter("Result");
	result.AddParameter("ctAmpersands") << ctAmpersands;
	result.AddParameter("ctApostrophes") << ctApostrophes;
	result.AddParameter("ctLeftAngleBrackets") << ctLeftAngleBrackets;
	result.AddParameter("ctQuotes") << ctQuotes;
	result.AddParameter("ctRightAngleBrackets") << ctRightAngleBrackets;
	result.SetIsStruct();
}

//
//
// validator1.easyStructTest (stooges)
// returns number
//
// This handler takes a single parameter named stooges, a struct,
// containing at least three elements named moe, larry and curly,
// all ints. Your handler must add the three numbers and return
// the result.
//


// The struct we have to handle
struct stooges
{
	int larry;
	int curly;
	int moe;
};

// how we de-serialize the stooges struct
const SOAPParameter&
operator>>(const SOAPParameter& param, stooges& st)
{
	param.GetParameter("larry") >> st.larry;
	param.GetParameter("curly") >> st.curly;
	param.GetParameter("moe") >> st.moe;
	return param;
}

// how we serialize ths stooges struct;
SOAPParameter&
operator<<(SOAPParameter& param, const stooges& st)
{
	param.AddParameter("larry") << st.larry;
	param.AddParameter("curly") << st.curly;
	param.AddParameter("moe") << st.moe;
	param.SetIsStruct();
	return param;
}

//
// Now lets do it!
//
void
easyStructTest(const SOAPMethod& req, SOAPMethod& resp)
{
	stooges st;
	const SOAPParameter& p = req.GetParameter("stooges");

	p >> st;

	int sum = st.moe + st.curly + st.larry;

	resp.SetName("easyStructTestResponse");
	resp.AddParameter("Result") << sum;
}


//
// validator1.echoStructTest (myStruct)
// returns struct
//
//
// This handler takes a single parameter named myStruct, a
// struct. Your handler must return the struct.
//


// our echo struct.  it's just a bunch of stooges
struct echostruct
{
	stooges substruct0;
	stooges substruct1;
	stooges substruct2;
	stooges substruct3;
	stooges substruct4;
	stooges substruct5;
	stooges substruct6;
	stooges substruct7;
	stooges substruct8;
	stooges substruct9;
};


// how we de-serialze the echo struct.
// notice how easy it is with operator overloading :)
const SOAPParameter&
operator>>(const SOAPParameter& param, echostruct& st)
{
	param.GetParameter("substruct0") >> st.substruct0;
	param.GetParameter("substruct1") >> st.substruct1;
	param.GetParameter("substruct2") >> st.substruct2;
	param.GetParameter("substruct3") >> st.substruct3;
	param.GetParameter("substruct4") >> st.substruct4;
	param.GetParameter("substruct5") >> st.substruct5;
	param.GetParameter("substruct6") >> st.substruct6;
	param.GetParameter("substruct7") >> st.substruct7;
	param.GetParameter("substruct8") >> st.substruct8;
	param.GetParameter("substruct9") >> st.substruct9;
	return param;
}

// and here we serialize it.  It's so easy.
SOAPParameter&
operator<<(SOAPParameter& param, const echostruct& st)
{
	param.AddParameter("substruct0") << st.substruct0;
	param.AddParameter("substruct1") << st.substruct1;
	param.AddParameter("substruct2") << st.substruct2;
	param.AddParameter("substruct3") << st.substruct3;
	param.AddParameter("substruct4") << st.substruct4;
	param.AddParameter("substruct5") << st.substruct5;
	param.AddParameter("substruct6") << st.substruct6;
	param.AddParameter("substruct7") << st.substruct7;
	param.AddParameter("substruct8") << st.substruct8;
	param.AddParameter("substruct9") << st.substruct9;
	param.SetIsStruct();
	return param;
}


//
// Now lets do it!
//
void
echoStructTest(const SOAPMethod& req, SOAPMethod& resp)
{
	echostruct st;
	const SOAPParameter& p = req.GetParameter("myStruct");

	p >> st;

	resp.SetName("echoStructTestResponse");
	resp.AddParameter("Result") << st;
}

//
// validator1.manyTypesTest (num, bool, state, doub, dat, bin)
// returns array
//
//
// This handler takes six parameters and returns an array containing
// all the parameters.
//


void
manyTypesTest(const SOAPMethod& req, SOAPMethod& resp)
{
	int			num;
	bool		b;
	SOAPString	state;
	double		doub;
	SOAPString	dat;
	SOAPString	bin;

	req.GetParameter("num") >> num;
	req.GetParameter("bool") >> b;
	req.GetParameter("state") >> state;
	req.GetParameter("doub") >> doub;
	req.GetParameter("dat") >> dat;
	req.GetParameter("bin") >> bin;

	resp.SetName("manyTypesTestResponse");

	SOAPParameter& result = resp.AddParameter("Result");
	result.AddParameter() << num;
	result.AddParameter() << b;

	// The validator is wrong.  It wants double math but wire type is float
	result.AddParameter() << state;
	(result.AddParameter() << doub).SetType("float");

	// Have to hork in a type since we don't explicitly support "timeInstant"
	// but we could!
	(result.AddParameter() << dat).SetType("timeInstant");

	result.AddParameter() << bin;
	result.SetIsArray();
}

//
// validator1.moderateSizeArrayCheck (myArray)
// returns string
//
//
// This handler takes a single parameter named myArray, which is
// an array containing between 100 and 200 elements. Each of the
// items is a string, your handler must return a string containing
// the concatenated text of the first and last elements.
//

void
moderateSizeArrayCheck(const SOAPMethod& req, SOAPMethod& resp)
{
	const SOAPParameter& p = req.GetParameter("myArray");
	size_t numels = p.GetArray().Size();

	SOAPString result;
	result += p.GetArray()[0].GetString();
	result += p.GetArray()[numels - 1].GetString();

	resp.SetName("moderateSizeArrayCheckResponse");
	resp.AddParameter("Result") << result;
}

//
// validator1.nestedStructTest (myStruct)
// returns number
//
//
// This handler takes a single parameter named myStruct, a struct,
// that models a daily calendar. At the top level, there is one struct
// for each year. Each year is broken down into months, and months
// into days. Most of the days are empty in the struct you receive, but
// the entry for April 1, 2000 contains a least three elements named moe,
// larry and curly, all <i4>s. Your handler must add the three numbers
// and return the result.
//
// Ken MacLeod: "This description isn't clear, I expected '2000.April.1'
// when in fact it's '2000.04.01'. Adding a note saying that month and day
// are two-digits with leading 0s, and January is 01 would help." Done.
//

void
nestedStructTest(const SOAPMethod& req, SOAPMethod& resp)
{
	const SOAPParameter& p = req.GetParameter("myStruct");

	stooges st;
	p.GetParameter("year2000").GetParameter("month04").GetParameter("day01") >> st;

	int sum = st.curly + st.larry + st.moe;
	resp.SetName("nestedStructTestResponse");
	resp.AddParameter("Result") << sum;
}

//
// validator1.simpleStructReturnTest (myNumber)
// returns struct
//
//
// This handler takes one parameter a number named myNumber, and returns
// a struct containing three elements, times10, times100 and times1000,
// the result of multiplying the number by 10, 100 and 1000.
void
simpleStructReturnTest(const SOAPMethod& req, SOAPMethod& resp)
{
	int myNumber;
	req.GetParameter("myNumber") >> myNumber;

	resp.SetName("simpleStructReturnTestResponse");

	SOAPParameter& result = resp.AddParameter("Result");
	result.AddParameter("times10") << myNumber * 10;
	result.AddParameter("times100") << myNumber * 100;
	result.AddParameter("times1000") << myNumber * 1000;
	result.SetIsStruct();
}


//
//  Return information about our toolkit
//
//
// Added an optional feature, not part of the validation test. We try to
// call a handler called whichToolkit. It takes no parameters and returns
// a struct containing the following elements, all strings: toolkitDocsUrl,
// toolkitName, toolkitVersion, toolkitOperatingSystem. If the call works,
// and all the elements are returned, we construct the string in the
// "software" column of the readout using this information. In previous
// versions we read the HTTP header for this information, often yielding
// the name of the server, and not the SOAP toolkit being used. 

void
whichToolkit(const SOAPMethod& req, SOAPMethod& resp)
{
	resp.SetName("whichToolkitResult");
	SOAPParameter& param = resp.AddParameter("Result");
	param.AddParameter("toolkitDocsUrl") << EASYSOAP_HOMEPAGE;
	param.AddParameter("toolkitName") << EASYSOAP_STRING;
	param.AddParameter("toolkitVersion") << EASYSOAP_VERSION_STRING;
	param.AddParameter("toolkitOperatingSystem") <<
	// TODO: This is a bit broken...
#ifdef _WIN32
		"Windows 2000";
#else
		"Linux";
#endif
	param.SetIsStruct();
}

void
ValidateHandler(const SOAPEnvelope& req, SOAPResponse& resp)
{
	const SOAPMethod& meth = req.GetBody().GetMethod();
	SOAPMethod& respm = resp.GetBody().GetMethod();

	if (meth.GetName() == "countTheEntities")
	{
		countTheEntities(meth, respm);
	}
	else if (meth.GetName() == "easyStructTest")
	{
		easyStructTest(meth, respm);
	}
	else if (meth.GetName() == "echoStructTest")
	{
		echoStructTest(meth, respm);
	}
	else if (meth.GetName() == "manyTypesTest")
	{
		manyTypesTest(meth, respm);
	}
	else if (meth.GetName() == "moderateSizeArrayCheck")
	{
		moderateSizeArrayCheck (meth, respm);
	}
	else if (meth.GetName() == "nestedStructTest")
	{
		nestedStructTest (meth, respm);
	}
	else if (meth.GetName() == "simpleStructReturnTest")
	{
		simpleStructReturnTest (meth, respm);
	}
	else if (meth.GetName() == "http://www.soapware.org/#whichToolkit")
	{
		whichToolkit (meth, respm);
	}
	else
		throw SOAPException("Unknown method: %s[%s]",
			(const char *)meth.GetName().GetName(),
			(const char *)meth.GetName().GetNamespace());
}

int
main(int argc, char* argv[], char *env[])
{
	int retval = 0;
	const char *faultactor = "SOAP-ENV::Sever";
	try
	{
		printf("SOAPServer: %s/%s\n", EASYSOAP_STRING, EASYSOAP_VERSION_STRING);

		SOAPParser p;
		SOAPResponse resp;
		SOAPEnvelope env;
		SOAPPacketWriter w;
		SOAPCGIHandler cgi;
		//cgi.SetLogFile("C:/validate.log");
		//cgi.SetInFile("C:/validate.log");

		faultactor = "SOAP-ENV::Client";
		p.Parse(env, cgi);
		faultactor = "SOAP-ENV::Server";

		const SOAPMethod& m = env.GetBody().GetMethod();

		//
		// TODO: Test SOAPAction, method namespace
		//
		ValidateHandler(env, resp);

		resp.WriteSOAPPacket(w);
		cgi.Write(resp.GetBody().GetMethod(), w.GetBytes(), w.GetLength());
	}
	catch(SOAPException& sex)
	{
		//
		// create SOAPFault
		//
		retval = 1;
		WriteFault(faultactor, sex.What());
	}
	catch (...)
	{
		//
		// create SOAPFault
		//
		faultactor = "SOAP-ENV::Server";
		WriteFault(faultactor, "Serious error occurred.");
		retval = 1;
	}


	return retval;
}

