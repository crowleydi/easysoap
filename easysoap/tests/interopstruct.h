//
//  The struct used for interop tests
//
struct SOAPInteropStruct
{
	SOAPInteropStruct()
	{
		varInt = 0;
		varFloat = 0.0;
	}

	SOAPInteropStruct(const char *str, int i, float f)
	{
		varString = str;
		varInt = i;
		varFloat = f;
	}

	bool operator==(const SOAPInteropStruct& other) const
	{
		return varInt == other.varInt &&
			varFloat == other.varFloat &&
			varString == other.varString;
	}

	bool operator!=(const SOAPInteropStruct& other) const
	{
		return varInt != other.varInt ||
			varFloat != other.varFloat ||
			varString != other.varString;
	}

	SOAPString	varString;
	int			varInt;
	float		varFloat;

	static const char *soap_namespace;
};
const char *SOAPInteropStruct::soap_namespace = "http://soapinterop.org/xsd";


//
//  Define how we serialize the struct
SOAPParameter&
operator<<(SOAPParameter& param, const SOAPInteropStruct& val)
{
	param.SetType("SOAPStruct", SOAPInteropStruct::soap_namespace);
	param.SetIsStruct();

	param.AddParameter("varString") << val.varString;
	param.AddParameter("varInt") << val.varInt;
	param.AddParameter("varFloat") << val.varFloat;

	return param;
}

//
// Define how we de-serialize the struct
const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPInteropStruct& val)
{
	// We should probably confirm the types are
	// correct...
	param.GetParameter("varString") >> val.varString;
	param.GetParameter("varInt") >> val.varInt;
	param.GetParameter("varFloat") >> val.varFloat;

	return param;
}


