#include <map>
#include <queue>
#include <string>
#include <thread>
#include <condition_variable>
#include <boost/network/uri.hpp>

class web_crawler
{
public:
  typedef std::string page_t;
  typedef std::map<boost::network::uri::uri, page_t> pages_t;
  typedef std::function<void(const page_t&)> processor_t;
  typedef std::queue<boost::network::uri::uri> urls_t;
  
  /**
   * @brief 
   * @param urls - urls to start crawling with
   * @param num_of_processors - number of page processing threads
   * @param num_memory_items - number of pages to wait for before start processing
   */
  web_crawler(const urls_t& urls, const size_t num_of_processors = 2, const size_t num_memory_items = 500);
  
private:
  const size_t num_memory_items_;
  
  void job_downloader(urls_t& urls, pages_t& pages);
  void job_cache(const size_t id);
  
  std::thread downloader_;
  
  std::vector<std::thread> processors_;
  
  urls_t urls;
  
  pages_t pages;
  
  std::mutex mutex_;
  std::condition_variable cv;
};
