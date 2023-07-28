#include <iostream>
#include <fstream>
#include <array>
#include "src/graph/adjacency_list.hpp"
#include "src/graph/io.hpp"
#include "src/graph/depth_first_search.hpp"
#include "src/graph/topological_sort.hpp"

struct VProp {
    const char *name;
    uint age;
};

struct EProp {
    const char *identifier;
    uint weight;
};

void bidirectional() {
    using G = graph::AdjacencyList<graph::tags::Bidirectional>;
    G g;
    std::cout << "Constructed an adjacency list with address [" << &g << "]\n";

    auto v0 = addVertex(g);
    auto v1 = addVertex(g);

    std::cout << "Added two new Vertices: [" << v0 << ", " << v1 << "]\n";


    auto eDesc = addEdge(v0, v1, g);

    std::cout << "Added new Edge with descriptor [src:" << eDesc.src << ", tar:" << eDesc.tar << ", id:" << eDesc.storedEdgeIdx << "]\n";
    
    auto v2 = addVertex(g);
    auto v3 = addVertex(g);
    auto v4 = addVertex(g);

    auto v0v2 = addEdge(v0, v2, g);
    auto v0v3 = addEdge(v0, v3, g);
    // auto v0v4 = addEdge(v0, v4, g).first;

    auto v2v4 = addEdge(v2, v4, g);

    std::cout << "\nEdges:\n\t[(" << eDesc.src << ", " << eDesc.tar << ", "  << eDesc.storedEdgeIdx << "), (" 
                                  << v0v2.src << ", " << v0v2.tar << ", "  << v0v2.storedEdgeIdx << "), (" 
                                  << v0v3.src << ", " << v0v3.tar << ", "  << v0v3.storedEdgeIdx << "), (" 
                                  << v2v4.src << ", " << v2v4.tar << ", "  << v2v4.storedEdgeIdx << ")]\n"; 

    std::cout << "Total edges: " << g.eList.size() << "\n";

{
    std::cout << "\nFetching incidence iterator for node [" << v0 << "]\n";
    auto itt = outEdges(v0, g);
    
    std::cout << "Iterating over neighbors of [" << v0 << "]\n";

    std::cout << "\t[";
    for ( auto it = itt.begin(); it != itt.end(); ++it ) {
        if ( it != itt.begin() )
            std::cout << ", ";
        std::cout << "(" << it->src << ", " << it->tar << ", " << it->storedEdgeIdx << ")";
    }
    std::cout << "]\n";
    
}
    std::cout << "Total edges: " << g.eList.size() << "\n";
{
    std::cout << "\nFetching edge iterator\n";
    G::EdgeRange itt = edges(g);

    std::cout << "(src, target, descriptor)\n";
    std::cout << "\t[";
    for ( auto it = itt.begin(); it != itt.end(); ++it ) {
        if ( it != itt.begin() )
            std::cout << ", ";
        std::cout << "(" << it->src << ", " << it->tar << ", " << it->storedEdgeIdx << ")";
    }
    std::cout << "]\n";
}
{
    std::cout << "\nFetching Vertex iterator\n";
    G::VertexRange vertexIT = vertices(g);

    std::cout << "(src, target, descriptor)\n";
    
    for ( auto vertex = vertexIT.begin(); vertex != vertexIT.end(); ++vertex ) {
        std::cout << "Vertex[" << *vertex << "]:\n  OutEdges:\t[";
        {
            auto out = outEdges(*vertex, g);
            for ( auto edge = out.begin(); edge != out.end(); ++edge ) {
                if ( edge != out.begin() ) 
                    std::cout << ", ";
                std::cout << "(" << edge->src << ", " << edge->tar << ", " << edge->storedEdgeIdx << ")";
            }
        }
        std::cout << "]\n  InEdges:\t[";
        {
            auto in = inEdges(*vertex, g);
            for ( auto edge = in.begin(); edge != in.end(); ++edge ) {
                if ( edge != in.begin() ) 
                    std::cout << ", ";
                std::cout << "(" << edge->src << ", " << edge->tar << ", " << edge->storedEdgeIdx << ")";
            }
        }
        std::cout << "]\n";
    }
    std::cout << "]\n";

}
}

