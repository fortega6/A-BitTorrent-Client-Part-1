#include "merkle_tree.hpp"
#include <stdexcept>

//////////////
#define ROOT 0
#define POW2(exp) (1UL << exp)
#define MAX(x,y) (x > y) ? x : y

size_t minGrPow2(size_t n)

{
    size_t pow = 1UL;
    while (pow < n)
        pow = (pow << 1);

    return pow;
}
//////////////

/**
 * @brief MerkleTree<T>::MerkleTree Class default constructor. Builds
 *                                  an empty Merkle Tree.
 */
template<typename T>
MerkleTree<T>::MerkleTree() : MerkleTree(0)
{
}

/**
 * @brief MerkleTree<T>::MerkleTree Class constructor. Builds an empty tree without
 *                                  root hash large enough to accomodate n blocks.
 * @param n Number of data blocks in the build tree.
 */
template<typename T>
MerkleTree<T>::MerkleTree(size_t n)
{
    numBlocks = n;
    size_t minLeafNum = (n > 2) ? n : 2;
    numPads = minGrPow2(minLeafNum) - n;
    treeSize = 2*(numBlocks + numPads) - 1;
    mktree = new Hash<T>[ treeSize ];
    pad();
}

/**
 * @brief MerkleTree<T>::MerkleTree Class constructor. Builds an empty tree with
 *                                  root hash large enough to accomodate n blocks.
 * @param n         Number of data blocks in the build tree.
 * @param rootHash  Hash of the root node.
 */
template<typename T>
MerkleTree<T>::MerkleTree(size_t n, const Hash<T>& rootHash)
{
    numBlocks = n;
    size_t minLeafNum = (n > 2) ? n : 2;
    numPads = minGrPow2(minLeafNum) - n;
    treeSize = 2 * (numBlocks + numPads) - 1;
    mktree = new Hash<T>[ treeSize ];
    mktree[ROOT] = rootHash;
    pad();
}

/**
 * @brief MerkleTree<T>::MerkleTree Class copy constructor. Builds a Merkle Tree
 *                                  from another given tree
 * @param x The copied Merkle Tree.
 */
template<typename T>
MerkleTree<T>::MerkleTree(const MerkleTree<T>& oth)
{
    //copy other tree data
    mktree = new Hash<T>[ oth.treeSize ];
    for (int i = 0; i < oth.treeSize; ++i)
        mktree[i] = oth.mktree[i];

    treeSize = oth.treeSize;
    numBlocks = oth.numBlocks;
    numPads = oth.numPads;
}

/**
 * @brief MerkleTree<T>::~MerkleTree Class destructor. Release the memory allocated
 *                                   to the tree.
 */
template<typename T>
MerkleTree<T>::~MerkleTree()
{
    delete [] mktree;
}

/**
 * @brief MerkleTree<T>::setRootHash Assign root hash of tree. It throws
 *                                   a std::runtime_error exception if the
 *                                   given hash is empty.
 * @param rootHash Hash set for the root node.
 */
template<typename T>
void MerkleTree<T>::setRootHash(const Hash<T>& rootHash)
{
    if (numBlocks == 0)
        throw std::runtime_error("Runtime Error: Null Merkle Tree or Invalid/Empty Root Hash!");

    mktree[ROOT] = rootHash;
}

/**
 * @brief MerkleTree<T>::getRootHash Return the root hash of Merkle tree.
 * @return  Hash in the root node of Merkle tree. Throws a std::runtime_error
 *          if the root hash is empty.
 */
template<typename T>
Hash<T> MerkleTree<T>::getRootHash()
{
    if ((numBlocks == 0) || mktree[ROOT].isEmpty())
        throw std::runtime_error("Runtime Error: Null Merkle Tree or Invalid/Empty Root Hash!");

    return mktree[ROOT];
}

/**
 * @brief MerkleTree<T>::addBlock Add data block number blockID to the tree
 *                                and calculate descendent hashes if possible.
 * @param blockID   ID for the added data block.
 * @param block     STL sequential container representing the data block.
 * @return          True if the data block is added successfully. Throws
 *                  a std::runtime_error exception if no block-id in the tree.
 */
template<typename T>
bool MerkleTree<T>::addBlock(size_t blockID, const T& block)
{
    if (blockID < 0 || blockID >= numBlocks)
        throw std::runtime_error("Range Error: Invalid Block ID!");

    bool success = true;
    mktree[block2ind(blockID)] = Hash<T>(block);
    updateTree(blockID);

    return success;
}

