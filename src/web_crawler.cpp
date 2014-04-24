#include <web_crawler/web_crawler.h>
#include <web_crawler/hrefs_search.h>
#include "base64.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>
#include <boost/filesystem.hpp>

#include <fstream>
#include <iostream>

#include <mutex>
#include <queue>
#include <set>
#include <cstdlib>
#include <chrono>

namespace http = boost::network::http;
namespace uri  = boost::network::uri;

namespace
{
  std::string get_filename(const uri::uri &url)
  {
    std::string path = uri::path(url);
    std::size_t index = path.find_last_of('/');
    std::string filename = path.substr(index + 1);
    return filename.empty()? "index.html" : filename;
  }
  
} // namespace


web_crawler::web_crawler(const urls_t& urls, const size_t num_of_processors /*= 2*/, const size_t num_memory_items /*= 500*/)
  : urls(urls)
  , num_memory_items_(num_memory_items)
  {
    boost::filesystem::create_directory("cache");
    
    downloader_ = std::thread(&web_crawler::job_downloader, this, std::ref(this->urls), std::ref(this->pages));
    
    for(size_t i = 0; i < num_of_processors; ++i)
      processors_.push_back(std::thread(&web_crawler::job_cache, this, i));
    
    downloader_.join();
    
    for(auto& proc : processors_)
      proc.join();
  }
  
void web_crawler::job_cache(const size_t id)
{
  while (!urls.empty())
  {
    std::unique_lock<std::mutex> lck(mutex_);
    std::cout<< id << "waiting..."<< num_memory_items_ - pages.size() <<std::endl;
    
    this->cv.wait(lck, [this]{return this->pages.size() > this->num_memory_items_;});
    
    auto item = *pages.begin();
    pages.erase(pages.begin());
    lck.unlock();
    
    const auto addr = to_base64(item.first.string());
    boost::filesystem::create_directory(authority(item.first));
    const auto& page = item.second;
    
    std::cout<< id << " saving to " << "cache/" + addr <<std::endl;
    std::ofstream ofs("cache/" + addr);
    std::copy(page.begin(), page.end(), std::ostreambuf_iterator<char>(ofs));
  }
}
  
void web_crawler::job_downloader(web_crawler::urls_t& urls, web_crawler::pages_t& pages)
{
  while (!urls.empty())
  {
    uri::uri url = urls.front();
    urls.pop();
    std::cout << url.string() <<std::endl;
    
    if (pages.find(url) != pages.end())
    {
      std::cout << "url already processed " <<std::endl;
      continue;
    } 
    
    if (!url.is_valid())
    {
      std::cout << "invalid url " <<std::endl;
      continue;
    }
    
    try
    {
      thread_local http::client client;
      http::client::response response;
      
      auto request = http::client::request(url); 
      response = client.get(request);
      auto st = status(response);
      if (st > 400)
      {
        std::cout << "error " << st <<std::endl;
        continue;
      }
      
      const auto page = body(response);    
      auto links = search_for_links(page); 
      std::remove_if(links.begin(), links.end(), [&pages](const uri::uri& link) { return pages.find(link) == pages.end();});
      
      for (const auto& link : links)
      {
        auto url_to_push = is_relative(link) ? link.string() : url.string() + link.string();
        urls.push(url_to_push);
      }
      pages[url] = page;
      
      cv.notify_all();
    }
    catch(std::exception& e)
    {
      std::cout<<"error " << e.what()<<std::endl;
      std::cout <<urls.size() << " " <<pages.size() <<std::endl;
      urls.push(url);
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
      continue;
    }
    
  }
}