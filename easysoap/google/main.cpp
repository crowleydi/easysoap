
#include <iostream>
#include "google.h"

//
// Quick little demo program to try out
// our GoogleSearch class.
//

void
usage(const char * prog)
{
	std::cout
		<< "Usage: " << prog << " <key> <query>" << std::endl
		<< "Where:" << std::endl
		<< "\t<key> is your Google account key." << std::endl
		<< "\t<query> is your search string." << std::endl << std::endl
		<< "Register for your Google account key at "
		<< "http://www.google.com/apis/" << std::endl << std::endl
		;
}

int
main(int argc, const char *argv[])
{
	if (argc < 3)
	{
		usage(argv[0]);
		return 1;
	}

	try
	{
		GoogleSearch google;

		//
		// set the key up one time so
		// we don't have to pass it in
		// with each query.
		google.setKey(argv[1]);

		//
		// Do our search.
		GoogleSearch::Result result;
		google.search(argv[2], 0, 10,
				false, "", false, "",
				"latin1", "latin1", result);

		//
		// Print out a summary of the results
		std::cout << "Found an estimated " << result.estimatedTotalResultsCount
			<< " results in " << result.searchTime << " second(s)." << std::endl
			<< "Search comments: " << result.searchComments << std::endl
			<< "Search tips: " << result.searchTips << std::endl
			;

		//
		// Print info on the hits.
		for (GoogleSearch::ResultElementArray::iterator el = result.resultElements.begin();
				el != result.resultElements.end(); ++el)
		{
			std::cout << "Title: " << el->title << std::endl
				<< "Snippet: " << el->snippet << std::endl
				<< "URL: " << el->URL << std::endl
				<< std::endl
				;
		}
	}
	catch (SOAPException& sex)
	{
		std::cerr << "Caught SOAP exception: "
			<< sex.What().Str() << std::endl;
		return 1;
	}

	return 0;
}
