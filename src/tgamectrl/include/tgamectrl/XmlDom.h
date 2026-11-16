#ifndef _TGAMECTRL_XMLDOM_H_
#define _TGAMECTRL_XMLDOM_H_

// Minimal read-only XML DOM used by CTControlMgr to load the UI layout files
// (3DData/Control/Xml/*.xml). Replaces MSXML (#import "msxml.tlb"), which only exists on
// Windows/MSVC. Backed by the in-tree expat.
//
// Only element nodes are kept as children: the loaders walk child elements by name, so
// text, comment and processing-instruction nodes (which MSXML's childNodes also returned)
// never matched anything. Attribute values are the file's raw bytes, which is what MSXML
// produced for these ASCII files after its BSTR -> ANSI conversion.

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace tgxml {

struct Node;
using NodeList = std::vector<std::unique_ptr<Node>>;

struct Node {
  std::string                                      name;
  std::vector<std::pair<std::string, std::string>> attributes;
  NodeList                                         children; // element children, in document order

  // Value of attribute `attr_name`, or nullptr if the element has no such attribute.
  const std::string* attribute(const char* attr_name) const;
};

// Parses `path` and returns its root element, or nullptr if the file can't be read or parsed.
std::unique_ptr<Node> load_file(const char* path);

} // namespace tgxml

#endif // _TGAMECTRL_XMLDOM_H_
