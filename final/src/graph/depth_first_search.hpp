#ifndef GRAPH_DEPTH_FIRST_SEARCH_HPP
#define GRAPH_DEPTH_FIRST_SEARCH_HPP


#include "adjacency_list.hpp"
#include "traits.hpp"

#include <vector>

namespace graph {

struct DFSNullVisitor {
	template<typename G, typename V>
	void initVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void startVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void discoverVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void finishVertex(const V&, const G&) { }	

	template<typename G, typename E>
	void examineEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void treeEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void backEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void forwardOrCrossEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void finishEdge(const E&, const G&) { }	
};



namespace detail {

enum struct DFSColor {
	White, Grey, Black
};

template<typename Graph, typename Visitor>
void dfsVisit(const Graph &g, Visitor &visitor, typename Traits<Graph>::VertexDescriptor u, std::vector<DFSColor> &color) {
	
    visitor.discoverVertex( u, g );
    color[getIndex(u, g)] = DFSColor::Grey;
    auto it = outEdges( u, g );
    for ( auto e = it.begin(); e != it.end(); ++e ) {
        auto v = e->tar;
        visitor.examineEdge( *e, g );
        if ( color[getIndex(v, g)] == DFSColor::White ) {
            visitor.treeEdge( *e, g );
            dfsVisit( g, visitor, v, color );
        } else if ( color[getIndex(v, g)] == DFSColor::Grey ) {
            visitor.backEdge( *e, g );
        } else if ( color[getIndex(v, g)] == DFSColor::Black ) {
            visitor.forwardOrCrossEdge( *e, g );
        }
        visitor.finishEdge( *e, g );
    } 
    color[getIndex(u, g)] = DFSColor::Black;
    visitor.finishVertex( u, g );

}

} // namespace detail

template<typename Graph, typename Visitor>
void dfs(const Graph &g, Visitor visitor) {
    std::vector<detail::DFSColor> vertices_color(numVertices(g));

    auto it = vertices(g);

	for ( auto vertex = it.begin(); vertex != it.end(); ++vertex ) {
        vertices_color[getIndex(*vertex, g)] = detail::DFSColor::White;
        visitor.initVertex(*vertex, g);
    }
    for ( auto vertex = it.begin(); vertex != it.end(); ++vertex ) {
        if ( vertices_color[getIndex(*vertex, g)] == detail::DFSColor::White ) {
            visitor.startVertex(*vertex, g);
            dfsVisit( g, visitor, *vertex, vertices_color );
        }
    }
}



} // namespace graph

#endif // GRAPH_DEPTH_FIRST_SEARCH_HPP
