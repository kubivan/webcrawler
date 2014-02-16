#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <rapidxml.hpp>

namespace http = boost::network::http;
namespace uri = boost::network::uri;

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


struct app_t
{
  std::vector<std::thread> workers_;
  std::queue<uri::uri> urls;
  std::mutex mutex_;
};

app_t app;



std::string get_page(const uri::uri& url)
{
    http::client client;
    http::client::request request(url);
    http::client::response response = client.get(request);

    return static_cast<std::string>(body(response));
}

void do_job(size_t thread_id)
try{
  while (!app.urls.empty())
  {
    app.mutex_.lock();
    uri::uri url = app.urls.front();
    app.urls.pop();
    app.mutex_.unlock();

    if (!url.is_valid())
      return;

    std::vector<std::string> refs;

    using namespace rapidxml;
    auto page = get_page(url);
	page.push_back('/0');
    xml_document<> doc;
    doc.parse<0>(const_cast<char*>(page.data()));
    xml_node<> *title_node = doc.first_node("head")->first_node("title");
    std::string filename = title_node->value();

    std::cout<<thread_id<<" : "<<url.string()<<" : "<<filename<<std::endl;
    std::ofstream ofs(filename.c_str());
    ofs << page << std::endl;


    xml_node<> *body_node = doc.first_node("body");
  }

}catch(std::exception& e)
{
	std::cout<<e.what()<<std::endl;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " url" << std::endl;
    return 1;
  }

  app.urls.push(uri::uri("http://www.cplusplus.com/reference/queue/queue/pop/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/list/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/stack/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/2hA0RXSz/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/queue/queue/pop/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/list/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/stack/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/2hA0RXSz/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/queue/queue/pop/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/list/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/stack/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/2hA0RXSz/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/list/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/stack/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/2hA0RXSz/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/queue/queue/pop/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/list/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/stack/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/2hA0RXSz/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/queue/queue/pop/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/list/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/stack/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/2hA0RXSz/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/list/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/reference/stack/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/"));
  app.urls.push(uri::uri("http://www.cplusplus.com/articles/2hA0RXSz/"));
  try
  {
    for (size_t i = 0; i< 4; ++i)
      app.workers_.push_back(std::thread(do_job,i));

    for (auto& w : app.workers_)
      w.join();

  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
//]
