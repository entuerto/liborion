// ART.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ART_H
#define ORION_ART_H

#include <orion/Orion-Stddefs.h>

#include <algorithm>
#include <array>
#include <memory>
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

constexpr int MAX_PREFIX_LEN = 10; /// The maximum prefix length for internal nodes.

struct InnerNode;

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

   template<typename ValueT>
   uint32_t prefix_mismatch(const std::string& key, int depth);

   const Node* minimum() const;
   const Node* maximum() const;

   void grow();

   std::unique_ptr<Node>& find_child(uint8_t c);

   void add_child(uint8_t c, std::unique_ptr<Node> child);

private:
   void add_child4(uint8_t c, std::unique_ptr<Node> child);
   void add_child16(uint8_t c, std::unique_ptr<Node> child);
   void add_child48(uint8_t c, std::unique_ptr<Node> child);
   void add_child256(uint8_t c, std::unique_ptr<Node> child);
};

static std::unique_ptr<Node> null_node{};
//--------------------------------------------------------------------------------------------------

struct InnerNode
{
   uint8_t num_children;
   uint32_t prefix_len;

   std::array<uint8_t, MAX_PREFIX_LEN> prefix;

   void set_prefix(const std::string& key, uint32_t len);

   int mismatch(const std::string& key, int depth) const;
};

//--------------------------------------------------------------------------------------------------

///
/// Small node with only 4 children
///
struct Node4 : public InnerNode
{
   std::array<uint8_t, 4> keys;
   std::array<std::unique_ptr<Node>, 4> children;
};

std::unique_ptr<Node4> make_node4()
{
   return std::make_unique<Node4>();
}

//--------------------------------------------------------------------------------------------------

///
/// Node with 16 children
///
struct Node16 : public InnerNode
{
   std::array<uint8_t, 16> keys;
   std::array<std::unique_ptr<Node>, 16> children;

   void copy(Node4* node4);
};

std::unique_ptr<Node16> make_node16()
{
   return std::make_unique<Node16>();
}

void Node16::copy(Node4* node4)
{
   num_children = node4->num_children;
   prefix_len   = node4->prefix_len;

   // Copy prefix array
   std::copy(std::begin(node4->prefix), std::end(node4->prefix), std::begin(prefix));
   // Copy keys array
   std::copy(std::begin(node4->keys), std::end(node4->keys), std::begin(keys));
   // Move children array
   std::move(std::begin(node4->children), std::end(node4->children), std::begin(children));
}

//--------------------------------------------------------------------------------------------------

///
/// Node with 48 children, but a full 256 byte field.
///
struct Node48 : public InnerNode
{
   std::array<uint8_t, 256> keys;
   std::array<std::unique_ptr<Node>, 48> children;

   void copy(Node16* node16);
};

std::unique_ptr<Node48> make_node48()
{
   return std::make_unique<Node48>();
}

void Node48::copy(Node16* node16)
{
   num_children = node16->num_children;
   prefix_len   = node16->prefix_len;

   // Copy prefix array
   std::copy(std::begin(node16->prefix), std::end(node16->prefix), std::begin(prefix));
   // Copy keys array
   for (int i = 0; i < node16->num_children; ++i)
   {
      keys[node16->keys[i]] = uint8_t(i + 1);
   }
   // Move children array
   std::move(std::begin(node16->children), std::end(node16->children), std::begin(children));
}

//--------------------------------------------------------------------------------------------------

///
/// Full node with 256 children
///
struct Node256 : public InnerNode
{
   std::array<std::unique_ptr<Node>, 256> children;

   void copy(Node48* node48);
};

std::unique_ptr<Node256> make_node256()
{
   return std::make_unique<Node256>();
}

void Node256::copy(Node48* node48)
{
   num_children = node48->num_children;
   prefix_len   = node48->prefix_len;

   // Copy prefix array
   std::copy(std::begin(node48->prefix), std::end(node48->prefix), std::begin(prefix));

   // Move children array
   for (int i = 0; i < 256; ++i)
   {
      if (node48->keys[i] > 0)
         children[i] = std::move(node48->children[node48->keys[i] - 1]);
   }
}

//--------------------------------------------------------------------------------------------------

///
/// Represents a leaf. These are of arbitrary size, as they include the key.
///
template<typename ValueT>
struct Leaf : public InnerNode
{
   std::string key;
   ValueT value;

   Leaf(std::string k, ValueT&& v)
      : key(std::move(k))
      , value(std::move(v))
   {
   }

