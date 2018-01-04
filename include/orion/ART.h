// ART.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ART_H
#define ORION_ART_H

#include <orion/Orion-Stddefs.h>

#include <orion/Exception.h>
#include <orion/Iterator.h>
#include <orion/Throw.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <memory>
#include <ostream>
#include <string>

namespace orion
{

enum class Kind
{
   Leaf    = 0,
   Node4   = 1,
   Node16  = 2,
   Node48  = 3,
   Node256 = 4
};

void format_arg(fmt::BasicFormatter<char>& f, const char*& fmt_str, Kind k);

constexpr uint32_t MaxPrefixLength = 10; /// The maximum prefix length for internal nodes.

struct InnerNode;
struct Node4;
struct Node16;
struct Node48;
struct Node256;

//--------------------------------------------------------------------------------------------------

/// Node class
struct Node
{
   Kind kind;

   std::unique_ptr<InnerNode> inode;

   Node(Kind k, std::unique_ptr<InnerNode> n)
      : kind(k)
      , inode(std::move(n))
   {
   }

   bool is_a(Kind k) const;
   bool is_leaf() const;

   uint32_t max_children() const;
   uint32_t min_children() const;

   uint32_t prefix_length() const;
   void prefix_length(uint32_t value);

   void set_prefix(const std::string& key, uint32_t len);

   template<typename KeyT, typename ValueT>
   uint32_t prefix_mismatch(const KeyT& key, int depth);

   const Node* minimum() const;
   const Node* maximum() const;

   void grow();
   void shrink();

   std::unique_ptr<Node>& find_child(uint8_t c);

   void add_child(uint8_t c, std::unique_ptr<Node> child);

   bool delete_child(uint8_t c);

private:
   void add_child4(uint8_t c, std::unique_ptr<Node> child);
   void add_child16(uint8_t c, std::unique_ptr<Node> child);
   void add_child48(uint8_t c, std::unique_ptr<Node> child);
   void add_child256(uint8_t c, std::unique_ptr<Node> child);

   int delete_child4(uint8_t c);
   int delete_child16(uint8_t c);
   int delete_child48(uint8_t c);
   int delete_child256(uint8_t c);
};

static std::unique_ptr<Node> null_node{};
//--------------------------------------------------------------------------------------------------

struct InnerNode
{
   uint8_t num_children;
   uint32_t prefix_len;

   std::array<uint8_t, MaxPrefixLength> prefix;

   void set_prefix(const std::string& key, uint32_t len);

   int mismatch(const std::string& key, int depth) const;
};

//--------------------------------------------------------------------------------------------------

///
/// Small node with only 4 children
///
struct Node4 : public InnerNode
{
   std::array<uint8_t, 4> keys{};
   std::array<std::unique_ptr<Node>, 4> children;

   void copy(Node16* node16);

   int index(uint8_t c) const;
};

std::unique_ptr<Node4> make_node4();

//--------------------------------------------------------------------------------------------------

///
/// Node with 16 children
///
struct Node16 : public InnerNode
{
   std::array<uint8_t, 16> keys{};
   std::array<std::unique_ptr<Node>, 16> children;

   void copy(Node4* node4);
   void copy(Node48* node48);

   int index(uint8_t c) const;
};

std::unique_ptr<Node16> make_node16();

//--------------------------------------------------------------------------------------------------

///
/// Node with 48 children, but a full 256 byte field.
///
struct Node48 : public InnerNode
{
   std::array<uint8_t, 256> keys{};
   std::array<std::unique_ptr<Node>, 48> children;

   void copy(Node16* node16);
   void copy(Node256* node256);

   int index(uint8_t c) const;
};

std::unique_ptr<Node48> make_node48();

//--------------------------------------------------------------------------------------------------

///
/// Full node with 256 children
///
struct Node256 : public InnerNode
{
   std::array<std::unique_ptr<Node>, 256> children;

   void copy(Node48* node48);

   int index(uint8_t c) const;
};

std::unique_ptr<Node256> make_node256();

//--------------------------------------------------------------------------------------------------

///
/// Represents a leaf. These are of arbitrary size, as they include the key.
///
template<typename KeyT, typename ValueT>
struct Leaf : public InnerNode
{
   KeyT key;
   ValueT value;

   Leaf(KeyT k, ValueT&& v);

   bool match(const KeyT& k, int depth) const;
};

template<typename KeyT, typename ValueT>
inline std::unique_ptr<Leaf<KeyT, ValueT>> make_leaf(const KeyT& key, ValueT&& value)
{
   return std::make_unique<Leaf<KeyT, ValueT>>(key,  std::forward<ValueT>(value));
}

//--------------------------------------------------------------------------------------------------

template<typename KeyT, typename ValueT>
class TreeDumper;

template<typename KeyT, typename ValueT>
class NodeIterator;

//--------------------------------------------------------------------------------------------------

//
// Adaptive Radix Tree
//
template<class KeyT, class ValueT>
class ARTree
{
public:
   NO_COPY(ARTree)

   using key_type        = KeyT;
   using value_type      = ValueT;
   using reference       = ValueT&;
   using const_reference = const ValueT&;
   using iterator        = NodeIterator<key_type, ValueT>;
   using const_iterator  = NodeIterator<key_type, const ValueT>;
   using difference_type = std::ptrdiff_t;
   using size_type       = std::size_t;

   ///
   /// Constructor
   ///
   ARTree() = default;

   ///
   /// Destructor
   ///
   ~ARTree() = default;

   /// Move 
   DEFAULT_MOVE(ARTree)

   iterator begin() noexcept { return {}; }
   const_iterator begin() const noexcept { return {}; }
   const_iterator cbegin() const noexcept { return {}; }

   iterator end() noexcept { return {}; }
   const_iterator end() const noexcept { return {}; }
   const_iterator cend() const noexcept { return {}; }

   /// Indicates if the tree has any elements.
   bool empty() const noexcept { return _size == 0; }

   /// Returns the size of the ART tree.
   size_type size() const noexcept { return _size; }

   ///
   /// Inserts a new value into the ART tree
   ///
   /// @param key The key
   /// @param key_len The length of the key
   /// @param value Specified value.
   /// @return nullptr if the item was newly inserted, otherwise the old value pointer is returned.
   ///
   value_type insert(key_type key, value_type&& value);

   ///
   /// erases elements
   ///
   /// @param key The key
   /// @param key_len The length of the key
   /// @return nullptr if the item was not found, otherwise the value pointer is returned.
   ///
   value_type erase(const key_type& key);

   ///
   /// Finds an element with key equivalent to key.
   ///
   /// @param key The key
   /// @param key_len The length of the key
   /// @return Iterator to an element with key equivalent to key. If no such element is found,
   ///         past-the-end iterator is returned.
   ///
   iterator find(const key_type& key);

   ///
   // Returns the minimum valued leaf
   //
   // @return The minimum leaf or null
   //
   const_iterator minimum() const;

   ///
   /// Returns the maximum valued leaf
   ///
   /// @return The maximum leaf or null
   ///
   const_iterator maximum() const;

   void dump(std::ostream& o) const;

private:
   std::pair<value_type, bool> recursive_erase(std::unique_ptr<Node>& node,
                                               const key_type& key,
                                               int depth);

   std::pair<value_type, bool> recursive_insert(std::unique_ptr<Node>& node,
                                                key_type key,
                                                value_type&& value,
                                                int depth);

private:
   std::unique_ptr<Node> _root;
   uint64_t _size = 0;
};

} // namespace orion

#include <orion/impl/ART.ipp>

#endif // ORION_ART_H
