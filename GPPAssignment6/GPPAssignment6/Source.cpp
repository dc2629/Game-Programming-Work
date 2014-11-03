#include"MatrixEntities.h"
#include"App.h"

using namespace std;

int main(int argc, char *argv[]){
	App hi;
	while (!hi.ProcessEvents()){
		hi.Render();
	}

	return 0;
}