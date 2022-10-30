// Copyright 2019 Luca Istrate, Danut Matei
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <utility>
#include <cassert>
#include "./aegraph.h"
std::ofstream fout("testing1.out");
std::string strip(std::string s) {
  // deletes whitespace from the beginning and end of the string
  s.erase(0, s.find_first_not_of(" \n\r\t"));
  s.erase(s.find_last_not_of(" \n\r\t") + 1);
  return s;
}

std::pair<std::string, std::string> split_first(std::string s,
                                                char delimiter = ',') {
  // returns a pair that contains: <first_cut, rest_of_graph>

  int numOpen = 0;

  int len = s.size();
  for (int i = 0; i < len; i++) {
    char c = s[i];
    if (c == delimiter && numOpen == 0)
      return std::make_pair(strip(s.substr(0, i)), strip(s.substr(i + 1, len)));
    if (c == '[') numOpen += 1;
    if (c == ']') numOpen -= 1;
  }

  return {strip(s), std::string()};
}

std::vector<std::string> split_level(std::string s, char delimiter = ',') {
  // splits 's' into separate entities (atoms, subgraphs)

  std::vector<std::string> result;
  auto snd = s;
  while (true) {
    auto p = split_first(snd, delimiter);
    auto fst = p.first;
    snd = p.second;

    result.push_back(fst);

    if (snd.empty()) return result;
  }
}

int AEGraph::num_subgraphs() const { return subgraphs.size(); }

int AEGraph::num_atoms() const { return atoms.size(); }

int AEGraph::size() const { return num_atoms() + num_subgraphs(); }

bool AEGraph::operator<(const AEGraph& other) const {
  return this->repr() < other.repr();
}

bool AEGraph::operator==(const AEGraph& other) const {
  return this->repr() == other.repr();
}

bool AEGraph::operator!=(const AEGraph& other) const {
  return this->repr() != other.repr();
}

AEGraph AEGraph::operator[](const int index) const {
  // offers an easier way of accessing the nested graphs
  if (index < num_subgraphs()) {
    return subgraphs[index];
  }

  if (index < num_subgraphs() + num_atoms()) {
    return AEGraph('(' + atoms[index - num_subgraphs()] + ')');
  }

  return AEGraph("()");
}

std::ostream& operator<<(std::ostream& out, const AEGraph& g) {
  out << g.repr();
  return out;
}

AEGraph::AEGraph(std::string representation) {
  // constructor that creates an AEGraph structure from a
  // serialized representation
  char left_sep = representation[0];
  char right_sep = representation[representation.size() - 1];

  assert((left_sep == '(' && right_sep == ')') ||
         (left_sep == '[' && right_sep == ']'));

  // if the left separator is '(' then the AEGraph is the entire
  // sheet of assertion
  if (left_sep == '(') {
    is_SA = true;
  } else {
    is_SA = false;
  }

  // eliminate the first pair of [] or ()
  representation = representation.substr(1, representation.size() - 2);

  // split the graph into separate elements
  auto v = split_level(representation);
  // add them to the corresponding vector
  for (auto s : v) {
    if (s[0] != '[') {
      atoms.push_back(s);
    } else {
      subgraphs.push_back(AEGraph(s));
    }
  }

  // also internally sort the new graph
  this->sort();
}

std::string AEGraph::repr() const {
  // returns the serialized representation of the AEGraph

  std::string left, right;
  if (is_SA) {
    left = '(';
    right = ')';
  } else {
    left = '[';
    right = ']';
  }

  std::string result;
  for (auto subgraph : subgraphs) {
    result += subgraph.repr() + ", ";
  }

  int len = atoms.size();
  if (len != 0) {
    for (int i = 0; i < len - 1; i++) {
      result += atoms[i] + ", ";
    }
    result += atoms[len - 1];
  } else {
    if (subgraphs.size() != 0)
      return left + result.substr(0, result.size() - 2) + right;
  }

  return left + result + right;
}

void AEGraph::sort() {
  std::sort(atoms.begin(), atoms.end());

  for (auto& sg : subgraphs) {
    sg.sort();
  }

  std::sort(subgraphs.begin(), subgraphs.end());
}

bool AEGraph::contains(const std::string other) const {
  // checks if an atom is in a graph
  if (find(atoms.begin(), atoms.end(), other) != atoms.end()) return true;

  for (const auto& sg : subgraphs)
    if (sg.contains(other)) return true;

  return false;
}

bool AEGraph::contains(const AEGraph& other) const {
  // checks if a subgraph is in a graph
  if (find(subgraphs.begin(), subgraphs.end(), other) != subgraphs.end())
    return true;

  for (const auto& sg : subgraphs)
    if (sg.contains(other)) return true;

  return false;
}

