#include <CollageApplication.h>
#include <iostream>
#include <stdio.h>

int main(int argc, char *argv[])
{
	CollageApplication collage;
	try
	{
		collage.start();
	}
	catch(std::exception& e)
    {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }

}
