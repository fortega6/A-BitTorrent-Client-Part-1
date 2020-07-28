#include "hash.hpp"
#include <stdexcept>

/**
 * @brief Hash<T>::Hash Class default constructor. Builds an empty hash.
 */
template <typename T>
Hash<T>::Hash()
{
}

/**
 * @brief Hash<T>::Hash Class constructor. Builds a hash from a byte sequence.
 * @param data  Unsigned char array representing the byte sequence.
 * @param size  Number the bytes in the sequence.
 */
template <typename T>
Hash<T>::Hash(const unsigned char *data, size_t size)
{
  // picosha2 requires h to be preallocated
  h = std::vector<unsigned char>(32);  
  picosha2::hash256(data, data+size, h);
}

/**
 * @brief Hash<T>::Hash Class constructor. Builds a hash from any STL sequential container.
 * @param data  STL sequential container.
 */
template <typename T>
Hash<T>::Hash(const T& data)
{
  // picosha2 requires h to be preallocated
  h = std::vector<unsigned char>(32);
  picosha2::hash256(data, h);
}

/**
 * @brief Hash<T>::Hash Class copy constructor. Builds a hash from another
 *                      hash given.
 * @param x The other hash.
 */
template<typename T>
Hash<T>::Hash(const Hash<T>& x)
{
   h = x.h;
}

/**
 * @brief Hash<T>::Hash Class private constructor. Builds a hash as the
 *                      combination of two given hashes.
 * @param x First given hash.
 * @param y Second given hash.
 */
template<typename T>
Hash<T>::Hash(const Hash<T>& x, const Hash<T>& y)
{
    std::vector<unsigned char> xy;
    xy.reserve(64);
    xy.insert(xy.end(), x.h.begin(), x.h.end());
    xy.insert(xy.end(), y.h.begin(), y.h.end());

    h = std::vector<unsigned char>(32);
    picosha2::hash256(xy, h);
}

/**
 * @brief Hash<T>::~Hash Class destructor.
 */
template<typename T>
Hash<T>::~Hash()
{
}

/**
 * @brief Hash<T>::setHash Assign hash (in byte form). Should be
 *                         rarely used (use constructors instead).
 * @param x STL vector of unsigned char representing the hash in byte form.
 */
template<typename T>
void Hash<T>::setHash(const std::vector<unsigned char>& x)
{
    if (x.size() != 32)
        throw std::runtime_error("Runtime Error: Invalid Hash!");

    h = x;
}

/**
 * @brief Hash<T>::returnHash Return hash to user (in byte form).
 * @return  STL vector of unsigned char representing the hash in byte form.
 *          If the hash is empty it throws a std::runtime_error exception.
 */
template<typename T>
std::vector<unsigned char> Hash<T>::returnHash()
{
    if (h.size() != 32)
        throw std::runtime_error("Runtime Error: Invalid Hash!");

    return h;
}

/**
 * @brief Hash<T>::returnHashString Return hash in hex string form.
 * @return Standar string representing the hash in hex form. If the
 *         hash is empty it throws a std::runtime_error exception.
 */
template<typename T>
std::string Hash<T>::returnHashString()
{
    if (h.size() != 32)
        throw std::runtime_error("Runtime Error: Invalid/Empty Hash!");

    return picosha2::bytes_to_hex_string(h);
}

/**
 * @brief Hash<T>::isEmpty Tells us whether hash has been set or only the default (empty).
 * @return  True if hash is an empty default hash. False otherwise.
 */
template<typename T>
bool Hash<T>::isEmpty() const
{
    return (h.size() != 32);
}

/**
 * @brief swap Swap the values of two hashes.
 * @param x First hash.
 * @param y Second hash.
 */
template<typename T>
void Hash<T>::swap(Hash<T>& x, Hash<T>& y)
{
    std::swap(x.h, y.h);
}

/**
 * @brief Hash<T>::operator = Class assignment operator. Set hash to be a
 *                            copy of the other given hash.
 * @param rhs   Right hand side operand. The copied hash.
 * @return      Reference to the copy hash (this).
 */
template<typename T>
Hash<T>& Hash<T>::operator=(Hash<T> rhs)
{
    if (this != &rhs)
        h = rhs.h;

    return *this;
}

/**
 * @brief Hash<T>::operator == Class comparison operator (lhs == rhs).
 * @param rhs   The right hand side hash.
 * @return      True if the two hashes are equals. False otherwise.
 */
template<typename T>
bool Hash<T>::operator==(const Hash<T>& rhs) const
{
    return (h == rhs.h);
}

/**
 * @brief Hash<T>::operator != Class comparison operator (lhs != rhs).
 * @param rhs   The right hand side hash.
 * @return      True if the two hashes are different. False otherwise.
 */
template<typename T>
bool Hash<T>::operator!=(const Hash<T>& rhs) const
{
    return (h != rhs.h);
}

/**
 * @brief Hash<T>::operator + Class addition operator: result = hash(lhs||rhs),
 *                            where || means the concatenation of the two hashes.
 * @param rhs   The right hand side hash.
 * @return      Hash of the concatenation of the left and right side hashes.
 *              Throws a std::runtime_error if any of two hashes is an empty hash.
 */
template<typename T>
Hash<T> Hash<T>::operator+(const Hash<T>& rhs) const
{
    if ((h.size() != 32) || (rhs.h.size() != 32))
        throw std::runtime_error("Runtime Error: Invalid Hash Operand!");

    std::vector<unsigned char> cat;
    cat.reserve(64);
    cat.insert(cat.end(), h.begin(), h.end());
    cat.insert(cat.end(), rhs.h.begin(), rhs.h.end());

    std::vector<unsigned char> sum(32);
    picosha2::hash256(cat, sum);

    Hash<T> hash;
    hash.setHash(sum);

    return hash;
}

/**
 * @brief operator << Overload ostream operator.
 * @param os    Output std::ostream.
 * @param x     Hash sets to the output stream.
 * @return      Output stream.
 */
template<typename U>
std::ostream& operator<<(std::ostream& os, const Hash<U>& x)
{
    return (os << picosha2::bytes_to_hex_string(x.h));
}
