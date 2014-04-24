#include <web_crawler/hrefs_search.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <gumbo.h>

namespace 
{
  
void search_for_links(GumboNode* node, links_t& links) 
{
  if (node->type != GUMBO_NODE_ELEMENT) 
    return;
  
  GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) 
    links.push_back(std::string(href->value));

  GumboVector* children = &node->v.element.children;
  for (int i = 0; i < children->length; ++i) 
    search_for_links(static_cast<GumboNode*>(children->data[i]), links);
}

}

links_t search_for_links(const std::string& contents) 
{

  links_t res;
  GumboOutput* output = gumbo_parse(contents.c_str());
  search_for_links(output->root, res);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  return res;
}
