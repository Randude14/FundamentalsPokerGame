#include <iostream>

#include "json.hpp"

int main(int argc,char *argv[])
{

	nlohmann::json js;
   	js["name"] = "Randall";	
	js["id"] = "12345";
	js["chrono"] = "Jan 28 2020 11:29PM";
	return 0;
}
