#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <sstream>
#include <string>



std::string to_base64(const std::string& s) 
{
  const std::string base64_padding[] = {"", "==","="};

  using namespace boost::archive::iterators;

  std::stringstream os;

  typedef 
          base64_from_binary<    // convert binary values ot base64 characters
              transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
                  const char *,
                  6,
                  8
              >
          > 
      base64_enc; // compose all the above operations in to a new iterator


  std::copy(base64_enc(s.c_str()), base64_enc(s.c_str() + s.size()),
    ostream_iterator<char>(os));

  os << base64_padding[s.size() % 3];
  return os.str();
}

std::string from_base64(const std::string& s) 
{
  using namespace boost::archive::iterators;

  std::stringstream os;

  // convert binary values to base64 characters
  typedef                                                   // retrieve 6 bit integers from a sequence of 8 bit bytes 
    transform_width <
      binary_from_base64<const char *>                      // compose all the above operations in to a new iterator
        , 8 
    , 6> base64_dec;

  unsigned int size = s.size();
  if(size <= 0) return "";

  for (int i = 1; i <= 2; i++) {
    if (s.c_str()[size - 1] == '=')
    size--;
    }

  std::copy(base64_dec(s.c_str()), base64_dec(s.c_str() + size),
    ostream_iterator<char>(os));

  return os.str();
}

// inline std::string to_base64(const std::string& str)
// {
//     using namespace boost::archive::iterators;
// 
//     std::stringstream os;
//     typedef 
//             base64_from_binary<    // convert binary values ot base64 characters
//                 transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
//                     const char *,
//                     6,
//                     8
//                 >
//             > 
//         base64_text; // compose all the above operations in to a new iterator
// 
//     std::copy(
//         base64_text(str.c_str()),
//         base64_text(str.c_str() + str.size()),
//         ostream_iterator<char>(os)
//     );
//     
//     return os.str();
// }