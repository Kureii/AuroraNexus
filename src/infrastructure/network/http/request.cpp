/* This file was created by Kureii (Tomas Adamek)
* Date created: 28. 09. 2023
* This file is under the MIT license
*/
//================================= Includes ===================================
#include "aurora-nexus/infrastructure/network/http/request.hpp"

#include <cstdint>
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string/erase.hpp>


//================================= Namespace ==================================
namespace aurora_nexus {

//================================= Public method ==============================

Request::Request(const std::string &http_text) : query_(nullptr), host_(""), body_(""), body_type_(""), path_(""){
  auto lines = SplitRequestText(http_text, "\r\n");

  auto first_line = SplitRequestText(std::string(lines[0]), " ");

  method_ = first_line[0];

  ParseURLPath(first_line[1]);

  FindHeaders(lines);
}

const std::string &Request::GetMethod() const { return method_; }

const std::string &Request::GetPath() const { return path_; }

const std::map<std::string, std::string> &Request::GetQuery() const {
  return *query_;
}

const std::map<std::string, std::string> &Request::GetHeaders() const {
  return headers_;
}

const std::string &Request::GetBody() const { return body_; }

const std::string &Request::GetHost() const { return host_; }

const std::string &Request::GetBodyType() const { return body_type_; }

//================================= Testing method =============================

#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================
std::vector<std::string> Request::SplitRequestText(const std::string &to_split, const std::string &separator) {
  uint64_t const separator_size = separator.size();
  std::vector<std::string> result;
  std::string_view view = to_split;
  size_t find = view.find(separator);
  while (find != std::string::npos) {
    result.emplace_back(view.substr(0, find));
    view.remove_prefix(find + separator_size);
    find = view.find(separator);
  }
  if (!view.empty()) {
    result.emplace_back(view);
  }
  return result;
}

void Request::ParseURLPath(const std::string &URL) {
  auto find = URL.find('?');

  std::string_view URL_view = URL;
  std::string_view path_view;
  std::string_view pre_query_view;

  if (find == std::string::npos) {
    path_ = URL;
    return;
  }
  query_ = std::make_unique<std::map<std::string, std::string>>();

  path_view = URL_view.substr(0, find);
  pre_query_view = URL_view.substr(find + 1);

  std::string pre_query(pre_query_view);
  std::replace(pre_query.begin(), pre_query.end(), '?', '&');
  std::replace(pre_query.begin(), pre_query.end(), '=', '&');

  auto query = SplitRequestText(pre_query, "&");
  auto query_size = query.size();

  if (query_size % 2 != 0) {
    --query_size;
  }

  for (uint64_t i = 0; i < query_size; i += 2) {
    query_->emplace(query[i], query[i + 1]);
  }

  path_ = std::string(path_view);
}

void Request::FindHeaders(const std::vector<std::string> &lines) {
  for (uint16_t i = 1; i < lines.size(); ++i) {
    std::string pre_line = lines[i];
    boost::algorithm::erase_all(pre_line, " ");
    if (pre_line.empty()) {
      body_ = "";
      if (i < lines.size()-1) {
        for (uint16_t j = i+1; j < lines.size(); ++j) {
          pre_line = lines[j];
          boost::algorithm::erase_all(pre_line, " ");
          body_ += pre_line + "\n";
        }
      }
      return;
    }
    std::vector<std::string> line = SplitRequestText(pre_line, ":");
    if(line[0] == "Host") {
      host_ = line[1];
    } else if(line[0] == "Content-Type") {
      body_type_ = line[1];
    } else {
      headers_.emplace(line[0], line[1]);
    }
  }

}

//================================= End namespace ==============================

}  // namespace aurora_nexus