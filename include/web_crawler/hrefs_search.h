
#pragma once

#include <string>
#include <vector>
#include <boost/network/uri.hpp>

typedef std::vector<boost::network::uri::uri> links_t;
// typedef std::vector<std::string> links_t;
links_t search_for_links(const std::string& contents);