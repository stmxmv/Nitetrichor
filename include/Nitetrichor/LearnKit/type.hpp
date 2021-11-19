/*!
* Created by Molybdenum on 10/14/21.
*
*/
#ifndef NITETRICHOR_TYPE_HPP
#define NITETRICHOR_TYPE_HPP
namespace NC {


/// \brief we don't use the C's isdigit(int), because the char to int conversion causes portability problems
/// \param ch
/// \return
inline bool IsDigit(char ch) {
    return ch >= '0' && ch <= '9';
}



}

#endif //NITETRICHOR_TYPE_HPP