/**
 * @brief MerkleTree<T>::addBlock Add data block number blockID to the tree
 *                                and calculate descendent hashes if possible.
 * @param blockID   ID for the added data block.
 * @param block     Unsigned char array representing the data block.
 * @return          True if the data block is added successfully. Throws
 *                  a std::runtime_error exception if no block-id in the tree.
 */
template<typename T>
bool MerkleTree<T>::addBlock(size_t blockID, const unsigned char* block, size_t size)
{
    if (blockID < 0 || blockID >= numBlocks)
        throw std::runtime_error("Range Error: Invalid Block ID!");

    bool success = true;
    mktree[block2ind(blockID)] = Hash<T>(block, size);
    updateTree(blockID);

    return success;
}

/**
 * @brief MerkleTree<T>::verifyBlock Verify integrity of block (use sibling and
 *                                   descendents hashes of block hash). If block
 *                                   is verified add hash to tree.
 * @param blockID   ID of the block.
 * @param blockHash Hash of the block.
 * @return          True if block is verified. False otherwise.
 */
template<typename T>
bool MerkleTree<T>::verifyBlock(size_t blockID, const Hash<T>& blockHash)
{
    bool verifFails;
    if (blockID < 0 || blockID >= numBlocks)
        verifFails = true;
    else
        verifFails = false;

    size_t unverNode = block2ind(blockID);
    size_t node = unverNode;
    Hash<T> unverHash = blockHash;

    while (!verifFails && (node > ROOT))
    {
        size_t sibl = getSibling(node);
        Hash<T> h1;
        Hash<T> h2;
        if (node < sibl)    //cur. node is left child
        {
            h1 = unverHash;
            h2 = mktree[sibl];
        }
        else                //cur. node is right child
        {
            h1 = mktree[sibl];
            h2 = unverHash;
        }

        if (!h1.isEmpty() && !h2.isEmpty())
        {
            unverHash = (h1 + h2);
            node = getParent(node);

            if (unverHash != mktree[node])
                verifFails = true;
        }
        else { verifFails = true; }
    }

    bool verified = !verifFails;
    if (verified)
    {
        mktree[unverNode] = blockHash;
        updateTree(blockID);
    }

    return verified;
}

/**
 * @brief MerkleTree<T>::verifyBlock Verify integrity of block using attached list
 *                                   of sibling and descendent hashes. If block is
 *                                   verified add hash to tree and incorporate
 *                                   sibling/descendent hashes.
 * @param blockID   ID of the block to verify.
 * @param blockHash Hash of the block to verify.
 * @param hashList  Contains (in order) hashes for block's sibling and all
 *                  descendents' hashes up until root node.
 * @param size      Number of hashes in in hashList
 * @return          True if block is verified. False otherwise.
 */
template<typename T>
bool MerkleTree<T>::verifyBlock(size_t blockID, const Hash<T>& blockHash, const Hash<T> hashList[], size_t size)
{
    bool verifFails;
    if (blockID < 0 || blockID >= numBlocks || (size != std::log2(numBlocks + numPads)))
        verifFails = true;
    else
        verifFails = false;

    Hash<T> unverHash = blockHash;
    size_t node = block2ind(blockID);

    for (int i = 0; !verifFails && i < size; ++i)
    {
        if (!hashList[i].isEmpty())
            if (node % 2)   //node is left child
                unverHash = unverHash + hashList[i];
            else            //node is right child
                unverHash = hashList[i] + unverHash;
        else
            verifFails = true;

        node = getParent(node);
    }

    bool verified;
    if (!verifFails && mktree[ROOT] == unverHash) //blockID authenticity verified
    {
        verified = true;

        //insert hashes into the tree
        mktree[node = block2ind(blockID)] = blockHash;

        for (int i = 0; i < size; ++i)
        {
            mktree[getSibling(node)] = hashList[i];
            node = getParent(node);
        }

        //update tree, if necessary
        updateTree(blockID);
    }
    else { verified = false; }

    return verified;
}

/**
 * @brief MerkleTree<T>::getLeftChild Return the left child of parent node.
 * @param parentNode    Parent node index.
 * @return              Index of left child node.
 */
template<typename T>
size_t MerkleTree<T>::getLeftChild(size_t parentNode)
{
    return (2*parentNode + 1);
}

/**
 * @brief MerkleTree<T>::getRightChild Return the right child of parent node.
 * @param parentNode    Parent node index.
 * @return              Index of right child node.
 */
template<typename T>
size_t MerkleTree<T>::getRightChild(size_t parentNode)
{
    return (2*parentNode + 2);
}

/**
 * @brief MerkleTree<T>::getParent Return the parent node of a given node.
 * @param childNode Child node index
 * @return          Index of the parent node. Negative value if root node.
 */