   bool match(const std::string& k, int depth) const
   {
      (void)depth;

      // Fail if the key lengths are different
      if (key.size() != k.size())
         return false;

      // Compare the keys starting at the depth
      return key.compare(k) == 0;
   }
};

template<class ValueT>
inline std::unique_ptr<Leaf<ValueT>> make_leaf(const std::string& key, ValueT&& value)
{
   return std::make_unique<Leaf<ValueT>>(key, std::move(value));
}

//--------------------------------------------------------------------------------------------------

template<class ValueT>
inline int longest_common_prefix(Leaf<ValueT>* l1, Leaf<ValueT>* l2, int depth)
{
   int max_cmp = std::min(l1->key.length(), l2->key.length()) - depth;

   int idx;
   for (idx = 0; idx < max_cmp; idx++)
   {
      if (l1->key[depth + idx] != l2->key[depth + idx])
         return idx;
   }

   return idx;
}

template<class ValueT>
Leaf<ValueT>* as_leaf(Node* n)
{
   return static_cast<Leaf<ValueT>*>(n->inode.get());
}

template<class ValueT>
const Leaf<ValueT>* as_leaf(const Node* n)
{
   return static_cast<Leaf<ValueT>*>(n->inode.get());
}

Node4* as_node4(Node* n)
{
   return static_cast<Node4*>(n->inode.get());
}

const Node4* as_node4(const Node* n)
{
   return static_cast<const Node4*>(n->inode.get());
}

Node16* as_node16(Node* n)
{
   return static_cast<Node16*>(n->inode.get());
}

const Node16* as_node16(const Node* n)
{
   return static_cast<const Node16*>(n->inode.get());
}

Node48* as_node48(Node* n)
{
   return static_cast<Node48*>(n->inode.get());
}

const Node48* as_node48(const Node* n)
{
   return static_cast<const Node48*>(n->inode.get());
}

Node256* as_node256(Node* n)
{
   return static_cast<Node256*>(n->inode.get());
}

const Node256* as_node256(const Node* n)
{
   return static_cast<const Node256*>(n->inode.get());
}

//--------------------------------------------------------------------------------------------------

//
// Adaptive Radix Tree
//
template<class ValueT>
class ARTree
{
public:
   using KeyType   = std::string;
   using ValueType = ValueT;

   ///
   /// Constructor
   ///
   ARTree() {}

   ///
   /// Destructor
   ///
   ~ARTree() {}

   /// Returns the size of the ART tree.
   uint64_t size() const { return _size; }

   /// Inserts a new value into the ART tree
   ///
   /// @param key The key
   /// @param key_len The length of the key
   /// @param value Specified value.
   /// @return nullptr if the item was newly inserted, otherwise the old value pointer is returned.
   ///
   ValueType insert(KeyType key, ValueType&& value)
   {
      auto ret = recursive_insert(_root, std::move(key), std::move(value), 0);

      if (ret.second)
         _size++;

      return ret.first;
   }

   ///
   /// erases elements
   ///
   /// @param key The key
   /// @param key_len The length of the key
   /// @return nullptr if the item was not found, otherwise the value pointer is returned.
   ///
   ValueType erase(KeyType key) { return ValueType{}; }

   ///
   /// Searches for a value in the ART tree
   ///
   /// @param key The key
   /// @param key_len The length of the key
   /// @return nullptr if the item was not found, otherwise the value pointer is returned.
   ///
   ValueType* search(KeyType key) { return nullptr; }

   ///
   // Returns the minimum valued leaf
   //
   // @return The minimum leaf or null
   //
   ValueType minimum() const
   {
      if (_root == nullptr)
         return ValueType{};

      const Leaf<ValueType>* leaf = as_leaf<ValueType>(_root->minimum());

      if (leaf == nullptr)
         return ValueType{};

      return leaf->value;
   }

