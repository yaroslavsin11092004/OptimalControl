#include "IncludeFiles.h"
using namespace std;
int main()
{
	fstream file("/home/yaroslavsinyakov/source/OptimalControl/config_system.json");
	if (file.is_open())
	{
		string data = {istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
		file.close();
		json js = json::parse(data);
		cout << js["CoreServer"]["host"].get<string>() << endl;
	}
	file.close();
	return 0;
}
