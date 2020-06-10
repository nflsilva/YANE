#include "include/yane.h"
#include "include/cartridge.h"

int main(int argc, char **argv)
{
	
	char* file_name = (char*)"/mnt/storage/Extra/CodeLiteWorkspaces/CodeLiteRootWorkspace/YANE/nestest.nes";
	cartridge* c = new cartridge(file_name);

	
	printf("Done!\n");
	return 0;
}
