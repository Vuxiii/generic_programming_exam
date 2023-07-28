#ifndef GRAPH_ADJACENCY_LIST_HPP
#define GRAPH_ADJACENCY_LIST_HPP

#include "tags.hpp"
#include "traits.hpp"
#include "properties.hpp"

#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

#include <cassert>
#include <list>
#include <vector>

#include <concepts>

namespace graph {

template<typename DirectedCategoryT,
         typename VertexPropT = NoProp,
         typename EdgePropT = NoProp>
struct AdjacencyList {
private:
	struct OutEdge {
		OutEdge(std::size_t edgeID) : edgeID(edgeID) {}

        const std::size_t edgeID;
	};

	struct InEdge {
		InEdge(std::size_t edgeID) : edgeID(edgeID) {}

        const std::size_t edgeID;
	};

	using OutEdgeList = std::vector<OutEdge>;
	using InEdgeList = std::vector<InEdge>;

public:
    using VertexProp = VertexPropT;
private:

	struct StoredVertexDirected {
		OutEdgeList eOut;
	};

    struct StoredVertexBidirectional {
        OutEdgeList eOut;
        InEdgeList eIn;
    };

    struct StoredVertexDirectedWithProp {
        StoredVertexDirectedWithProp() = default;
        StoredVertexDirectedWithProp(VertexProp prop) : prop(prop) {}

		OutEdgeList eOut;
        VertexProp prop;
	};
    
    struct StoredVertexBidirectionalWithProp {
        StoredVertexBidirectionalWithProp() = default;
        StoredVertexBidirectionalWithProp(VertexProp prop) : prop(prop) {}

        OutEdgeList eOut;
        InEdgeList eIn;
        VertexProp prop;
    };

public:
	using DirectedCategory = DirectedCategoryT;
private:
    // Does the user want to store props?
    using VertexTypeDirected = std::conditional<
                std::same_as<VertexProp, NoProp>,
                    StoredVertexDirected,
                    StoredVertexDirectedWithProp>::type;
    
    // Does the user want to store props?
    using VertexTypeBidirectional = std::conditional<
                std::same_as<VertexProp, NoProp>,
                    StoredVertexBidirectional,
                    StoredVertexBidirectionalWithProp>::type;

    // Is it directed or bidirectional?

    using StoredVertex = std::conditional<
                std::same_as<DirectedCategory, tags::Directed>, 
                    VertexTypeDirected, // If Directed tag is set
                    VertexTypeBidirectional>::type; // If Directed tag is NOT set
public:
	using EdgeProp = EdgePropT;
    using VertexDescriptor = std::size_t;
private:	
	
    struct Edge {
		VertexDescriptor src, tar; // Indices into vList. 
	};

    struct EdgeWithProp {
		
        EdgeWithProp() = default;
        EdgeWithProp( VertexDescriptor src, VertexDescriptor tar ) : src(src), tar(tar) {}
        EdgeWithProp( VertexDescriptor src, VertexDescriptor tar, EdgeProp prop ) : src(src), tar(tar), prop(prop) {}

        VertexDescriptor src, tar; // Indices into vList. 
        EdgeProp prop;
	};



    // Does the user want to store props with the edges?
    using StoredEdge = std::conditional<
                std::same_as<EdgeProp, NoProp>,
                    Edge,
                    EdgeWithProp>::type;

	using VList = std::vector<StoredVertex>;
	using EList = std::vector<StoredEdge>;
public: // Graph

	struct EdgeDescriptor {
		EdgeDescriptor() = default;
		EdgeDescriptor(std::size_t src, std::size_t tar,
		               std::size_t storedEdgeIdx)
			: src(src), tar(tar), storedEdgeIdx(storedEdgeIdx) {}
	public:
		std::size_t src, tar;
		std::size_t storedEdgeIdx;
	public:
		friend bool operator==(const EdgeDescriptor &a,
		                       const EdgeDescriptor &b) {
			return a.storedEdgeIdx == b.storedEdgeIdx;
		}
	};
public: // PropertyGraph
	// using VertexProp = VertexPropT;
	// using EdgeProp = EdgePropT;
public: // VertexListGraph
	struct VertexRange {
		// the iterator is simply a counter that returns its value when
		// dereferenced
		using iterator = boost::counting_iterator<VertexDescriptor>;
	public:
		VertexRange(std::size_t n) : n(n) {}
		iterator begin() const { return iterator(0); }
		iterator end()   const { return iterator(n); }
	private:
		std::size_t n;
	};
public: // EdgeListGraph
	struct EdgeRange {
		// We want to adapt the edge list,
		// so it dereferences to EdgeDescriptor instead of StoredEdge
		using EListIterator = typename EList::const_iterator;

