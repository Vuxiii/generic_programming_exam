#ifndef GRAPH_TOPOLOGICAL_SORT_HPP
#define GRAPH_TOPOLOGICAL_SORT_HPP

#include "depth_first_search.hpp"

namespace graph {
namespace detail {

template<typename OIter>
struct TopoVisitor : DFSNullVisitor {
	TopoVisitor(OIter iter) : iter(iter) {}

    template<typename G, typename V>
	void finishVertex(const V &v, const G &g) { 
        *iter++ = v;
    }
private:
	OIter iter;
};


} // namespace detail

template<typename Graph, typename OutputIterator>
void topoSort( const Graph &g, OutputIterator oIter ) {
    dfs(g, detail::TopoVisitor(oIter));
}

} // namespace graph

#endif // GRAPH_TOPOLOGICAL_SORT_HPP
