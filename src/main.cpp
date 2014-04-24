#include <web_crawler/web_crawler.h>
#include <iostream>

int main(int argc, char *argv[])
{
  web_crawler::urls_t urls;
  urls.push(boost::network::uri::uri("http://www.cplusplus.com/reference/queue/queue/pop/"));
  try
  {
  web_crawler app(urls, 2);
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}