		struct iterator : boost::iterator_adaptor<
				iterator, // because we use CRTP
				EListIterator, // the iterator we adapt
				// we want to convert the StoredEdge into an EdgeDescriptor:
				EdgeDescriptor,
				// we can use RA as the underlying iterator supports it:
				std::random_access_iterator_tag,
				// when we dereference we return by value, not by reference
				EdgeDescriptor
			> {
			using Base = boost::iterator_adaptor<
				iterator, EListIterator, EdgeDescriptor,
				std::random_access_iterator_tag, EdgeDescriptor>;
		public:
			iterator() = default;
			iterator(EListIterator i, EListIterator first) : Base(i), first(first) {}
		private:
			// let the Boost machinery use our methods:
			friend class boost::iterator_core_access;

			EdgeDescriptor dereference() const {
				// get our current position stored in the
				// boost::iterator_adaptor base class
				const EListIterator &i = this->base_reference();
				return EdgeDescriptor{i->src, i->tar,
					static_cast<std::size_t>(i - first)};
			}
		private:
			EListIterator first;
		};
	public:
		EdgeRange(const AdjacencyList &g) : g(&g) {}

		iterator begin() const {
			return iterator(g->eList.begin(), g->eList.begin());
		}

		iterator end() const {
			return iterator(g->eList.end(), g->eList.begin());
		}
	private:
		const AdjacencyList *g;
	};
public: // IncidenceGraph
    struct OutEdgeRange {
        // We want to adapt the edge list,
		// so it dereferences to EdgeDescriptor instead of StoredEdge
		using EListIterator = typename OutEdgeList::const_iterator;

		struct iterator : boost::iterator_adaptor<
				iterator, // because we use CRTP
				EListIterator, // the iterator we adapt
				// we want to convert the StoredEdge into an EdgeDescriptor:
				EdgeDescriptor,
				// we can use RA as the underlying iterator supports it:
				std::random_access_iterator_tag,
				// when we dereference we return by value, not by reference
				EdgeDescriptor
			> {
			using Base = boost::iterator_adaptor<
				iterator, EListIterator, EdgeDescriptor,
				std::random_access_iterator_tag, EdgeDescriptor>;
		public:
			iterator() = default;
			iterator(EListIterator i, EListIterator first, const AdjacencyList *g) : Base(i), first(first), g(g) {}
		private:
			// let the Boost machinery use our methods:
			friend class boost::iterator_core_access;

			EdgeDescriptor dereference() const {
				// get our current position stored in the
				// boost::iterator_adaptor base class
				const EListIterator &i = this->base_reference();
                
                StoredEdge e = g->eList[i->edgeID];


				return EdgeDescriptor{e.src, e.tar, i->edgeID};
			}
		private:
			EListIterator first;
            const AdjacencyList *g;
		};
	public:
		OutEdgeRange(const VertexDescriptor u,  const AdjacencyList &g) : g(&g), u(u) {}

		iterator begin() const {
            auto i = getIndex(u, *g);

            return iterator(g->vList[i].eOut.begin(), g->vList[i].eOut.begin(), g);
			// return iterator(g->eList[e->storedEdgeIdx].begin(), g->eList.begin());
		}

		iterator end() const {
            auto i = getIndex(u, *g);
            return iterator(g->vList[i].eOut.end(), g->vList[i].eOut.begin(), g);
			// return iterator(g->eList.end(), g->eList.begin());
		}
	private:
		const AdjacencyList *g;
		const VertexDescriptor u;
    };
public: // BidirectionalGraph
	// TODO
    struct InEdgeRange {
        // We want to adapt the edge list,
		// so it dereferences to EdgeDescriptor instead of StoredEdge
		using EListIterator = typename InEdgeList::const_iterator;