   ///
   /// Returns the maximum valued leaf
   ///
   /// @return The maximum leaf or null
   ///
   ValueType maximum() const
   {
      if (_root == nullptr)
         return ValueType{};

      const Leaf<ValueType>* leaf = as_leaf<ValueType>(_root->maximum());

      if (leaf == nullptr)
         return ValueType{};

      return leaf->value;
   }

private:
   std::pair<ValueType, bool> recursive_insert(std::unique_ptr<Node>& node,
                                               KeyType key,
                                               ValueType&& value,
                                               int depth)
   {
      // If we are at a NULL node, inject a leaf
      if (node == nullptr)
      {
         node = std::make_unique<Node>(Kind::Leaf, make_leaf(key, std::move(value)));
         return {ValueType{}, false};
      }

      // If we are at a leaf, we need to replace it with a node
      if (node->is_leaf())
      {
         Leaf<ValueType>* leaf = as_leaf<ValueType>(node.get());

         // Check if we are updating an existing value
         if (not leaf->match(key, depth))
         {
            ValueType old_value = std::move(leaf->value);
            leaf->value         = std::move(value);
            return {old_value, true};
         }

         // New value, we must split the leaf into a node4
         std::unique_ptr<Node> new_leaf =
            std::make_unique<Node>(Kind::Leaf, make_leaf(key, std::move(value)));
         Leaf<ValueType>* leaf2 = as_leaf<ValueType>(new_leaf.get());

         // Determine longest prefix
         int longest_prefix = longest_common_prefix(leaf, leaf2, depth);

         std::unique_ptr<Node> new_node = std::make_unique<Node>(Kind::Node4, make_node4());
         new_node->set_prefix(key.substr(depth), longest_prefix);

         depth += longest_prefix;

         new_node->add_child(leaf->key[depth + longest_prefix], std::move(node));
         new_node->add_child(leaf2->key[depth + longest_prefix], std::move(new_leaf));

         node = std::move(new_node);

         return {ValueType{}, false};
      }

      // Check if given node has a prefix
      if (node->prefix_length() > 0)
      {
         // Determine if the prefixes differ, since we need to split
         uint32_t prefix_diff = node->prefix_mismatch<ValueType>(key, depth);
         if (prefix_diff >= node->prefix_length())
         {
            depth += node->prefix_length();
            goto RECURSE_SEARCH;
         }

         // Create a new node
         std::unique_ptr<Node> new_node = std::make_unique<Node>(Kind::Node4, make_node4());

         new_node->set_prefix(key, prefix_diff);

         // Adjust the prefix of the old node
         if (node->prefix_length() <= MAX_PREFIX_LEN)
         {
            auto& inode = node->inode;

            inode->prefix_len -= (prefix_diff + 1);

            new_node->add_child(inode->prefix[prefix_diff], std::move(node));

            for (int i = 0; i < std::min(MAX_PREFIX_LEN, int(inode->prefix_len)); ++i)
            {
               inode->prefix[i] = inode->prefix[prefix_diff + i + 1];
            }
         }
         else
         {
            auto& inode = node->inode;

            inode->prefix_len -= (prefix_diff + 1);

            const Leaf<ValueType>* l = as_leaf<ValueType>(node->minimum());

            new_node->add_child(l->key[depth + prefix_diff], std::move(node));

            for (int i = 0; i < std::min(MAX_PREFIX_LEN, int(inode->prefix_len)); ++i)
            {
               inode->prefix[i] = l->key[depth + prefix_diff + i + 1];
            }
         }

         // Insert the new leaf
         std::unique_ptr<Node> new_leaf =
            std::make_unique<Node>(Kind::Leaf, make_leaf(key, std::move(value)));
         new_node->add_child(key[depth + prefix_diff], std::move(new_leaf));

         return {ValueType{}, false};
      }

   RECURSE_SEARCH:

      // Find a child to recurse to
      auto& child = node->find_child(key[depth]);
      if (child != nullptr)
      {
         return recursive_insert(child, key, std::move(value), depth + 1);
      }

      // No child, node goes within us
      std::unique_ptr<Node> new_leaf =
         std::make_unique<Node>(Kind::Leaf, make_leaf(key, std::move(value)));
      node->add_child(key[depth], std::move(new_leaf));
      return {ValueType{}, false};
   }

private:
   std::unique_ptr<Node> _root;
   uint64_t _size;
};

//--------------------------------------------------------------------------------------------------
// InnerNode

void InnerNode::set_prefix(const std::string& key, uint32_t len)
{
   prefix_len = len;

   std::copy_n(std::begin(key), std::min(MAX_PREFIX_LEN, int(len)), std::begin(prefix));
}

int InnerNode::mismatch(const std::string& key, int depth) const
{
   uint32_t key_len = key.length();
   uint32_t max_cmp = std::min(std::min(uint32_t(MAX_PREFIX_LEN), prefix_len), key_len - depth);

   int idx;
   for (idx = 0; idx < max_cmp; idx++)
   {
      if (prefix[idx] != key[depth + idx])
         return idx;
   }

   return idx;
}

//--------------------------------------------------------------------------------------------------
// Node

