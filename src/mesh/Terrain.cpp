
int *generate_terrain(int width, int height, int cols, int rows){

	cout << "Generating terrain" << endl;

	vertices.push_back(vec3(0,0,0));
	MeshVertex mv;

	const siv::PerlinNoise perlin(1231375486);
	int *result = (int *) calloc(cols * rows, sizeof(int));

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){

			int per = perlin.accumulatedOctaveNoise2D_0_1(i, j, 2);

			/*mv.uv.x = j;
			mv.uv.y = i;

			mv.position.x = 
			verts.push_back(mv);

			if (i < rows - 1 || j < cols - 1){

				//1st tri
				indices.push_back(cols * i + j);
				indices.push_back(cols * i + j + 1);
				indices.push_back(cols * (i+1) + j);

				//2nd tri
				indices.push_back(cols * i + j + 1);
				indices.push_back(cols * (i+1) + j + 1);
				indices.push_back(cols * i + j);
			}*/

			result[i * cols + j] = per;
		}
	}

	return result;
}

