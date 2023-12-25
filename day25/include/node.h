#include <cstdint>
#include <istream>
#include <unordered_set>
#include <vector>

class Node {
  uint32_t id;

 public:
  int read(std::istream& is) {
    uint32_t local_id = 0;
    for (uint32_t i = 0; i < 3; i++) {
      char c = is.get();
      if (!is.good() || c == '\0') {
        return -1;
      }
      local_id |= (c << (i * 8));
    }
    id = local_id;
    return 0;
  }
  uint32_t getId() const { return id; }

  std::string toString() const {
    std::string string_id(3, 0);
    for (uint32_t i = 0; i < 3; i++) {
      string_id[i] = (id >> i * 8) & 0xff;
    }
    return string_id;
  }

  bool operator<(Node other) const { return this->id < other.id; }
  bool operator>(Node other) const { return this->id > other.id; }
  bool operator==(Node other) const { return id == other.id; }
};

typedef std::vector<Node> Nodes;
struct NodeProps {
  Node node;
  Nodes adjacents;
  bool searchProp1 = false;

  NodeProps() { adjacents.reserve(20); }
  NodeProps(Node node) : node(node) { adjacents.reserve(20); }

  bool operator<(const NodeProps& other) const { return node < other.node; }
  bool operator>(const NodeProps& other) const { return node > other.node; }
  bool operator==(const NodeProps& other) const { return node == other.node; }
};

namespace std {
template <>
struct hash<Node> {
  size_t operator()(Node n) const { return hash<uint32_t>{}(n.getId()); }
};

template <>
struct hash<NodeProps> {
  size_t operator()(const NodeProps& n) const { return hash<Node>{}(n.node); }
};
}  // namespace std

struct NodeComp {
  bool operator()(Node first, Node other) { return first < other; }
};

using NodePropsCont = std::unordered_set<NodeProps>;