		struct iterator : boost::iterator_adaptor<
				iterator, // because we use CRTP
				EListIterator, // the iterator we adapt
				// we want to convert the StoredEdge into an EdgeDescriptor:
				EdgeDescriptor,
				// we can use RA as the underlying iterator supports it:
				std::random_access_iterator_tag,
				// when we dereference we return by value, not by reference
				EdgeDescriptor
			> {
			using Base = boost::iterator_adaptor<
				iterator, EListIterator, EdgeDescriptor,
				std::random_access_iterator_tag, EdgeDescriptor>;
		public:
			iterator() = default;
			iterator(EListIterator i, EListIterator first, const AdjacencyList *g) : Base(i), first(first), g(g) {}
		private:
			// let the Boost machinery use our methods:
			friend class boost::iterator_core_access;

			EdgeDescriptor dereference() const {
				// get our current position stored in the
				// boost::iterator_adaptor base class
				const EListIterator &i = this->base_reference();
                
                StoredEdge e = g->eList[i->edgeID];


				return EdgeDescriptor{e.src, e.tar, i->edgeID};
			}
		private:
			EListIterator first;
            const AdjacencyList *g;
		};
	public:
		InEdgeRange(const VertexDescriptor u,  const AdjacencyList &g) : g(&g), u(u) {}

		iterator begin() const {
            auto i = getIndex(u, *g);
            return iterator(g->vList[i].eIn.begin(), g->vList[i].eIn.begin(), g);
			// return iterator(g->eList[e->storedEdgeIdx].begin(), g->eList.begin());
		}

		iterator end() const {
            auto i = getIndex(u, *g);
            return iterator(g->vList[i].eIn.end(), g->vList[i].eIn.begin(), g);
			// return iterator(g->eList.end(), g->eList.begin());
		}
	private:
		const AdjacencyList *g;
		const VertexDescriptor u;
    };
public:
	AdjacencyList() = default;
	AdjacencyList(std::size_t n) : vList(n) {}
private: // TODO! Change to private
	VList vList;
	EList eList;
public: // Graph
	friend VertexDescriptor source(EdgeDescriptor e, const AdjacencyList &g) {
		return e.src;
	}

	friend VertexDescriptor target(EdgeDescriptor e, const AdjacencyList &g) {
		return e.tar;
	}
public: // VertexListGraph
	friend std::size_t numVertices(const AdjacencyList &g) {
		return g.vList.size();
	}

	friend VertexRange vertices(const AdjacencyList &g) {
		return VertexRange(numVertices(g));
	}
public: // EdgeListGraph
	friend std::size_t numEdges(const AdjacencyList &g) {
		return g.eList.size();
	}

	friend EdgeRange edges(const AdjacencyList &g) {
		return EdgeRange(g);
	}
public: // Other
	friend std::size_t getIndex(VertexDescriptor v, const AdjacencyList &g) {
		return v;
	}
public: // IncidenceGraph
	// TODO
    friend OutEdgeRange outEdges( VertexDescriptor u, const AdjacencyList &g ) {
        OutEdgeRange it = OutEdgeRange(u, g);
        return it;
    } 

    friend size_t outDegree( VertexDescriptor u, const AdjacencyList &g ) {
        auto itt = outEdges(u, g);
        size_t size = 0;

        for ( auto it = itt.begin(); it != itt.end(); ++it ) { ++size; }

        return size;
    }

public: // BidirectionalGraph
	// TODO
    friend InEdgeRange inEdges( VertexDescriptor u, const AdjacencyList &g ) requires !std::same_as<DirectedCategory, tags::Directed> {
        InEdgeRange it = InEdgeRange(u, g);
        return it;
    } 
public: // MutableGraph
	// TODO
    friend VertexDescriptor addVertex( AdjacencyList &g ) requires std::same_as<VertexProp, NoProp> {
        VertexDescriptor d = numVertices(g);
        StoredVertex v;
        g.vList.push_back(v);
        return d;
    }

    
    friend VertexDescriptor addVertex( AdjacencyList &g ) requires (!std::same_as<VertexProp, NoProp> && std::is_default_constructible<VertexProp>::value) {
        VertexDescriptor d = numVertices(g);
        StoredVertex v;
        g.vList.push_back(v);
        return d;
    }

    friend VertexDescriptor addVertex( VertexProp prop, AdjacencyList &g ) requires (!std::same_as<VertexProp, NoProp>) {
        VertexDescriptor d = numVertices(g);
        StoredVertex v(prop);
        g.vList.push_back(v);
        return d;
    }

