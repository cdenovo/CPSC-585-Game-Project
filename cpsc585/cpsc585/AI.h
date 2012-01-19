#include <stdlib.h>
#include <math.h>

#include "Renderer.h"
#include "Input.h"


class AI
{
public:
	AI(void);
	~AI(void);
	void shutdown();
	void initialize(Renderer* renderer, Input* input);
	void simulate(float milliseconds);

private:
	Renderer* renderer;
	Input* input;
	int count;
	int fps;
};

