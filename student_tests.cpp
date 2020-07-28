#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE

#include "catch.hpp"
#include "hash.hpp"
#include "merkle_tree.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

TEST_CASE( "Hash<std::string>", "[Hash<T>]" )
{
    INFO("Hint: testing Hash<T> STL container constructor");

    std::string str = "The quick brown fox jumps over the lazy dog";
    Hash<std::string> hash(str);

    CHECK(hash.returnHashString() == std::string("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"));
}

TEST_CASE( "Hash<unsigned char*>", "[Hash<T>]" )
{
    INFO("Hint: testing Hash<T> byte array constructor");

    unsigned char ary[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    Hash<unsigned char*> hash(ary,sizeof(ary));

    CHECK(hash.returnHashString() == std::string("17e88db187afd62c16e5debf3e6527cd006bc012bc90b51a810cd80c2d511f43"));
}

TEST_CASE( "Hash<T>: Default Constructor", "[Hash<T>]" )
{
    INFO("Hint: testing Hash<T> default constructor and Hash<T>::isEmpty function");

    Hash<std::string> hash;
    REQUIRE(hash.isEmpty());
}

TEST_CASE( "Hash<T>: Assign Hash (in byte form)", "[Hash<T>]")
{
    INFO("Hint: testing Hash<T>::setHash");

    std::vector<unsigned char> byteVec1(32, 0xFA);

    Hash<std::string> hash;
    hash.setHash(byteVec1);

    REQUIRE(hash.returnHashString() == std::string("fafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafa"));

    /////
    std::vector<unsigned char> byteVec2(32, 0);

    Hash<std::string> padHash;
    padHash.setHash(byteVec2);

    REQUIRE(padHash.returnHashString() == std::string(64,'0'));
    /////
}

TEST_CASE( "Hash<T>: swap", "[Hash<T>]" )
{
    INFO("Hint: testing Hash<T>::swap");

    Hash<std::string> hash1(std::string("the wind in the willows"));
    Hash<std::string> hash2(std::string("le petit prince"));
    swap(hash1, hash2);

    REQUIRE(hash1.returnHashString() == std::string("4bdeb51ac20d14fcb36ff6a5d76a6190f9a46bde54cca69597e916b9017d7951"));
    REQUIRE(hash2.returnHashString() == std::string("bd8c4649d14228bdba925efcb9a47d1b96134828fe730b4ebed896acf00babe6"));
}

TEST_CASE( "Hash<T>: operators =, ==, !=, +, <<", "[Hash<T>]" )
{
    INFO("Hint: testing Hash<T> operator overloads (=, ==, !=, +, <<)");

    std::string str = "The quick brown fox jumps over the lazy dog";

    Hash<std::string> hash1(str);
    Hash<std::string> hash2(str);
    REQUIRE(hash1 == hash2);

    std::vector<unsigned char> padHashVec(32, 0);
    Hash<std::string> padHash;
    padHash.setHash(padHashVec);

    REQUIRE(hash1 != padHash);
    REQUIRE(hash2 != padHash);

    hash2 = padHash;
    REQUIRE(hash2 == padHash);
    REQUIRE(hash1 != hash2);

    Hash<std::string> hashSum = hash1 + hash2;
    REQUIRE(hashSum.returnHashString() == std::string("d04c7205e92a31593bc4623161cb8158284de7a47595977e4e63923c7c50bca6"));

    std::ostringstream oss;
    oss << hashSum;
    REQUIRE(oss.str() == hashSum.returnHashString());

    std::cout << "HASH_SUM_STR: " << hashSum << std::endl;
}

TEST_CASE( "MerkleTree<std::string>", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::addBlock");
    MerkleTree<std::string> t(3);

    std::string str = "The quick brown fox jumps over the lazy dog";

    t.addBlock(0, str);
    t.addBlock(1, str);
    t.addBlock(2, str);

    CHECK(t.getRootHash().returnHashString() == std::string("545cf39de35c920380aed7a679c88ff265fde7dd5dd09f207131ae3fc28e247b"));
}

TEST_CASE( "Merkle Tree Dafualt Constructor", "[MerkleTree<T>]" )
{
    INFO("Hint: Testing MerkleTree<T>::MerkleTree()");

    MerkleTree<std::string> t;  //empty merkle tree

    try
    {
        t.getRootHash();
        std::cout << "This message never must be show!" << std::endl;
        REQUIRE(false);
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
        REQUIRE(true);
    }
}

TEST_CASE( "Merkle Tree Constructor 1: Blocks Number", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::MerkleTree(size_t)");

    std::vector<unsigned char> byteArr(32, 0xFA);

    Hash<std::string> rootHash;
    rootHash.setHash(byteArr);

    MerkleTree<std::string> t(3);
    t.setRootHash(rootHash);

    REQUIRE(t.getRootHash().returnHashString() == std::string("fafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafa"));
}

TEST_CASE( "Merkle Tree Copy Constructor", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::MerkleTree(const MerkleTree<T>&)");

    std::string str = "The quick brown fox jumps over the lazy dog";

    Hash<std::string> rootHash1(str);
    MerkleTree<std::string> t1(4, rootHash1);
    REQUIRE(t1.getRootHash().returnHashString() == std::string("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"));

    MerkleTree<std::string> t2(t1);
    REQUIRE(t2.getRootHash().returnHashString() == std::string("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"));
    REQUIRE(t1.getRootHash().returnHashString() == t2.getRootHash().returnHashString());
}

TEST_CASE( "Merkle Tree Assignment operator", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::operator=(const MerkleTree<T>&)");

    std::string str = "The quick brown fox jumps over the lazy dog";

    ////
    Hash<std::string> rootHash1(str);
    MerkleTree<std::string> t1(4, rootHash1);

    REQUIRE(t1.getRootHash().returnHashString() == std::string("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"));
    ////

    ////
    MerkleTree<std::string> t2(3);

    t2.addBlock(0, str);
    t2.addBlock(1, str);
    t2.addBlock(2, str);

    REQUIRE(t2.getRootHash().returnHashString() == std::string("545cf39de35c920380aed7a679c88ff265fde7dd5dd09f207131ae3fc28e247b"));
    ////

    t1 = t2;
    REQUIRE(t1.getRootHash().returnHashString() == std::string("545cf39de35c920380aed7a679c88ff265fde7dd5dd09f207131ae3fc28e247b"));
    REQUIRE(t1.getRootHash().returnHashString() == t2.getRootHash().returnHashString());
}

TEST_CASE( "Merkle Tree addBlock", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::addBlock (version 1 and 2)");

    /////////////////
    /// add Block V1
    std::string str = "The quick brown fox jumps over the lazy dog";

    MerkleTree<std::string> t1(3);
    t1.addBlock(0, str);
    t1.addBlock(1, str);
    t1.addBlock(2, str);

    REQUIRE(t1.getRootHash().returnHashString() == std::string("545cf39de35c920380aed7a679c88ff265fde7dd5dd09f207131ae3fc28e247b"));
    /////////////////

    /////////////////
    /// add Block V2
    char* cstr = (char*)"The quick brown fox jumps over the lazy dog";

    MerkleTree<std::string> t2(3);
    t2.addBlock(0, (unsigned char*)cstr, strlen(cstr));
    t2.addBlock(1, (unsigned char*)cstr, strlen(cstr));
    t2.addBlock(2, (unsigned char*)cstr, strlen(cstr));

    REQUIRE(t2.getRootHash().returnHashString() == std::string("545cf39de35c920380aed7a679c88ff265fde7dd5dd09f207131ae3fc28e247b"));
    /////////////////

    REQUIRE(t1.getRootHash() == t2.getRootHash());
}

TEST_CASE( "Merkle Tree Swap", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::swap(MerkleTree<T>&, MerkleTree<T>&)");

    std::string str = "The quick brown fox jumps over the lazy dog";

    ////
    Hash<std::string> rootHash1(str);
    MerkleTree<std::string> t1(4, rootHash1);

    REQUIRE(t1.getRootHash().returnHashString() == std::string("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"));
    ////

    ////
    MerkleTree<std::string> t2(3);

    t2.addBlock(0, str);
    t2.addBlock(1, str);
    t2.addBlock(2, str);

    REQUIRE(t2.getRootHash().returnHashString() == std::string("545cf39de35c920380aed7a679c88ff265fde7dd5dd09f207131ae3fc28e247b"));
    ////

    swap(t1, t2);
    REQUIRE(t1.getRootHash().returnHashString() == std::string("545cf39de35c920380aed7a679c88ff265fde7dd5dd09f207131ae3fc28e247b"));
    REQUIRE(t2.getRootHash().returnHashString() == std::string("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"));
}

TEST_CASE( "Merkle Tree Verify Block (1)", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::verifyBlock(size_t, const Hash<T>&)");
    MerkleTree<std::string> t(3);

    std::string str = "The quick brown fox jumps over the lazy dog";

    t.addBlock(0, str);
    t.addBlock(1, str);
    t.addBlock(2, str);

    Hash<std::string> blockHash(str);

    REQUIRE(t.verifyBlock(0, blockHash));
    REQUIRE(t.verifyBlock(1, blockHash));
    REQUIRE(t.verifyBlock(2, blockHash));
}

TEST_CASE( "Merkle Tree Verify Block (2)", "[MerkleTree<T>]" )
{
    INFO("Hint: testing MerkleTree<T>::verifyBlock(size_t, const Hash<T>&, const Hash<T>[], size_t)");

    std::string str = "The quick brown fox jumps over the lazy dog";
    std::vector<unsigned char> rawPadVec(32, 0);
    Hash<std::string> padHash;
    padHash.setHash(rawPadVec);

    Hash<std::string> blockHash(str);
    Hash<std::string> hashList[2];
    hashList[0] = blockHash;
    hashList[1] = blockHash + padHash;

    MerkleTree<std::string> t(3);
    t.addBlock(0, str);
    t.addBlock(1, str);
    t.addBlock(2, str);

    //valid verification
    REQUIRE(t.verifyBlock(0, blockHash, hashList, 2));
    REQUIRE(t.verifyBlock(1, blockHash, hashList, 2));

    //invalid verification
    REQUIRE(t.verifyBlock(2, blockHash, hashList, 2) == false);
    REQUIRE(t.verifyBlock(0, Hash<std::string>(str + "H"), hashList, 2) == false);
}
