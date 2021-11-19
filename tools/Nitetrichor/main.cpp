/*!
* Created by Molybdenum on 8/24/21.
*
*/

#include "rapidxml/rapidxml_ext.hpp"
#include <iterator>
#include <string>

namespace rx = rapidxml;

std::string build_simple_doc_by_rapidxml() {
    rx::xml_document<> doc;
    rx::xml_node<>* decl = doc.allocate_node(rx::node_declaration);
    rx::xml_attribute<>* decl_ver =
            doc.allocate_attribute("version", "1.0");
    decl->append_attribute(decl_ver);
    doc.append_node(decl);

    rx::xml_node<>* node =
            doc.allocate_node(rx::node_element,    "Hello", "World");
    doc.append_node(node);

    std::string text;
    rapidxml::print(std::back_inserter(text), doc, 0);
    printf("%s", text.c_str());

    return text;
}

void parse_doc_by_rapidxml(char* xml_doc) {
    rx::xml_document<> doc;        // character type defaults to char
    doc.parse<0>(xml_doc);  // 0 means default parse flags

    rx::xml_node<> *node = doc.first_node("Hello");
    std::string node_val = node->value();

    printf("%s", node_val.c_str());
}

int main(int argc, const char * argv[]) {
    // main entry here
    std::string doc = build_simple_doc_by_rapidxml();
    parse_doc_by_rapidxml(&doc[0]);
    return 0;
}

