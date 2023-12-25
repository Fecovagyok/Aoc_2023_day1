#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <istream>
#include <unordered_set>
#include <vector>

#include "dfs_helper.h"
#include "link.h"

struct NodeProps {
  Node node;
  Nodes adjacents;

  // For DFS
  bool inDFS = false;
  uint64_t discovery = -1;
  uint64_t lowest = -1;

  NodeProps() { adjacents.reserve(20); }
  NodeProps(Node node) : node(node) { adjacents.reserve(20); }

  void resetDFS() {
    inDFS = false;
    discovery = -1;
    lowest = -1;
  }

  void buildTarjanDFS(DFSHelper& graph, Node prev) {
    graph.incTime();
    inDFS = true;
    lowest = discovery = graph.getTime();

    for (size_t i = 0; i < adjacents.size(); i++) {
      if (prev == adjacents[i]) continue;
      NodeProps& next_node = graph.getNodeProps(adjacents[i]);
      if (next_node.inDFS) {
        lowest = std::min(lowest, next_node.discovery);
      } else {
        next_node.buildTarjanDFS(graph, node);
        lowest = std::min(lowest, next_node.discovery);
        LinkProps& link_to_next = graph.getLinkProps(Link{node, adjacents[i]});

        if (next_node.lowest > discovery) {
          graph.addLinkProps(link_to_next);
        }
      }
    }
  }
  bool operator<(const NodeProps& other) const { return node < other.node; }
  bool operator>(const NodeProps& other) const { return node > other.node; }
  bool operator==(const NodeProps& other) const { return node == other.node; }
};
namespace std {
template <>
struct hash<NodeProps> {
  size_t operator()(const NodeProps& n) const { return hash<Node>{}(n.node); }
};
}  // namespace std

class Graph : public DFSHelper {
  NodePropsCont nodes;
  LinkMap linkMap;

  uint64_t time = 0;

 public:
  void readLinkMap(std::istream& is, Node key) {
    Node node;
    NodeProps& local_nodes = nodes.emplace(key, NodeProps{key}).first->second;

    while (true) {
      is.ignore(3, ' ');
      if (node.read(is)) {
        break;
      }
      // Inserting neighbours
      local_nodes.adjacents.push_back(node);
      {
        Link link{key, node};
        if (linkMap.count(link) == 0)
          linkMap.emplace(link, LinkProps{link});
        else
          std::cout << "DOes this ever happend?";
      }

      // Inserting node itself
      if (nodes.count(node) == 0) {
        NodeProps& inserted =
            nodes.emplace(node, NodeProps{node}).first->second;
        inserted.adjacents.push_back(key);
      } else {
        nodes[node].adjacents.push_back(key);
      }
    }
  }

  std::vector<LinkProps*> winners;

  void resetDFSAll() {
    for (auto& node : nodes) {
      node.second.resetDFS();
    }
  }

  NodeProps& getNodeProps(Node node) override { return nodes[node]; }
  LinkProps& getLinkProps(Link link) override {
    try {
      return linkMap.at(link);
    } catch (std::exception& e) {
      std::cout << "NAgay a baj"
                << "\n"
                << std::endl;
      throw;
    }
  }
  uint64_t getTime() override { return time; }
  void incTime() override { time++; }
  void addLinkProps(LinkProps& link) override { winners.push_back(&link); }

  void selfTest() {
    if (nodes.size() != 15) {
      std::cout << "Nem 15" << std::endl;
      exit(1);
    }
    std::unordered_set<Node> set;
    for (const auto& node : nodes) {
      set.clear();
      const Nodes& node_ids = node.second.adjacents;
      for (size_t i = 0; i < node_ids.size(); i++) {
        if (set.count(node_ids[i]) == 1) {
          std::cout << "Baj"
                    << "\n";
          exit(1);
        }
        set.insert(node_ids[i]);
      }
    }
  }

  void trySolution(NodeProps& node) {
    std::unordered_set<NodeProps*> first;
    std::unordered_set<NodeProps*> second;
    for (size_t i = 0; i < winners.size(); i++) {
      NodeProps& propsA = nodes[winners[i]->link.getA()];
      NodeProps& propsB = nodes[winners[i]->link.getB()];

      resetDFSAll();

      propsA.buildDFS(*this, first);

      resetDFSAll();

      propsB.buildDFS(*this, second);

      if (first.size() + second.size() == nodes.size()) {
        std::cout << "salkfnsdlkfjaslkfdjalksjfdlkasjflkasjflkasjal"
                  << std::endl;
      } else {
        std::cout << "\nEredmenyek: " << first.size() << " + " << second.size();
      }
    }
  }

  void dfs() {
    for (auto& node_it : nodes) {
      NodeProps& nodeProps = node_it.second;
      nodeProps.buildTarjanDFS(*this, nodeProps.node);
      if (winners.size() == 3) {
        std::cout << "Baszo" << std::endl;
        trySolution(nodeProps);
        for (size_t i = 0; i < winners.size(); i++) {
          std::cout << winners[i]->name << "\n";
        }
      }
      node_it.second.resetDFS();
      time = 0;
      winners.clear();
    }
  }
};
