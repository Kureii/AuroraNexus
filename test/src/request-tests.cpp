/* This file was created by Kureii (Tomas Adamek)
* Date created: 02. 10. 2023
* This file is under the MIT license
*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <map>

#include "aurora-nexus/infrastructure/network/http/request.hpp"

std::string host = "github.com/Kureii/AuroraNexus";

std::string empty_get_method = "GET / HTTP/1.1\r\n"
    "Host: kureii.cz\r\n"
    "User-Agent: curl/8.0.1\r\n"
    "Accept: */*\r\n\r\n";

std::string get_method = "GET /Kureii/AuroraNexus HTTP/1.1\r\n"
    "Host: github.com\r\n"
    "User-Agent: curl/8.0.1\r\n"
    "Accept: */*\r\n\r\n";

std::string get_method_with_query = "GET /blog?article=Fdf56dDSz51&cmnt_sort=date_up HTTP/1.1\r\n"
    "Host: kureii.cz\r\n"
    "User-Agent: curl/8.0.1\r\n"
    "Accept: */*\r\n\r\n";

std::string post_method_json = "POST /login HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: 39\r\n"
    "\r\n"
    "{\r\n"
    "  \"username\": \"your_username\",\r\n"
    "  \"password\": \"your_password\"\r\n"
    "}";


TEST(REQUEST_TEST, get_method) {
  auto request = std::make_unique<aurora_nexus::Request>(get_method.c_str());

  std::string result = "GET";

  ASSERT_EQ(request->GetMethod(), result);
}

TEST(REQUEST_TEST, parese_enpty_path) {
  auto request = std::make_unique<aurora_nexus::Request>(empty_get_method.c_str());

  std::string result = "/";

  ASSERT_EQ(request->GetPath(), result);
}

TEST(REQUEST_TEST, parese_non_enpty_path) {
  auto request = std::make_unique<aurora_nexus::Request>(get_method.c_str());

  std::string result = "/Kureii/AuroraNexus";

  ASSERT_EQ(request->GetPath(), result);
}

TEST(REQUEST_TEST, get_path_with_query) {
  auto request = std::make_unique<aurora_nexus::Request>(get_method_with_query.c_str());

  std::string result = "/blog";

  ASSERT_EQ(request->GetPath(), result);
}

TEST(REQUEST_TEST, get_host) {
  auto request = std::make_unique<aurora_nexus::Request>(empty_get_method.c_str());

  std::string result = "kureii.cz";

  ASSERT_EQ(request->GetHost(), result);
}

TEST(REQUEST_TEST, get_query) {
  auto request = std::make_unique<aurora_nexus::Request>(get_method_with_query.c_str());

  std::map<std::string, std::string> result;

  result.emplace("article","Fdf56dDSz51");
  result.emplace("cmnt_sort","date_up");

  ASSERT_EQ(request->GetQuery(), result);
}

TEST(REQUEST_TEST, get_headers) {
  auto request = std::make_unique<aurora_nexus::Request>(get_method.c_str());

  std::map<std::string, std::string> result;

  result.emplace("User-Agent","curl/8.0.1");
  result.emplace("Accept","*/*");

  ASSERT_EQ(request->GetHeaders(), result);
}

TEST(REQUEST_TEST, get_empty_body) {
  auto request = std::make_unique<aurora_nexus::Request>(empty_get_method.c_str());

  ASSERT_TRUE(request->GetBody().empty());
}

TEST(REQUEST_TEST, get_empty_body_type) {
  auto request = std::make_unique<aurora_nexus::Request>(empty_get_method.c_str());

 ASSERT_TRUE(request->GetBodyType().empty());
}

TEST(REQUEST_TEST, get_body) {
  auto request = std::make_unique<aurora_nexus::Request>(post_method_json.c_str());

  std::string result = "{\n\"username\":\"your_username\",\n\"password\":\"your_password\"\n}\n";
  ASSERT_EQ(request->GetBody(), result);
}

TEST(REQUEST_TEST, get_body_type) {
  auto request = std::make_unique<aurora_nexus::Request>(post_method_json.c_str());

  std::string result = "application/json";
  ASSERT_EQ(request->GetBodyType(), result);
}