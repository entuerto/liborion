//
// ART.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ART_IPP
#define ORION_ART_IPP

#include <orion/Orion-Stddefs.h>

namespace orion
{

inline void format_arg(fmt::BasicFormatter<char>& f, const char*& /*fmt_str*/, Kind k)
{
   auto& w = f.writer();

   switch (k)
   {
      case Kind::Node4:
         w.write("Node4");
         break;
      case Kind::Node16:
         w.write("Node16");
         break;
      case Kind::Node48:
         w.write("Node48");
         break;
      case Kind::Node256:
         w.write("Node256");
         break;
      case Kind::Leaf:
         w.write("Leaf");
         break;
   }
}

//--------------------------------------------------------------------------------------------------

template<typename KeyT, typename ValueT>
inline int longest_common_prefix(Leaf<KeyT, ValueT>* l1, Leaf<KeyT, ValueT>* l2, int depth)
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

template<typename KeyT, typename ValueT>
Leaf<KeyT, ValueT>* as_leaf(Node* n)
{
   return static_cast<Leaf<KeyT, ValueT>*>(n->inode.get());
}

template<typename KeyT, typename ValueT>
const Leaf<KeyT, ValueT>* as_leaf(const Node* n)
{
   return static_cast<Leaf<KeyT, ValueT>*>(n->inode.get());
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
// InnerNode

void InnerNode::set_prefix(const std::string& key, uint32_t len)
{
   prefix_len = len;

   std::copy_n(std::begin(key), std::min(MaxPrefixLength, len), std::begin(prefix));
}

int InnerNode::mismatch(const std::string& key, int depth) const
{
   uint32_t key_len = key.length();
   uint32_t max_cmp = std::min(std::min(MaxPrefixLength, prefix_len), key_len - depth);

   int idx;
   for (idx = 0; idx < max_cmp; idx++)
   {
      if (prefix[idx] != key[depth + idx])
         return idx;
   }

   return idx;
}

//--------------------------------------------------------------------------------------------------
// Node4

inline std::unique_ptr<Node4> make_node4()
{
   return std::make_unique<Node4>();
}

void Node4::copy(Node16* node16)
{
   prefix_len = node16->prefix_len;

   // Copy prefix array
   std::copy(std::begin(node16->prefix), std::end(node16->prefix), std::begin(prefix));

   num_children = 0;

   for (int i = 0; i < keys.size(); i++)
   {
      keys[i]     = node16->keys[i];
      children[i] = std::move(node16->children[i]);
      num_children++;
   }
}

int Node4::index(uint8_t c) const
{
   for (int i = 0; i < num_children; i++)
   {
      if (keys[i] == c)
         return i;
   }
   return -1;
}

//--------------------------------------------------------------------------------------------------
// Node16

inline std::unique_ptr<Node16> make_node16()
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

void Node16::copy(Node48* node48)
{
   prefix_len = node48->prefix_len;

   // Copy prefix array
   std::copy(std::begin(node48->prefix), std::end(node48->prefix), std::begin(prefix));

   num_children = 0;
   for (int i = 0; i < node48->keys.size(); i++)
   {
      uint8_t idx = node48->keys[i];
      if (idx <= 0)
         continue;

      if (node48->children[idx - 1] != nullptr)
      {
         children[num_children] = std::move(node48->children[idx - 1]);
         keys[num_children]     = uint8_t(i);
         num_children++;
      }
   }
}

int Node16::index(uint8_t c) const
{
   // Compare the key to all 16 stored keys
   int bitfield = 0;
   for (int i = 0; i < 16; ++i)
   {
      if (keys[i] == c)
         bitfield |= (1 << i);
   }

   // Use a mask to ignore children that don't exist
   int mask = (1 << num_children) - 1;
   bitfield &= mask;

   // If we have a match (any bit set) then we can
   // return the pointer match using ctz to get
   // the index.
   if (bitfield != 0u)
      return __builtin_ctz(bitfield);

   return -1;
}

//--------------------------------------------------------------------------------------------------
// Node48

inline std::unique_ptr<Node48> make_node48()
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

void Node48::copy(Node256* node256)
{
   prefix_len = node256->prefix_len;

   // Copy prefix array
   std::copy(std::begin(node256->prefix), std::end(node256->prefix), std::begin(prefix));

   num_children = 0;

   for (int i = 0; i < node256->children.size(); i++)
   {
      if (node256->children[i] != nullptr)
      {
         children[num_children] = std::move(node256->children[i]);
         keys[i]                = uint8_t(num_children + 1);
         num_children++;
      }
   }
}

int Node48::index(uint8_t c) const
{
   int i = keys[c];

   return (i > 0) ? i - 1 : -1;
}

//--------------------------------------------------------------------------------------------------
// Node256

inline std::unique_ptr<Node256> make_node256()
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

int Node256::index(uint8_t c) const
{
   return c;
}

//--------------------------------------------------------------------------------------------------
// Leaf

template<typename KeyT, typename ValueT>
Leaf<KeyT, ValueT>::Leaf(KeyT k, ValueT&& v)
   : InnerNode()
   , key(std::move(k))
   , value(std::move(v))
{
}

template<typename KeyT, typename ValueT>
bool Leaf<KeyT, ValueT>::match(const KeyT& k, int depth) const
{
   (void)depth;

   // Fail if the key lengths are different
   if (key.size() != k.size())
      return false;

   // Compare the keys starting at the depth
   return key == k;
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
template<typename KeyT, typename ValueT>
uint32_t Node::prefix_mismatch(const KeyT& key, int depth)
{
   int idx = inode->mismatch(key, depth);

   if (idx < MaxPrefixLength)
      return idx;

   const auto* leaf = as_leaf<KeyT, ValueT>(minimum());

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

         kind  = Kind::Node16;
         inode = std::move(new_node);
         break;
      }
      case Kind::Node16:
      {
         std::unique_ptr<Node48> new_node = make_node48();

         new_node->copy(static_cast<Node16*>(inode.get()));

         kind  = Kind::Node48;
         inode = std::move(new_node);
         break;
      }
      case Kind::Node48:
      {
         std::unique_ptr<Node256> new_node = make_node256();

         new_node->copy(static_cast<Node48*>(inode.get()));

         kind  = Kind::Node256;
         inode = std::move(new_node);
         break;
      }
      default:
         break; // Error
   }
}

void Node::shrink()
{
   switch (kind)
   {
      // Shrink to Leaf when children[0] is leaf else replace inode with child.
      case Kind::Node4:
      {
         Node4* node4 = as_node4(this);

         auto& child = node4->children[0];
         if (child->is_leaf())
         {
            kind  = Kind::Leaf;
            inode = std::move(child->inode);
            return;
         }

         uint32_t current_prefix_len = node4->prefix_len;
         if (current_prefix_len < MaxPrefixLength)
         {
            node4->prefix[current_prefix_len] = node4->keys[0];
            current_prefix_len++;
         }

         auto& child_inode = child->inode;

         if (current_prefix_len < MaxPrefixLength)
         {
            uint32_t child_prefix_len =
               std::min(child_inode->prefix_len, MaxPrefixLength - current_prefix_len);

            for (int i = 0; i < child_prefix_len; i++)
            {
               node4->prefix[current_prefix_len + i] = child_inode->prefix[i];
            }
            current_prefix_len += child_prefix_len;
         }

         for (int i = 0; i < std::min(current_prefix_len, MaxPrefixLength); i++)
         {
            child_inode->prefix[i] = node4->prefix[i];
         }

         child_inode->prefix_len += node4->prefix_len + 1;

         inode = std::move(child->inode);
         break;
      }
      // Shrink to Node4
      case Kind::Node16:
      {
         std::unique_ptr<Node4> new_node = make_node4();

         new_node->copy(static_cast<Node16*>(inode.get()));

         kind  = Kind::Node4;
         inode = std::move(new_node);
         break;
      }
      // Shrink to Node16
      case Kind::Node48:
      {
         std::unique_ptr<Node16> new_node = make_node16();

         new_node->copy(static_cast<Node48*>(inode.get()));

         kind  = Kind::Node16;
         inode = std::move(new_node);
         break;
      }
      // Shrink to Node48
      case Kind::Node256:
      {
         std::unique_ptr<Node48> new_node = make_node48();

         new_node->copy(static_cast<Node256*>(inode.get()));

         kind  = Kind::Node48;
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

         int idx = n->index(c);

         if (idx != -1)
            return n->children[idx];

         break;
      }
      case Kind::Node16:
      {
         Node16* n = as_node16(this);

         int idx = n->index(c);

         if (idx != -1)
            return n->children[idx];

         break;
      }
      case Kind::Node48:
      {
         Node48* n = as_node48(this);

         int idx = n->index(c);

         if (idx != -1)
            return n->children[idx];

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
      if (bitfield != 0u)
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

bool Node::delete_child(uint8_t c)
{
   int nc = -1;
   switch (kind)
   {
      case Kind::Node4:
         nc = delete_child4(c);
         break;
      case Kind::Node16:
         nc = delete_child16(c);
         break;
      case Kind::Node48:
         nc = delete_child48(c);
         break;
      case Kind::Node256:
         nc = delete_child256(c);
         break;
      default:
         break;
   }

   if (nc != -1 and nc < min_children())
   {
      shrink();
      return true;
   }
   return false;
}

int Node::delete_child4(uint8_t c)
{
   Node4* node = as_node4(this);

   int idx = node->index(c);
   if (idx >= 0)
   {
      node->num_children--;
      node->keys[idx] = 0;
      node->children[idx].reset();

      for (int i = idx; i <= node->num_children and i + 1 < node->keys.size(); i++)
      {
         node->keys[i]     = node->keys[i + 1];
         node->children[i] = std::move(node->children[i + 1]);
      }

      node->keys[node->num_children]     = 0;
      node->children[node->num_children] = nullptr;
   }
   return node->num_children;
}

int Node::delete_child16(uint8_t c)
{
   Node16* node = as_node16(this);

   int idx = node->index(c);
   if (idx >= 0)
   {
      node->num_children--;
      node->keys[idx] = 0;
      node->children[idx].reset();

      for (int i = idx; i <= node->num_children and i + 1 < node->keys.size(); i++)
      {
         node->keys[i]     = node->keys[i + 1];
         node->children[i] = std::move(node->children[i + 1]);
      }

      node->keys[node->num_children]     = 0;
      node->children[node->num_children] = nullptr;
   }
   return node->num_children;
}

int Node::delete_child48(uint8_t c)
{
   Node48* node = as_node48(this);

   int idx = node->index(c);
   if (idx >= 0 and node->children[idx] != nullptr)
   {
      node->num_children--;
      node->keys[idx] = 0;
      node->children[idx].reset();
   }
   return node->num_children;
}

int Node::delete_child256(uint8_t c)
{
   Node256* node = as_node256(this);

   int idx = node->index(c);
   if (node->children[idx] != nullptr)
   {
      node->children[idx].reset();
      node->num_children--;
   }
   return node->num_children;
}

//--------------------------------------------------------------------------------------------------
// ARTree

template<class KeyT, class ValueT>
typename ARTree<KeyT, ValueT>::value_type ARTree<KeyT, ValueT>::insert(key_type key,
                                                                       value_type&& value)
{
   auto ret = recursive_insert(_root, std::move(key), std::move(value), 0);

   if (not ret.second)
      _size++;

   return ret.first;
}

template<class KeyT, class ValueT>
typename ARTree<KeyT, ValueT>::value_type ARTree<KeyT, ValueT>::erase(const key_type& key)
{
   auto ret = recursive_erase(_root, key, 0);

   if (ret.second)
      _size--;

   return ret.first;
}

template<class KeyT, class ValueT>
typename ARTree<KeyT, ValueT>::iterator ARTree<KeyT, ValueT>::find(const key_type& key)
{
   Node* current = _root.get();

   uint32_t depth = 0;

   while (current != nullptr)
   {
      if (current->is_leaf())
      {
         auto* leaf = as_leaf<key_type, value_type>(current);

         if (leaf->match(key))
         {
            return {current};
         }
         return {};
      }

      auto& inode = current->inode;

      if (inode->prefix_len > 0)
      {
         uint32_t prefix_len = inode->mismatch(key, depth);

         if (prefix_len != std::min(inode->prefix_len, MaxPrefixLength))
         {
            return {};
         }
         depth += inode->prefix_len;
      }

      auto& next = current->find_child(key.at(depth));

      current = (next != nullptr) ? next.get() : nullptr;

      depth++;
   }

   return {};
}

template<class KeyT, class ValueT>
typename ARTree<KeyT, ValueT>::const_iterator ARTree<KeyT, ValueT>::minimum() const
{
   if (_root == nullptr)
      return {};

   auto node = _root->minimum();

   if (node == nullptr or not node->is_leaf())
      return {};

   return const_iterator{const_cast<Node*>(node)};
}

template<class KeyT, class ValueT>
typename ARTree<KeyT, ValueT>::const_iterator ARTree<KeyT, ValueT>::maximum() const
{
   if (_root == nullptr)
      return {};

   auto node = _root->maximum();

   if (node == nullptr or not node->is_leaf())
      return {};

   return const_iterator{const_cast<Node*>(node)};
}

template<class KeyT, class ValueT>
void ARTree<KeyT, ValueT>::dump(std::ostream& o) const
{
   TreeDumper<key_type, value_type> td{_root.get()};

   o << td.str();
}

template<class KeyT, class ValueT>
std::pair<typename ARTree<KeyT, ValueT>::value_type, bool>
ARTree<KeyT, ValueT>::recursive_erase(std::unique_ptr<Node>& node, const key_type& key, int depth)
{
   if (node == nullptr or key.empty())
   {
      return {value_type{}, false};
   }

   Node* current = node.get();

   if (current->is_leaf())
   {
      auto* leaf = as_leaf<key_type, value_type>(current);

      if (leaf->match(key))
      {
         node.reset();
         return {leaf->value, true};
      }
      return {value_type{}, false};
   }

   auto& inode = current->inode;

   if (inode->prefix_len > 0)
   {
      uint32_t prefix_len = inode->mismatch(key, depth);

      if (prefix_len != std::min(inode->prefix_len, MaxPrefixLength))
      {
         return {value_type{}, false};
      }
      depth += inode->prefix_len;
   }

   auto& next = current->find_child(key.at(depth));
   if (next == nullptr)
   {
      return {value_type{}, false};
   }

   if (next->is_leaf())
   {
      auto* leaf = as_leaf<key_type, value_type>(next.get());

      if (leaf->match(key))
      {
         current->delete_child(key.at(depth));
         return {leaf->value, true};
      }
      return {value_type{}, false};
   }

   return recursive_erase(next, key, depth + 1);
}

template<class KeyT, class ValueT>
std::pair<typename ARTree<KeyT, ValueT>::value_type, bool> ARTree<KeyT, ValueT>::recursive_insert(
   std::unique_ptr<Node>& node,
   key_type key,
   value_type&& value,
   int depth)
{
   // If we are at a NULL node, inject a leaf
   if (node == nullptr)
   {
      node = std::make_unique<Node>(Kind::Leaf, make_leaf(key, std::move(value)));
      return {value_type{}, false};
   }

   // If we are at a leaf, we need to replace it with a node
   if (node->is_leaf())
   {
      auto* leaf = as_leaf<key_type, value_type>(node.get());

      // Check if we are updating an existing value
      if (leaf->match(key, depth))
      {
         value_type old_value = std::move(leaf->value);
         leaf->value          = std::move(value);
         return {old_value, true};
      }

      // New value, we must split the leaf into a node4
      std::unique_ptr<Node> new_leaf =
         std::make_unique<Node>(Kind::Leaf, make_leaf(key, std::move(value)));
      auto* leaf2 = as_leaf<key_type, value_type>(new_leaf.get());

      // Determine longest prefix
      int longest_prefix = longest_common_prefix(leaf, leaf2, depth);

      std::unique_ptr<Node> new_node = std::make_unique<Node>(Kind::Node4, make_node4());
      new_node->set_prefix(key.substr(depth), longest_prefix);

      depth += longest_prefix;

      new_node->add_child(leaf->key[depth + longest_prefix], std::move(node));
      new_node->add_child(leaf2->key[depth + longest_prefix], std::move(new_leaf));

      node = std::move(new_node);

      return {value_type{}, false};
   }

   // Check if given node has a prefix
   if (node->prefix_length() > 0)
   {
      // Determine if the prefixes differ, since we need to split
      uint32_t prefix_diff = node->prefix_mismatch<key_type, value_type>(key, depth);
      if (prefix_diff >= node->prefix_length())
      {
         depth += node->prefix_length();
         goto RECURSE_SEARCH;
      }

      // Create a new node
      std::unique_ptr<Node> new_node = std::make_unique<Node>(Kind::Node4, make_node4());

      new_node->set_prefix(key, prefix_diff);

      // Adjust the prefix of the old node
      if (node->prefix_length() <= MaxPrefixLength)
      {
         auto& inode = node->inode;

         inode->prefix_len -= (prefix_diff + 1);

         new_node->add_child(inode->prefix[prefix_diff], std::move(node));

         for (int i = 0; i < std::min(MaxPrefixLength, inode->prefix_len); ++i)
         {
            inode->prefix[i] = inode->prefix[prefix_diff + i + 1];
         }
      }
      else
      {
         auto& inode = node->inode;

         inode->prefix_len -= (prefix_diff + 1);

         const auto* l = as_leaf<key_type, value_type>(node->minimum());

         new_node->add_child(l->key[depth + prefix_diff], std::move(node));

         for (int i = 0; i < std::min(MaxPrefixLength, inode->prefix_len); ++i)
         {
            inode->prefix[i] = l->key[depth + prefix_diff + i + 1];
         }
      }

      // Insert the new leaf
      std::unique_ptr<Node> new_leaf =
         std::make_unique<Node>(Kind::Leaf, make_leaf(key, std::move(value)));
      new_node->add_child(key[depth + prefix_diff], std::move(new_leaf));

      return {value_type{}, false};
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
   return {value_type{}, false};
}

//--------------------------------------------------------------------------------------------------
// NodeIterator

template<typename KeyT, typename ValueT>
class NodeIterator : public InputIterator<NodeIterator<KeyT, ValueT>, ValueT>
{
public:
   friend class IteratorOp;

   using BaseType = InputIterator<NodeIterator<KeyT, ValueT>, ValueT>;

   using key_type   = KeyT;
   using value_type = ValueT;
   using reference  = ValueT&;

   /// Default constructor
   NodeIterator() = default;

   explicit NodeIterator(Node* node)
      : _current_node(node)
   {
   }

   /// Copy 
   DEFAULT_COPY(NodeIterator)

   /// Move 
   DEFAULT_MOVE(NodeIterator)

   /// Destructor
   ~NodeIterator() = default;

   friend bool operator==(const NodeIterator& ni1, const NodeIterator& ni2)
   {
      return ni1._current_node == ni2._current_node;
   }

   friend bool operator!=(const NodeIterator& ni1, const NodeIterator& ni2)
   {
      return not(ni1 == ni2);
   }

private:
   reference value() const 
   {
      throw_if_null<Exception>(_current_node, "Null pointer", _src_loc);
      throw_if<Exception>(not _current_node->is_leaf(), "None leaf node", _src_loc);

      const auto* leaf = as_leaf<key_type, value_type>(_current_node);
      return leaf->value;
   }

   void increment()
   {
      /*
            for (;;)
            {
               Node* other_node = nullptr;
               int other_child_idx = -1;

               Node* next_node = _depth[_depth_level].node;
               int child_idx   = _depth[_depth_level].child_idx;

               switch (next_node.kind)
               {
                  case Kind::Node4:
                  {
                     Node4* n = as_node4(next_node);

                     next_child(child_idx, n->children, other_child_idx, other_node);
                     break;
                  }
                  case Node16:
                  {
                     Node16* n = as_node16(next_node);

                     next_child(child_idx, n->children, other_child_idx, other_node);
                     break;
                  }
                  case Node48:
                  {
                     Node48* n = as_node48(next_node);

                     for (int i = child_idx; i < n->keys.size(); i++)
                     {
                        int idx = n->keys[i];
                        if (idx <= 0)
                           continue;

                        Node* child = n->children[idx - 1].get();
                        if (child != nullptr)
                        {
                           other_child_idx = i;
                           other_node = child;
                           break
                        }
                     }
                     break;
                  }
                  case Node256:
                  {
                     Node256* n = as_node256(next_node);

                     next_child(child_idx, n->children, other_child_idx, other_node);
                     break;
                  }
               }

               if (other_node == nullptr)
               {
                  if (_depth_level > 0)
                  {
                     // return to previous level
                     --_depth_level;
                  }
                  else
                  {
                     _current_node = nullptr; // done!
                     return;
                  }
               }
               else
               {
               // start from the next when we come back from the child node
               _depth[_depth_level].child_idx = other_child_idx + 1;
               _current_node = other_node;

               ++_depth_level;
               _depth[_depth_level] = &iteratorLevel{otherNode, 0};
               }
            }
      */
   }
   /*
      template<std::size_t N>
      void next_child(int child_idx, std::array<std::unique_ptr<Node>, N>& children, int&
   other_child_idx, Node*&, other_node)
      {
         other_child_idx = -1;
         other_node = nullptr;

         for (int i = child_idx; i < children.size(); i++)
         {
            if (children[i] == nullptr)
               continue;

            other_child_idx = i;
            other_node = children[i].get();
            break;
         }
      }
   */
private:
   Node* _current_node = nullptr;

   // int _depth_level;
};

//--------------------------------------------------------------------------------------------------
// TreeDumper

template<typename KeyT, typename ValueT>
class TreeDumper
{
public:
   explicit TreeDumper(Node* node)
   {
      if (node != nullptr)
         base_node(node, 0, 0, 0);
   }

   const char* str() const { return _writer.c_str(); }

private:
   struct DepthPos
   {
      int child_num;
      int children_total;
   };

   void generate_padding(int depth, int child_num, int total, std::string& pad1, std::string& pad2)
   {
      _depth_array[depth] = DepthPos{child_num, total};

      for (int d = 0; d <= depth; ++d)
      {
         if (d < depth)
         {
            pad1 +=
               (_depth_array[d].child_num + 1 < _depth_array[d].children_total) ? "│   " : "    ";
            continue;
         }

         if (total == 0)
         {
            pad1 += "─";
         }
         else
         {
            pad1 += (_depth_array[d].child_num + 1 < _depth_array[d].children_total) ? "├" : "└";
         }
         pad1 += "──";
      }
      pad1 += " ";

      for (int d = 0; d <= depth; ++d)
      {
         if ((child_num + 1 < total) and (total > 0))
         {
            pad2 +=
               (_depth_array[d].child_num + 1 < _depth_array[d].children_total) ? "│   " : "    ";
            continue;
         }

         pad2 += (d < depth and _depth_array[d].child_num + 1 < _depth_array[d].children_total)
                    ? "│   "
                    : "    ";
      }
   }

   template<class T, std::size_t N>
   void write_array(const std::array<T, N>& arr)
   {
      _writer.write("[ ");

      for (auto i : arr)
      {
         if (i == 0)
         {
            _writer.write("{} ", i);
            continue;
         }
         _writer.write("{} ", static_cast<char>(i));
      }

      _writer.write("]");
   }

   template<class T, std::size_t N>
   void write_pointers(const std::array<T, N>& arr)
   {
      _writer.write("[ ");

      for (auto& i : arr)
      {
         if (i == nullptr)
         {
            _writer.write("nullptr ");
            continue;
         }
         _writer.write("{:#x} ", reinterpret_cast<uintptr_t>(i.get()));
      }

      _writer.write("]");
   }

   void base_node(Node* node, int depth, int child_num, int total)
   {
      std::string pad_header;
      std::string pad;

      generate_padding(depth, child_num, total, pad_header, pad);
      if (node == nullptr)
      {
         _writer.write(pad_header + "nullptr\n");
         return;
      }

      _writer.write(pad_header);
      _writer.write("{0} ({1:#x})\n", node->kind, reinterpret_cast<uintptr_t>(node));

      switch (node->kind)
      {
         case Kind::Node4:
         {
            Node4* n = as_node4(node);

            // Prefix
            _writer.write(pad);
            _writer.write("prefix({0}): ", n->prefix_len);
            write_array(n->prefix);
            _writer.write("\n");

            // Keys
            _writer.write(pad);
            _writer.write("keys: ");
            write_array(n->keys);
            _writer.write("\n");

            // Children
            _writer.write(pad);
            _writer.write("children({0}): ", n->num_children);
            write_pointers(n->children);
            _writer.write("\n");

            int i = 0;
            for (auto& child : n->children)
            {
               base_node(child.get(), depth + 1, i, n->children.size());
               ++i;
            }
            break;
         }
         case Kind::Node16:
         {
            Node16* n = as_node16(node);

            // Prefix
            _writer.write(pad);
            _writer.write("prefix({0}): ", n->prefix_len);
            write_array(n->prefix);
            _writer.write("\n");

            // Keys
            _writer.write(pad);
            _writer.write("keys: ");
            write_array(n->keys);
            _writer.write("\n");

            // Children
            _writer.write(pad);
            _writer.write("children({0}): \n", n->num_children);

            int i = 0;
            for (auto& child : n->children)
            {
               base_node(child.get(), depth + 1, i, n->children.size());
               ++i;
            }
            break;
         }
         case Kind::Node48:
         {
            Node48* n = as_node48(node);

            // Prefix
            _writer.write(pad);
            _writer.write("prefix({0}): ", n->prefix_len);
            write_array(n->prefix);
            _writer.write("\n");

            // Keys
            _writer.write(pad);
            _writer.write("keys: ");
            write_array(n->keys);
            _writer.write("\n");

            // Children
            _writer.write(pad);
            _writer.write("children({0}): \n", n->num_children);

            int i = 0;
            for (auto& child : n->children)
            {
               base_node(child.get(), depth + 1, i, n->children.size());
               ++i;
            }
            break;
         }
         case Kind::Node256:
         {
            Node256* n = as_node256(node);

            // Prefix
            _writer.write(pad);
            _writer.write("prefix({0}): ", n->prefix_len);
            write_array(n->prefix);
            _writer.write("\n");

            // Children
            _writer.write(pad);
            _writer.write("children({0}): \n", n->num_children);

            int i = 0;
            for (auto& child : n->children)
            {
               base_node(child.get(), depth + 1, i, n->children.size());
               ++i;
            }
            break;
         }
         case Kind::Leaf:
         {
            auto* l = as_leaf<KeyT, ValueT>(node);

            // Key
            _writer.write(pad);
            _writer.write("key: {}\n", l->key);

            // Value
            _writer.write(pad);
            _writer.write("val: {}\n", l->value);
         }
      }
      _writer.write(pad + "\n");
   }

private:
   std::array<DepthPos, 4096> _depth_array;
   fmt::MemoryWriter _writer;
};

} // namespace orion
#endif // ORION_ART_IPP
