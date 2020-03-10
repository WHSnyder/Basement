#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include "rendering/Texture.h"
#include "rendering/Shader.h"
#include "rendering/RenderTarget.h"

using namespace std;

class GameContext {

private:
	vector<Shader *> shaders;

public:

	Pipeline *pipeline;


}