bool Node::is_a(Kind k) const
{
   return kind == k;
}

bool Node::is_leaf() const
{
   return kind == Kind::Leaf;
}

uint32_t Node::prefix_length() const
{
   return inode != nullptr ? inode->prefix_len : 0;
}

void Node::prefix_length(uint32_t value)
{
   inode->prefix_len = value;
}

void Node::set_prefix(const std::string& key, uint32_t len)
{
   inode->set_prefix(key, len);
}

uint32_t Node::max_children() const
{
   switch (kind)
   {
      case Kind::Node4:
         return 2;
      case Kind::Node16:
         return 5;
      case Kind::Node48:
         return 17;
      case Kind::Node256:
         return 49;
      default:
         break;
   }
   return 0;
}

uint32_t Node::min_children() const
{
   switch (kind)
   {
      case Kind::Node4:
         return 4;
      case Kind::Node16:
         return 16;
      case Kind::Node48:
         return 48;
      case Kind::Node256:
         return 256;
      default:
         break;
   }
   return 0;
}

/// Calculates the index at which the prefixes mismatch
template<typename ValueT>
uint32_t Node::prefix_mismatch(const std::string& key, int depth)
{
   int idx = inode->mismatch(key, depth);

   if (idx < MAX_PREFIX_LEN)
      return idx;

   const Leaf<ValueT>* leaf = as_leaf<ValueT>(minimum());

   int limit = std::min(leaf->key.size(), key.size()) - depth;

   for (; idx < limit; idx++)
   {
      if (leaf->key[idx + depth] != key[idx + depth])
         break;
   }

   return idx;
}

/// Find the minimum leaf under a Node
const Node* Node::minimum() const
{
   switch (kind)
   {
      case Kind::Leaf:
         return this;
      case Kind::Node4:
      {
         const Node4* n4 = as_node4(this);
         if (n4->children[0] != nullptr)
            return n4->children[0]->minimum();
         break;
      }
      case Kind::Node16:
      {
         const Node16* n16 = as_node16(this);
         if (n16->children[0] != nullptr)
            return n16->children[0]->minimum();
         break;
      }
      case Kind::Node48:
      {
         const Node48* n48 = as_node48(this);

         int i = 0;
         for (; i < max_children(); ++i)
         {
            if (n48->keys[i] == 0)
               continue;
            break;
         }
         if (n48->children[n48->keys[i] - 1] != nullptr)
            return n48->children[n48->keys[i] - 1]->minimum();
         break;
      }
      case Kind::Node256:
      {
         const Node256* n256 = as_node256(this);

         for (int i = 0; i < max_children(); ++i)
         {
            if (n256->children[i] == nullptr)
               continue;

            return n256->children[i]->minimum();
         }
         break;
      }
   }

   return nullptr;
}

/// Find the maximum leaf under a Node
const Node* Node::maximum() const
{
   switch (kind)
   {
      case Kind::Leaf:
         return this;
      case Kind::Node4:
      {
         const Node4* n4 = as_node4(this);

         return n4->children[n4->num_children - 1]->maximum();
      }
      case Kind::Node16:
      {
         const Node16* n16 = as_node16(this);

         return n16->children[n16->num_children - 1]->maximum();
      }
      case Kind::Node48:
      {
         const Node48* n48 = as_node48(this);

         for (int i = 255; i >= 0; --i)
         {
            if (n48->keys[i] == 0)
               continue;

            n48->children[n48->keys[i] - 1]->maximum();
         }
         break;
      }
      case Kind::Node256:
      {
         const Node256* n256 = as_node256(this);

         for (int i = 255; i >= 0; --i)
         {
            if (n256->children[i] == nullptr)
               continue;

            n256->children[i]->maximum();
         }
         break;
      }
   }

   return nullptr;
}

void Node::grow()
{
   switch (kind)
   {
      case Kind::Node4:
      {
         std::unique_ptr<Node16> new_node = make_node16();

         new_node->copy(static_cast<Node4*>(inode.get()));

         inode = std::move(new_node);
         break;
      }
      case Kind::Node16:
      {
         std::unique_ptr<Node48> new_node = make_node48();

         new_node->copy(static_cast<Node16*>(inode.get()));

         inode = std::move(new_node);
         break;
      }
      case Kind::Node48:
      {
         std::unique_ptr<Node256> new_node = make_node256();

         new_node->copy(static_cast<Node48*>(inode.get()));

         inode = std::move(new_node);
         break;
      }
      default:
         break;
   }
}

