#include "include/yane.h"

#include "include/cartridge.h"
#include "include/ram.h"
#include "include/cpu_bus.h"
#include "include/r6502.h"

int main(int argc, char **argv){
	
	char* file_name = (char*)"/mnt/storage/Extra/CodeLiteWorkspaces/CodeLiteRootWorkspace/YANE/resources/nestest.nes";
	
	cartridge* c = new cartridge(file_name);
	ram* r = new ram(2 * 1024);
	cpu_bus* b = new cpu_bus(c, r);
	
	r6502* cpu = new r6502(b);

	ui16_t addr = cpu->relative_addressing();

	
	
	printf("Done!\n");
	return 0;
}