void directed() {
    std::cout << "Hello world! Yaya" << std::endl;
    using G = graph::AdjacencyList<graph::tags::Directed>;
    G g;
    std::cout << "Constructed an adjacency list with address [" << &g << "]\n";

    auto v0 = addVertex(g);
    auto v1 = addVertex(g);

    std::cout << "Added two new Vertices: [" << v0 << ", " << v1 << "]\n";


    auto eDesc = addEdge(v0, v1, g);

    std::cout << "Added new Edge with descriptor [src:" << eDesc.src << ", tar:" << eDesc.tar << ", id:" << eDesc.storedEdgeIdx << "]\n";
    
    auto v2 = addVertex(g);
    auto v3 = addVertex(g);
    auto v4 = addVertex(g);

    auto v0v2 = addEdge(v0, v2, g);
    auto v0v3 = addEdge(v0, v3, g);
    // auto v0v4 = addEdge(v0, v4, g).first;

    auto v2v4 = addEdge(v2, v4, g);

    std::cout << "\nEdges:\n\t[(" << eDesc.src << ", " << eDesc.tar << ", "  << eDesc.storedEdgeIdx << "), (" 
                                  << v0v2.src << ", " << v0v2.tar << ", "  << v0v2.storedEdgeIdx << "), (" 
                                  << v0v3.src << ", " << v0v3.tar << ", "  << v0v3.storedEdgeIdx << "), (" 
                                  << v2v4.src << ", " << v2v4.tar << ", "  << v2v4.storedEdgeIdx << ")]\n"; 

    std::cout << "Total edges: " << g.eList.size() << "\n";

{
    std::cout << "\nFetching incidence iterator for node [" << v0 << "]\n";
    auto itt = outEdges(v0, g);
    
    std::cout << "Iterating over neighbors of [" << v0 << "]\n";

    std::cout << "\t[";
    for ( auto it = itt.begin(); it != itt.end(); ++it ) {
        if ( it != itt.begin() )
            std::cout << ", ";
        std::cout << "(" << it->src << ", " << it->tar << ", " << it->storedEdgeIdx << ")";
    }
    std::cout << "]\n";
    
}
    std::cout << "Total edges: " << g.eList.size() << "\n";
{
    std::cout << "\nFetching edge iterator\n";
    G::EdgeRange itt = edges(g);

    std::cout << "(src, target, descriptor)\n";
    std::cout << "\t[";
    for ( auto it = itt.begin(); it != itt.end(); ++it ) {
        if ( it != itt.begin() )
            std::cout << ", ";
        std::cout << "(" << it->src << ", " << it->tar << ", " << it->storedEdgeIdx << ")";
    }
    std::cout << "]\n";
}
}


void props() {
{
    using G = graph::AdjacencyList<graph::tags::Directed, VProp>;
    G g;
    std::cout << "Constructed an adjacency list with address [" << &g << "]\n";
    std::cout << "It holds properpy of type: VProp { const char *name; uint age; };\n";
    const char *s = "William";
    auto v = addVertex( VProp{ s, 22 }, g);

    std::cout << "Added vertex to graph [" << v << "]\n";

    std::cout << "Fetching stored prop in vertex[" << v << "]\n";

    VProp &prop = g[v];

    std::cout << "Fetched:\n\tVProp{ const char *name = \"" << prop.name << "\"; uint age = " << prop.age << "; };\n";

    std::cout << "Wops. Wrong age. Updating age to 23!\n";

    prop.age = 23;
    std::cout << "Re-Fetching stored prop in vertex[" << v << "]\n";
    prop = g[v];
    std::cout << "Re-Fetched:\n\tVProp{ const char *name = \"" << prop.name << "\"; uint age = " << prop.age << "; };\n";
}
{
    using G = graph::AdjacencyList<graph::tags::Directed, VProp, EProp>;
    G g;
    std::cout << "Constructed an adjacency list with address [" << &g << "]\n";
    std::cout << "It holds properpy of type: VProp { const char *name; uint age; };\n";
    std::cout << "It holds properpy of type: EProp { const char *identifier; uint weight; };\n";
    const char *home_name = "Home";
    auto home = addVertex( VProp{ home_name, 22 }, g);

    const char *work_name = "Work";
    auto work = addVertex( VProp{ work_name, 0 }, g );
    std::cout << "Added vertex to graph [" << home << "]\n";
    std::cout << "Added vertex to graph [" << work << "]\n";


    auto route = addEdge( home, work, EProp{ "Distance To Work", 5 }, g);


    VProp &prop_home = g[home];
    VProp &prop_work = g[work];

    std::cout << "\nvertex[" << home << "] = " << prop_home.name << "\n";
    std::cout << "\nvertex[" << work << "] = " << prop_work.name << "\n";
    
    EProp &prop_dist = g[route];

    std::cout << "Distance from [" << prop_home.name << "] to [" << prop_work.name << "] is " << prop_dist.weight << "\n";

}
}

