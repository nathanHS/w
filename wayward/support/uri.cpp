#include "wayward/support/uri.hpp"
#include <regex>
#include <wayward/support/format.hpp>

namespace wayward {
  Maybe<URI> URI::parse(const std::string& input) {
    /*
      Groups:
      (scheme) :// ( (username) (:(password))? @)? (hostname) (:(port))? (path)? (\?(query))? (#(fragment))?
      1 = scheme
      3 = username
      5 = password
      6 = hostname
      8 = port
      9 = path
      10 = query
      12 = fragment
    */
    static const std::regex uri_matcher { "^([\\w]+)\\://(([^:@]+)(\\:([^@]+))?@)?([^/:?#]+)(\\:(\\d+))?([^?#]+)?(\\?([^#]*))?(#(.*))?$" };
    MatchResults results;
    if (std::regex_match(input, results, uri_matcher)) {
      URI uri;
      uri.scheme = results[1];
      uri.username = results[3];
      uri.password = results[5];
      uri.host = results[6];
      if (results.length(8)) {
        std::stringstream ss { results[8] };
        ss >> uri.port;
      } else {
        uri.port = -1;
      }
      uri.path = results[9];
      uri.query = results[10];
      uri.fragment = results[12];
      return uri;
    } else {
      return Nothing;
    }
  }

  std::string URI::to_string() const {
    std::stringstream ss;
    ss << scheme << "://";
    if (username.size()) {
      ss << username;
      if (password.size()) {
        ss << ':' << password;
      }
      ss << '@';
    }
    ss << host << path << query << fragment;
    return ss.str();
  }
}