std::vector<std::vector<int> > AEGraph::get_paths_to(
    const std::string other) const {
  // returns all paths in the tree that lead to an atom like <other>
  std::vector<std::vector<int> > paths;

  int len_atoms = num_atoms();
  int len_subgraphs = num_subgraphs();

  for (int i = 0; i < len_atoms; i++) {
    if (atoms[i] == other && size() > 1) {
      paths.push_back({i + len_subgraphs});
    }
  }

  for (int i = 0; i < len_subgraphs; i++) {
    if (subgraphs[i].contains(other)) {
      auto r = subgraphs[i].get_paths_to(other);
      for (auto& v : r) v.insert(v.begin(), i);
      copy(r.begin(), r.end(), back_inserter(paths));
    }
  }

  return paths;
}

std::vector<std::vector<int> > AEGraph::get_paths_to(
    const AEGraph& other) const {
  // returns all paths in the tree that lead to a subgraph like <other>
  std::vector<std::vector<int> > paths;
  int len_subgraphs = num_subgraphs();

  for (int i = 0; i < len_subgraphs; i++) {
    if (subgraphs[i] == other && size() > 1) {
      paths.push_back({i});
    } else {
      auto r = subgraphs[i].get_paths_to(other);
      for (auto& v : r) v.insert(v.begin(), i);
      copy(r.begin(), r.end(), back_inserter(paths));
    }
  }

  return paths;
}

std::vector<std::vector<int> > Possible_Double_Cuts(AEGraph other,
                                                    std::vector<int> position,
                                                    int cuts) {
  std::vector<std::vector<int> > all_positions, aux;
  std::vector<int> next_pos, last_pos;
  if (other.num_subgraphs() == 0) {
    if (cuts == 2) {
      last_pos = position;
      last_pos.pop_back();
      all_positions.push_back(last_pos);
    }
  } else if (other.num_subgraphs() == 1) {
    if (other.num_atoms() == 0) {
      if (cuts == 2) {
        last_pos = position;
        last_pos.pop_back();
        all_positions.push_back(last_pos);
        cuts--;
      }
      next_pos = position;
      next_pos.push_back(0);
      aux = Possible_Double_Cuts(other.subgraphs[0], next_pos, cuts + 1);
      for (auto it = aux.begin(); it != aux.end(); ++it)
        all_positions.push_back(*it);
    } else {
      cuts = 0;
      next_pos = position;
      next_pos.push_back(0);
      aux = Possible_Double_Cuts(other.subgraphs[0], next_pos, cuts + 1);
      for (auto it = aux.begin(); it != aux.end(); ++it)
        all_positions.push_back(*it);
    }
  } else if (other.subgraphs.size() > 1) {
    cuts = 0;
    for (int i = 0; i < other.num_subgraphs(); ++i) {
      next_pos = position;
      next_pos.push_back(i);
      aux = Possible_Double_Cuts(other.subgraphs[i], next_pos, cuts + 1);
      for (auto it = aux.begin(); it != aux.end(); ++it) {
        all_positions.push_back(*it);
      }
    }
  }
  return all_positions;
}

std::vector<std::vector<int> > AEGraph::possible_double_cuts() const {
  // 10p
  return Possible_Double_Cuts(*this, std::vector<int>(), 0);
}

AEGraph Double_cut(AEGraph& it, std::vector<int> where) {
  // 10p
  if (where.size() == 1) {
    for (int i = 0; i < it.subgraphs[where[0]].subgraphs[0].num_atoms(); ++i) {
      it.atoms.push_back(it.subgraphs[where[0]].subgraphs[0].atoms[i]);
    }
    for (int i = 0; i < it.subgraphs[where[0]].subgraphs[0].num_subgraphs();
         ++i) {
      it.subgraphs.push_back(it.subgraphs[where[0]].subgraphs[0].subgraphs[i]);
    }
    it.subgraphs.erase(it.subgraphs.begin() + where[0]);
  } else {
    int aux = where[0];
    where.erase(where.begin());
    Double_cut(it.subgraphs[aux], where);
  }
  return it;
}

AEGraph AEGraph::double_cut(std::vector<int> where) const {
  // 10p
  AEGraph copy = *this;
  Double_cut(copy, where);
  return copy;
}

std::vector<std::vector<int> > Possible_Erasures(AEGraph other,
                                                 std::vector<int> position,
                                                 int level) {
  std::vector<std::vector<int> > all_positions, aux;
  std::vector<int> pos;
  if (level == -1) {
    for (int i = 0; i < other.num_atoms(); i++) {
      pos = position;
      pos.push_back(i);
      all_positions.push_back(pos);
    }
    for (int i = 0; i < other.num_subgraphs(); i++) {
      pos = position;
      pos.push_back(i + other.num_atoms());
      aux = Possible_Erasures(other.subgraphs[i], pos, level + 1);
      for (auto it = aux.begin(); it != aux.end(); it++) {
        all_positions.push_back(*it);
      }
    }
  } else if (level % 2 == 0) {
    all_positions.push_back(position);
    for (int i = 0; i < other.num_subgraphs(); i++) {
      pos = position;
      pos.push_back(i + other.num_atoms());
      aux = Possible_Erasures(other.subgraphs[i], pos, level + 1);
      for (auto it = aux.begin(); it != aux.end(); it++) {
        all_positions.push_back(*it);
      }
    }
  } else {
    if (other.num_atoms() + other.num_subgraphs() > 1) {
      for (int i = 0; i < other.num_atoms(); i++) {
        pos = position;
        pos.push_back(i);
        all_positions.push_back(pos);
      }
      for (int i = 0; i < other.num_subgraphs(); i++) {
        pos = position;
        pos.push_back(i + other.num_atoms());
        aux = Possible_Erasures(other.subgraphs[i], pos, level + 1);
        for (auto it = aux.begin(); it != aux.end(); it++) {
          all_positions.push_back(*it);
        }
      }
    }
  }
  return all_positions;
}

