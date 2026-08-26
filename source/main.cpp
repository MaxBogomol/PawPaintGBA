#include "paint.h"

Paint paint;

int main(void) {
    //paint.setup();
	paint.setupVideo();
	paint.setupLayers();
	//paint.setupTools();

	while (1) {
		//paint.updateInputs();
		//paint.updateTools();
		paint.updateVideo();
	}

    return 0;
}