void printer() {
    using G = graph::AdjacencyList<graph::tags::Directed>;
    G g;

    auto v0 = addVertex(g);
    auto v1 = addVertex(g);

    auto eDesc = addEdge(v0, v1, g);

    auto v2 = addVertex(g);
    auto v3 = addVertex(g);
    auto v4 = addVertex(g);

    auto v0v2 = addEdge(v0, v2, g);
    auto v0v3 = addEdge(v0, v3, g);
    auto v0v4 = addEdge(v0, v4, g);

    auto v2v4 = addEdge(v2, v4, g);

    std::ofstream outputFile("graph.dot");
    if (outputFile.is_open()) {
        graph::printDot(outputFile, g);

        outputFile.close(); 
        std::cout << "Graph saved to graph.dot" << std::endl;
    } else {
        std::cerr << "Failed to open the output file." << std::endl;
    }
}

void sort() {

    std::ifstream file("graph.dot");
    if (file.is_open()) {
        using G = graph::AdjacencyList<graph::tags::Directed>;

        G g = graph::loadDimacs<G>(file);
        file.close(); 

        std::cout << "Read the graph!\n";

        std::vector<typename graph::Traits<G>::VertexDescriptor > vs;
        vs.reserve( numVertices(g) );
        graph::topoSort( g, std::back_inserter(vs) );
        std::reverse( vs.begin(), vs.end() );

        std::cout << "Topological Sort:\t[";
        for ( auto v = vs.begin(); v != vs.end(); ++v ) {
            if ( v != vs.begin() ) 
                std::cout << ", ";
            std::cout << *v;
        } 
        std::cout << "]\n";

    } else {
        std::cerr << "Failed to open the file." << std::endl;
    }

}

void sort2() {
    using G = graph::AdjacencyList<graph::tags::Directed>;
    G g(6);
    addEdge(5, 2, g);
    addEdge(5, 0, g);
    addEdge(4, 0, g);
    addEdge(4, 1, g);
    addEdge(2, 3, g);
    addEdge(3, 1, g);

    std::vector<typename graph::Traits<G>::VertexDescriptor > vs;
    vs.reserve( numVertices(g) );
    graph::topoSort( g, std::back_inserter(vs) );
    std::reverse( vs.begin(), vs.end() );
    
    std::cout << "Topological Sort:\t[";
    for ( auto v = vs.begin(); v != vs.end(); ++v ) {
        if ( v != vs.begin() ) 
            std::cout << ", ";
        std::cout << *v;
    } 
    std::cout << "]\n";
}

void sort3() {
    using G = graph::AdjacencyList<graph::tags::Directed>;
    typedef graph::Traits<G>::VertexDescriptor Vertex;

    G g(6);
    addEdge(0, 1, g);
    addEdge(2, 4, g);
    addEdge(2, 5, g);
    addEdge(0, 3, g);
    addEdge(1, 4, g);
    addEdge(4, 3, g);

    std::vector<Vertex> vs;
    graph::topoSort( g, std::back_inserter(vs) );
    std::reverse( vs.begin(), vs.end() );
    
    std::cout << "Topological Sort:\t[";
    for ( auto v = vs.begin(); v != vs.end(); ++v ) {
        if ( v != vs.begin() ) 
            std::cout << ", ";
        std::cout << *v;
    } 
    std::cout << "]\n";
}

int main() {
    std::cout << "\n----------------DIRECTED-----------------\n\n";

    directed();

    std::cout << "\n--------------BIDIRECTIONAL--------------\n\n";

    bidirectional();

    std::cout << "\n------------------PROPS------------------\n\n";

    props();

    // std::cout << "\n-----------------PRINTER-----------------\n\n";
    
    // printer();

    std::cout << "\n------------------SORT-------------------\n\n";

    sort();

    std::cout << "\n------------------SORT2------------------\n\n";

    sort2();

    std::cout << "\n------------------SORT3------------------\n\n";

    sort3();

    return 0;
}