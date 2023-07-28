// #include "src/Tree.hpp"
// #include <cassert>

// int main() {


//     DM852::Tree<int, std::string> t;

//     t.insert( 24, "a" );
//     t.insert( 18, "b" );
//     t.insert( 26, "c" );
//     t.insert( 5, "d" );
//     t.insert( 20, "e" );
//     std::pair<DM852::Tree::Node *, bool> l = t.insert( 27, "f" );
//     t.insert( 2, "g" );
//     t.insert( 7, "h" );
//     t.insert( 23, "i" );
//     t.insert( 21, "j" );

//     // std::cout << "[Key = " << t.begin()->key << ", Value = " << t.begin()->value << "]\n";

//     for ( const DM852::Tree::Node *n = t.begin(); n != t.end(); n = n->next() ) 
//         std::cout << "[Key = " << n->key << ", Value = " << n->value << "]\n";

//     std::cout << "\n";

//     for ( const DM852::Tree::Node *n = l.first; n != t.begin(); n = n->prev() ) 
//         std::cout << "[Key = " << n->key << ", Value = " << n->value << "]\n";


//     DM852::Tree<int, std::string> t1, t2;
//     t1.insert(1, "1");
//     t2.insert(1, "1");

//     std::cout << (t1 == t2 ? "True" : "False") << std::endl;

//     std::string s1 = "abe";
//     std::string s2(s1);
//     std::string s3 = s1;

//     std::cout << &s1 << "\n";
//     std::cout << &s2 << "\n";
//     std::cout << &s3 << "\n";


// {
//     DM852::Tree<int, std::string> t1;
//     t1.insert(1, "1");
//     DM852::Tree<int, std::string> t2(t1);
//     // DM852::Tree t2;
//     // t2.insert(1, "1");
//     std::cout << &t1 << "\n";
//     std::cout << &t2 << "\n";
//     std::cout << &t1.front() << "\n";
//     std::cout << &t2.front() << "\n\n";

//     DM852::Tree::Node *n;
//     n = t1.begin();
//     // n->operator=( t1.begin() );
//     n = t1.begin();

//     assert( &t1.front() != &t2.front() );
// }
//     return 0;
// }


 #include<cassert>
 #include <functional>
 #include <iostream>
 
 #include <iostream>
 
 #define DO(args...) 	std::cout << "DO: " << #args << std::endl; 	args
 
 // Make our own next() and distance()functions that doens't use iter traits.
 template<typename InputIt>
 constexpr int jlaDistance(InputIt first, InputIt last) {
 	int res = 0;
 	for(; first != last; ++first) ++res;
 	return res;
 }
 
 template<typename InputIt>
 constexpr InputIt jlaNext(InputIt it, int n = 1) {
 	while(n > 0) {
 		--n;
 		++it;
 	}
 	return it;
 }
 
 
 #include "src/Tree.hpp"
 #include <cassert>
//  #include <concepts>
 #include <string>
 template<typename T, typename U> using Tree = DM852::Tree<T, U>;
 struct SomeElement {
 	SomeElement() = default;
 	SomeElement(const char *data) : data(data) {}
 	friend bool operator==(const SomeElement &a, const char *b) { return a.data == b; }
 	friend bool operator==(const SomeElement &a, const SomeElement &b) { return a.data == b.data; }
 public:
 	std::string data;
 };
 using T = Tree<int, SomeElement>;
 using ValueType = T::value_type;
 int main() {
 		DO(T t;)
 		assert(t.empty());
 		assert(t.size() == 0);
 		assert(jlaDistance(t.begin(), t.end()) == t.size());
 		assert(t.begin() == t.end());
 	}
