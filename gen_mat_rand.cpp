#include <ctime>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char const *argv[])
{
	srand (time(NULL));
	ofstream out("matrix512.txt");
	out << "512\n{\n";
	for (int i = 0; i < 512; ++i)
	{	
		out <<"{";
		for (int j = 0; j < 512; ++j)
		{
			out << ((double)rand()/(double)RAND_MAX);
			out << (j+1 < 512 ? "," : "");
		}
		out << (i+1 < 512 ? "},\n" : "}\n");
	}
	out << "}";

	return 0;
}