std::vector<std::vector<int> > AEGraph::possible_erasures(int level) const {
  // 10p
  return Possible_Erasures(*this, std::vector<int>(), -1);
}

AEGraph Erase(AEGraph& it, std::vector<int> where) {
  if (where.size() == 1) {
    if (where[0] < it.num_subgraphs()) {
      it.subgraphs.erase(it.subgraphs.begin() + where[0]);
    } else {
      it.atoms.erase(it.atoms.begin() + where[0] - it.num_subgraphs());
    }
  } else {
    int aux = where[0];
    where.erase(where.begin());
    Erase(it.subgraphs[aux], where);
  }
  return it;
}

AEGraph AEGraph::erase(std::vector<int> where) const {
  // 10p
  AEGraph copy = *this;
  Erase(copy, where);
  return copy;
}

std::vector<std::vector<int> > subgraph_appearances(AEGraph parent,
                                                    AEGraph subgraph,
                                                    std::vector<int> position) {
  std::vector<std::vector<int> > all_positions, aux;
  std::vector<int> pos;
  // we search only for positions where we can apply deiteration
  // so the graph has to have more than one child
  if (parent.num_subgraphs() + parent.num_atoms() > 1) {
    for (int i = 0; i < parent.num_subgraphs(); ++i) {
      pos = position;
      pos.push_back(i);
      if (parent.subgraphs[i] == subgraph) {
        all_positions.push_back(pos);
      } else {
        aux = subgraph_appearances(parent.subgraphs[i], subgraph, pos);
        for (auto it = aux.begin(); it != aux.end(); ++it) {
          all_positions.push_back(*it);
        }
      }
    }
  } else if (parent.num_subgraphs() == 1) {
    pos = position;
    pos.push_back(0);
    aux = subgraph_appearances(parent.subgraphs[0], subgraph, pos);
    for (auto it = aux.begin(); it != aux.end(); ++it) {
      all_positions.push_back(*it);
    }
  }
  return all_positions;
}

std::vector<std::vector<int> > atom_appearances(AEGraph parent,
                                                std::string atom,
                                                std::vector<int> position) {
  std::vector<std::vector<int> > all_positions, aux;
  std::vector<int> pos;
  // we search only for positions where we can apply deiteration
  // so the graph has to have more than one child
  if (parent.num_subgraphs() + parent.num_atoms() > 1){
    for (int i = 0; i < parent.num_atoms(); ++i) {
      if (parent.atoms[i] == atom) {
        pos = position;
        pos.push_back(parent.num_subgraphs() + i);
        all_positions.push_back(pos);
      }
    }
  }
  for (int i = 0; i < parent.num_subgraphs(); ++i) {
    pos = position;
    pos.push_back(i);
    aux = atom_appearances(parent.subgraphs[i], atom, pos);
    for (auto it = aux.begin(); it != aux.end(); ++it) {
      all_positions.push_back(*it);
    }
  }
  return all_positions;
}

std::vector<std::vector<int> > AEGraph::possible_deiterations() const {
  // 20p
  std::vector<std::vector<int> > all_positions, aux;
  std::vector<int> pos;
  for (int i = 0; i < num_subgraphs(); ++i) {
    pos.clear();
    pos.push_back(i);
    aux = subgraph_appearances(*this, subgraphs[i], std::vector<int>());
    // the current position doesn't count
    aux.erase(std::remove(aux.begin(), aux.end(), pos), aux.end());
    for (auto it = aux.begin(); it != aux.end(); ++it) {
      all_positions.push_back(*it);
    }
  }
  for (int i = 0; i < num_atoms(); ++i) {
    pos.clear();
    pos.push_back(i + num_subgraphs());
    aux = atom_appearances(*this, atoms[i], std::vector<int>());
    // the current position doesn't count
    aux.erase(std::remove(aux.begin(), aux.end(), pos), aux.end());
    for (auto it = aux.begin(); it != aux.end(); ++it) {
      all_positions.push_back(*it);
    }
  }
  return all_positions;
}

AEGraph AEGraph::deiterate(std::vector<int> where) const {
  // 10p
  AEGraph copy = *this;
  Erase(copy, where);
  return copy;
}
