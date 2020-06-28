#include "yane.hpp"

#include "i_test.hpp"
#include "nes_test.hpp"



int main(int argc, char **argv){
	
	i_test* tests[] = { new nes_test() };
	
	int n_tests = sizeof(i_test*) / sizeof(tests);
	for(int i = 0; i < n_tests; i++){
		
		i_test* t = tests[i];
		
		std::cout << t->get_name() << ": ";
		if(t->run_test()){
			std::cout << " OK!";
		}
		else {
			std::cout << " NOK!";
		}
		std::cout << std::endl;
	}
	
	std::cout << "Done " << n_tests << " tests." << std::endl;
	return 0;
}