#include <iostream>
#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>


enum files_e { s1, s2, s3, s4, fbo1, tex1, outbuf, pool, N};

const char* names[] = {"s1", "s2", "s3", "s4", "fbo1", "tex1", "outbuf", "pool"};

typedef std::pair<int, int> Edge;

namespace yo {

struct yolo {
	void printYolo();
};

}

using namespace yo;

static void printYolo(){
	std::cout << "yolo" << std::endl;
}


Edge used_by[] = 
{
Edge(pool, s1), Edge(pool, s2), 
Edge(s2, fbo1), Edge(pool, s4), 
Edge(tex1, s4), Edge(s4, outbuf),
Edge(s1, fbo1), Edge(fbo1, s3),
Edge(pool, s3), Edge(s3, outbuf) 
};

/*
Comp command:
clang++ -o tst -I/usr/local/Cellar/boost/1.72.0_2/include boost_graph_test.cpp /usr/local/Cellar/boost/1.72.0_2/lib/libboost_graph.a
*/

using namespace boost;

typedef adjacency_list<vecS, vecS, bidirectionalS, property<vertex_color_t, default_color_type> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;



//Calculate all distances from final buffer in graph
//For each dep in DefaultFBO, prepare




//prepare()

























int main(int argc, char **argv){

	yolo t = yolo();
	t.printYolo();
	
	Graph g(used_by, used_by + sizeof(used_by) / sizeof(Edge), N);

	typedef std::list<Vertex> PL;
	PL pipeline;
	boost::topological_sort(g, std::front_inserter(pipeline));

	std::cout << "Pipeline ordering: ";

	for (PL::iterator i = pipeline.begin(); i != pipeline.end(); ++i)
		std::cout << names[*i] << " ";
	
	std::cout << std::endl;

	return 1;
}

