#include "Tree.hpp"

namespace DM852 {
template<typename Key, typename Value>
using value_type = std::pair<const Key, Value>;



/**
 * @return: whether or not other is equal to this
 */
// bool operator==(const Node &l, const Node &r) {
//     return l.key == r.key && l.value == r.value;
// }

/**
 * @return: whether or not other is not equal to this
 */
// bool operator!=(const Node &l, const Node &r) {
//     return !(l == r);
// }

/**
 * @return: whether or not other is equal to this
 */
// bool operator==(const Tree &l, const Tree &r) {
//     if ( l.size() != r.size() ) return false;
    
//     const Node *le = l.begin();
//     const Node *re = r.begin();
//     while ( le != nullptr ) {
//         if ( *le != *re ) return false;
//         le = le->next();
//         re = re->next();
//     }
//     return true;
// }

/**
 * @return: whether or not other is not equal to this
 */
// bool operator!=(const Tree &l, const Tree &r) {
//     return !(l == r);
// }

}