#include "XmlDom.h"

#include <cstdio>
#include <cstring>

#include <expat.h>

namespace tgxml {

const std::string* Node::attribute(const char* attr_name) const {
  for ( const auto& a : attributes ) {
    if ( a.first == attr_name )
      return &a.second;
  }
  return nullptr;
}

namespace {

// The parser is created for ISO-8859-1, so every input byte maps to one code point and
// expat hands back UTF-8; converting back to Latin-1 restores the file's original bytes.
std::string utf8_to_latin1(const XML_Char* s) {
  std::string out;
  for ( const unsigned char* p = reinterpret_cast<const unsigned char*>( s ); *p; ++p ) {
    if ( *p < 0x80 ) {
      out += static_cast<char>( *p );
    } else if ( ( *p & 0xE0 ) == 0xC0 && p[1] ) {
      out += static_cast<char>( ( ( *p & 0x1F ) << 6 ) | ( p[1] & 0x3F ) );
      ++p;
    }
  }
  return out;
}

struct BuildState {
  std::unique_ptr<Node> root;
  std::vector<Node*>    stack;
};

void XMLCALL on_start(void* user, const XML_Char* name, const XML_Char** atts) {
  auto* st   = static_cast<BuildState*>( user );
  auto  node = std::make_unique<Node>();
  node->name = utf8_to_latin1( name );
  for ( int i = 0; atts[i] && atts[i + 1]; i += 2 )
    node->attributes.emplace_back( utf8_to_latin1( atts[i] ), utf8_to_latin1( atts[i + 1] ) );

  Node* raw = node.get();
  if ( st->stack.empty() ) {
    if ( st->root )
      return; // a second root element is not well-formed; expat reports it anyway
    st->root = std::move( node );
  } else {
    st->stack.back()->children.push_back( std::move( node ) );
  }
  st->stack.push_back( raw );
}

void XMLCALL on_end(void* user, const XML_Char*) {
  auto* st = static_cast<BuildState*>( user );
  if ( !st->stack.empty() )
    st->stack.pop_back();
}

} // namespace

std::unique_ptr<Node> load_file(const char* path) {
  FILE* fp = std::fopen( path, "rb" );
  if ( !fp )
    return nullptr;

  BuildState st;
  XML_Parser parser = XML_ParserCreate( "ISO-8859-1" );
  if ( !parser ) {
    std::fclose( fp );
    return nullptr;
  }
  XML_SetUserData( parser, &st );
  XML_SetElementHandler( parser, on_start, on_end );

  bool ok = true;
  char buf[8192];
  for ( ;; ) {
    size_t len  = std::fread( buf, 1, sizeof( buf ), fp );
    int    done = len < sizeof( buf );
    if ( XML_Parse( parser, buf, static_cast<int>( len ), done ) == XML_STATUS_ERROR ) {
      ok = false;
      break;
    }
    if ( done )
      break;
  }

  XML_ParserFree( parser );
  std::fclose( fp );
  return ok ? std::move( st.root ) : nullptr;
}

} // namespace tgxml
