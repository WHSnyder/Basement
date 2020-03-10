#include <iostream> // std::cout
#include <utility> // std::pair
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>


enum files_e { dax_h, yow_h, boz_h, zow_h, foo_cpp, 
             foo_o, bar_cpp, bar_o, libfoobar_a,
             zig_cpp, zig_o, zag_cpp, zag_o, 
             libzigzag_a, killerapp, N };

const char* name[] = { "dax.h", "yow.h", "boz.h", "zow.h", "foo.cpp",
                     "foo.o", "bar.cpp", "bar.o", "libfoobar.a",
                     "zig.cpp", "zig.o", "zag.cpp", "zag.o",
                     "libzigzag.a", "killerapp" };

typedef std::pair<int, int> Edge;

	Edge used_by[] = {
	Edge(dax_h, foo_cpp), Edge(dax_h, bar_cpp), Edge(dax_h, yow_h),
	Edge(yow_h, bar_cpp), Edge(yow_h, zag_cpp),
	Edge(boz_h, bar_cpp), Edge(boz_h, zig_cpp), Edge(boz_h, zag_cpp),
	Edge(zow_h, foo_cpp), 
	Edge(foo_cpp, foo_o),
	Edge(foo_o, libfoobar_a),
	Edge(bar_cpp, bar_o),
	Edge(bar_o, libfoobar_a),
	Edge(libfoobar_a, libzigzag_a),
	Edge(zig_cpp, zig_o),
	Edge(zig_o, libzigzag_a),
	Edge(zag_cpp, zag_o),
	Edge(zag_o, libzigzag_a),
	Edge(libzigzag_a, killerapp)
};

/*
Comp command:
clang++ -o tst -I/usr/local/Cellar/boost/1.72.0_2/include boost_graph_test.cpp /usr/local/Cellar/boost/1.72.0_2/lib/libboost_graph.a
*/

using namespace boost;

typedef adjacency_list<vecS, vecS, bidirectionalS, property<vertex_color_t, default_color_type> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;


int main(int argc, char **argv){
	Graph g(used_by, used_by + sizeof(used_by) / sizeof(Edge), N);
	std::cout << "Donezo" << std::endl;
	return 1;
}