    // No prop directed
    friend EdgeDescriptor addEdge( VertexDescriptor u, VertexDescriptor v, AdjacencyList &g) requires (std::same_as<DirectedCategory, tags::Directed> && std::same_as<EdgeProp, NoProp>) {
        size_t index = numEdges(g);
        StoredEdge e = { .src = u, .tar = v };
        g.eList.push_back(e);
        
        OutEdge out(index);
        g.vList[getIndex(u, g)].eOut.push_back(out);

        return EdgeDescriptor{u, v, index};
    }

    // No prop bidirectional
    friend EdgeDescriptor addEdge( VertexDescriptor u, VertexDescriptor v, AdjacencyList &g) requires (std::same_as<DirectedCategory, tags::Bidirectional> && std::same_as<EdgeProp, NoProp>) {
        size_t index = numEdges(g);
        StoredEdge e = { .src = u, .tar = v };
        g.eList.push_back(e);
        
        OutEdge out(index);
        InEdge in(index);
        
        g.vList[getIndex(u, g)].eOut.push_back(out);
        g.vList[getIndex(v, g)].eIn.push_back(in);
     
        return EdgeDescriptor{u, v, index};
    }


    // With prop! directed Must be default constructible
    friend EdgeDescriptor addEdge( VertexDescriptor u, VertexDescriptor v, AdjacencyList &g) requires (std::same_as<DirectedCategory, tags::Directed> && !std::same_as<EdgeProp, NoProp> && std::is_default_constructible<EdgeProp>::value) {
        size_t index = numEdges(g);
        StoredEdge e = { .src = u, .tar = v };
        g.eList.push_back(e);
        
        OutEdge out(index);
        g.vList[getIndex(u, g)].eOut.push_back(out);

        return EdgeDescriptor{u, v, index};
    }

    // With prop! bidirectional Must be default constructible
    friend EdgeDescriptor addEdge( VertexDescriptor u, VertexDescriptor v, AdjacencyList &g) requires (std::same_as<DirectedCategory, tags::Bidirectional> && !std::same_as<EdgeProp, NoProp> && std::is_default_constructible<EdgeProp>::value) {
        size_t index = numEdges(g);
        StoredEdge e = { .src = u, .tar = v };
        g.eList.push_back(e);
        
        OutEdge out(index);
        InEdge in(index);
        
        g.vList[getIndex(u, g)].eOut.push_back(out);
        g.vList[getIndex(v, g)].eIn.push_back(in);
     
        return EdgeDescriptor{u, v, index};
    }

    

    // With prop! directed Must be default constructible
    friend EdgeDescriptor addEdge( VertexDescriptor u, VertexDescriptor v, EdgeProp prop, AdjacencyList &g) requires (std::same_as<DirectedCategory, tags::Directed> && !std::same_as<EdgeProp, NoProp>) {
        size_t index = numEdges(g);
        StoredEdge e(u, v, prop);
        g.eList.push_back(e);
        
        OutEdge out(index);
        g.vList[getIndex(u, g)].eOut.push_back(out);

        return EdgeDescriptor{u, v, index};
    }

    // With prop! bidirectional Must be default constructible
    friend EdgeDescriptor addEdge( VertexDescriptor u, VertexDescriptor v, EdgeProp prop, AdjacencyList &g) requires (std::same_as<DirectedCategory, tags::Bidirectional> && !std::same_as<EdgeProp, NoProp>) {
        size_t index = numEdges(g);
        StoredEdge e(u, v, prop);
        g.eList.push_back(e);
        
        OutEdge out(index);
        InEdge in(index);
        
        g.vList[getIndex(u, g)].eOut.push_back(out);
        g.vList[getIndex(v, g)].eIn.push_back(in);
     
        return EdgeDescriptor{u, v, index};
    }

public: // MutablePropertyGraph
	// TODO
public: // PropertyGraph
	// TODO


    const VertexProp &operator[]( VertexDescriptor v ) const {
        return vList[getIndex(v, *this)].prop;
    }

    VertexProp &operator[]( VertexDescriptor v ) {
        return vList[getIndex(v, *this)].prop;
    }

    const EdgeProp &operator[]( EdgeDescriptor v ) const {
        return eList[v.storedEdgeIdx].prop;
    }

    EdgeProp &operator[]( EdgeDescriptor v ) {
        return eList[v.storedEdgeIdx].prop;
    }

};

} // namespace graph

#endif // GRAPH_ADJACENCY_LIST_HPP