std::unique_ptr<Node>& Node::find_child(uint8_t c)
{
   switch (kind)
   {
      case Kind::Node4:
      {
         Node4* n = as_node4(this);

         for (int i = 0; i < n->num_children; i++)
         {
            if (n->keys[i] == c)
               return n->children[i];
         }
         break;
      }
      case Kind::Node16:
      {
         Node16* n = as_node16(this);

         // Compare the key to all 16 stored keys
         int bitfield = 0;
         for (int i = 0; i < 16; ++i)
         {
            if (n->keys[i] == c)
               bitfield |= (1 << i);
         }

         // Use a mask to ignore children that don't exist
         int mask = (1 << n->num_children) - 1;
         bitfield &= mask;

         // If we have a match (any bit set) then we can
         // return the pointer match using ctz to get
         // the index.
         if (bitfield)
            return n->children[__builtin_ctz(bitfield)];
         break;
      }
      case Kind::Node48:
      {
         Node48* n = as_node48(this);

         int i = n->keys[c];
         if (i > 0)
            return n->children[i - 1];
         break;
      }
      case Kind::Node256:
      {
         Node256* n = as_node256(this);
         return n->children[c];
      }
      default:
         break;
   }
   return null_node;
}

void Node::add_child(uint8_t c, std::unique_ptr<Node> child)
{
   switch (kind)
   {
      case Kind::Node4:
         add_child4(c, std::move(child));
         break;
      case Kind::Node16:
         add_child16(c, std::move(child));
         break;
      case Kind::Node48:
         add_child48(c, std::move(child));
         break;
      case Kind::Node256:
         add_child256(c, std::move(child));
         break;
      default:
         break;
   }
}

void Node::add_child4(uint8_t c, std::unique_ptr<Node> child)
{
   Node4* node = as_node4(this);

   if (node->num_children < max_children())
   {
      int idx;
      for (idx = 0; idx < node->num_children; idx++)
      {
         if (c < node->keys[idx])
            break;
      }

      // Shift to make room
      auto keys_first = std::begin(node->keys) + idx;
      auto keys_last  = keys_first + (node->num_children - idx);
      std::move(keys_first, keys_last, keys_first + 1);

      auto children_first = std::begin(node->children) + idx;
      auto children_last  = children_first + (node->num_children - idx);
      std::move(children_first, children_last, children_first + 1);

      // Insert element
      node->keys[idx]     = c;
      node->children[idx] = std::move(child);
      node->num_children++;
      return;
   }

   grow();

   add_child(c, std::move(child));
}

void Node::add_child16(uint8_t c, std::unique_ptr<Node> child)
{
   Node16* node = as_node16(this);

   if (node->num_children < max_children())
   {
      unsigned mask = (1 << node->num_children) - 1;

      // Compare the key to all 16 stored keys
      unsigned bitfield = 0;
      for (short i = 0; i < 16; ++i)
      {
         if (c < node->keys[i])
            bitfield |= (1 << i);
      }

      // Use a mask to ignore children that don't exist
      bitfield &= mask;

      // Check if less than any
      unsigned idx = 0;
      if (bitfield)
      {
         idx = __builtin_ctz(bitfield);

         auto keys_first = std::begin(node->keys) + idx;
         auto keys_last  = keys_first + (node->num_children - idx);
         std::move(keys_first, keys_last, keys_first + idx + 1);

         auto children_first = std::begin(node->children) + idx;
         auto children_last  = children_first + (node->num_children - idx);
         std::move(children_first, children_last, children_first + idx + 1);
      }
      else
         idx = node->num_children;

      // Set the child
      node->keys[idx]     = c;
      node->children[idx] = std::move(child);
      node->num_children++;
      return;
   }

   grow();

   add_child(c, std::move(child));
}

void Node::add_child48(uint8_t c, std::unique_ptr<Node> child)
{
   Node48* node = as_node48(this);

   if (node->num_children < max_children())
   {
      int idx = 0;
      for (; idx < max_children(); ++idx)
      {
         if (node->children[idx] == nullptr)
            break;
      }

      // Set the child
      node->keys[c]       = idx + 1;
      node->children[idx] = std::move(child);
      node->num_children++;
      return;
   }

   grow();

   add_child(c, std::move(child));
}

void Node::add_child256(uint8_t c, std::unique_ptr<Node> child)
{
   Node256* node = as_node256(this);

   // Set the child
   node->children[c] = std::move(child);
   node->num_children++;
}

} // namespace orion

#endif // ORION_ART_H