template<typename T>
size_t MerkleTree<T>::getParent(size_t childNode)
{
   return (childNode - 1) / 2;
}

/**
 * @brief MerkleTree<T>::getSibling Return the sibling node of a given node.
 * @param childNode Give node index.
 * @return          Index of the sibling node. Negative if root node.
 */
template<typename T>
size_t MerkleTree<T>::getSibling(size_t childNode)
{
    return (childNode % 2) ? childNode + 1 : childNode - 1;
}

/**
 * @brief MerkleTree<T>::getAunt Return the parent's sibling of a given node.
 * @param childNode Given node index.
 * @return          Index of parent's sibling node. Negative if it not exists.
 */
template<typename T>
size_t MerkleTree<T>::getAunt(size_t childNode)
{
    return getSibling(getParent(childNode));
}

/**
 * @brief MerkleTree<T>::block2ind Return the Merkle Tree node index
 *                                 corresponding to the data block's hash.
 * @param blockID   ID of data block.
 * @return          Merkle Tree node index corresponding to hash of block blockID.
 */
template<typename T>
size_t MerkleTree<T>::block2ind(size_t blockID)
{
    return (numBlocks + numPads - 1) + blockID;
}

/**
 * @brief MerkleTree<T>::pad Set hash of padding blocks; also update hashes of
 *                           descendents, if possible
 */
template<typename T>
void MerkleTree<T>::pad()
{
    std::vector<unsigned char> padHash(32, 0);

    for (int id = numBlocks; id < (numBlocks + numPads); ++id)
    {
        mktree[block2ind(id)].setHash(padHash);
        updateTree(id);
    }
}

/**
 * @brief MerkleTree<T>::updateTree Calculate descendent hashes after adding block,
 *                                  if possible.
 * @param blockID   ID of added data block.
 */
template<typename T>
void MerkleTree<T>::updateTree(size_t blockID)
{
    bool missing = false;
    int node = block2ind(blockID);

    while (!missing && (node > ROOT))
    {
        size_t child1 = node;
        size_t child2 = getSibling(node);

        size_t lftChild = std::min(child1, child2);
        size_t rgtChild = std::max(child1, child2);

        Hash<T> h1 = mktree[lftChild];
        Hash<T> h2 = mktree[rgtChild];

        if (h1.isEmpty() || h2.isEmpty())   //if a hash is missing...
            missing = true;                 //stop update
        else                                            //else...
            mktree[node = getParent(node)] = h1 + h2;   //->update parent node hash
    }
}

/**
 * @brief MerkleTree<T>::swap Swap the value of two Merkle Trees.
 * @param x First Merkle Tree.
 * @param y Second Merkle Tree.
 */
template<typename T>
void MerkleTree<T>::swap(MerkleTree<T>& x, MerkleTree<T>& y)
{
    Hash<T>* treePtr;
    size_t aux;

    treePtr  = x.mktree;
    x.mktree = y.mktree;
    y.mktree = treePtr;

    aux = x.numBlocks;
    x.numBlocks = y.numBlocks;
    y.numBlocks = aux;

    aux = x.numPads;
    x.numPads = y.numPads;
    y.numPads = aux;

    aux = x.treeSize;
    x.treeSize = y.treeSize;
    y.treeSize = x.treeSize;
}

/**
 * @brief MerkleTree<T>::operator = Class asignment operator. Set the tree to be a
 *                                  copy of the right hand side merkle tree.
 * @param rhs   Right hand side operand. The copied tree.
 * @return      Reference to the copy tree (this).
 */
template<typename T>
MerkleTree<T>& MerkleTree<T>::operator=(MerkleTree<T> rhs)
{
    if (this != &rhs)   //it is no self-assignment
    {
        if (treeSize < rhs.treeSize)    //there is not enough space at dest.
        {
            delete [] mktree;                       //free up the old space
            mktree = new Hash<T> [rhs.treeSize];    //allocate new space
        }

        //copy right hand side data
        for (int i = 0; i < rhs.treeSize; ++i)
            mktree[i] = rhs.mktree[i];

        treeSize = rhs.treeSize;
        numBlocks = rhs.numBlocks;
        numPads = rhs.numPads;
    }

    return *this;
}

/**
 * @brief operator << Overload ostream operator. Writes the Merkle Tree to an
 *                    output stream.
 * @param os    Output std::ostream.
 * @param t     Merkle Tree writed to the stream
 * @return      std::stream after the tree has been writed out.
 */
template<typename U>
std::ostream& operator<<(std::ostream& os, const MerkleTree<U>& t)
{
    for (int i = 0; i < t.treeSize; ++i)
        os << i << ":" << t.mktree[i] << std::endl;

    return os;
}
