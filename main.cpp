#include "include/yane.h"

#include "include/cartridge.h"
#include "include/ram.h"
#include "include/cpu_bus.h"

int main(int argc, char **argv){
	
	char* file_name = (char*)"/mnt/storage/Extra/CodeLiteWorkspaces/CodeLiteRootWorkspace/YANE/resources/nestest.nes";
	
	cartridge* c = new cartridge(file_name);
	ram* r = new ram(2 * 1024);
	
	cpu_bus* b = new cpu_bus(c, r);
	

	//byte_t r = c->read(0x8580);

	
	
	printf("Done!\n");
	return 0;
}
