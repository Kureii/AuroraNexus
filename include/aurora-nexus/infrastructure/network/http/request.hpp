/* This file was created by Kureii (Tomas Adamek)
 * Date created: 28. 09. 2023
 * This file is under the MIT license
 */

#ifndef AURORANEXUS_REQUEST_H
#define AURORANEXUS_REQUEST_H

#include <iostream>
#include <map>
#include <vector>
#include <memory>

namespace aurora_nexus {

class Request {
  std::string method_;  // GET, POST, atd.
  std::string path_;    // /home, /blog, atd.
  std::string host_;    //
  std::unique_ptr<std::map<std::string, std::string>> query_; // ?query=value
  std::map<std::string, std::string> headers_; //
  std::string body_;
  std::string body_type_;

  /**
   * @brief Splits a given string by a specified separator into a vector of substrings.
   *
   * This method takes a string and a separator as input and returns a vector containing
   * the substrings that result from splitting the input string by the separator.
   *
   * @param to_split The string to be split.
   * @param separator The string used as the separator for splitting.
   * @return std::vector<std::string> A vector containing the substrings.
   *
   * @note
   * - If the input string is empty, the returned vector will be empty.
   * - If the separator is not found in the input string, the returned vector will contain the entire input string as its only element.
   *
   * @example
   * ```
   * std::string my_text = "apple,orange,banana";
   * Request req;
   * auto result = req.SplitRequestText(my_text, ",");
   * // result will be {"apple", "orange", "banana"}
   * ```
   */
  std::vector<std::string> SplitRequestText(const std::string &to_split, const std::string& separator);

  /**
   * @brief Parses the URL path and query string.
   *
   * This method splits the input URL into its path and query string components.
   * The path is stored in the private member variable `path_`, while the query string
   * is split into key-value pairs and stored in `query_`.
   *
   * @param URL Reference to a std::string containing the URL to be parsed.
   *
   * @note
   * - If the URL does not contain a query string, `path_` will be set to the entire URL and `query_` will remain empty.
   * - If the query string contains an odd number of key-value elements, the last element will be ignored.
   *
   * @example
   * ```
   * std::string my_url = "/path?name=John&age=30";
   * Request req;
   * req.ParseURLPath(my_url);
   * ```
   */
  void ParseURLPath(const std::string &URL);

  /**
   * @brief Parses and stores the headers from a vector of HTTP request lines.
   *
   * This method iterates through a vector of lines representing an HTTP request
   * and extracts the headers, storing them in the private member variable `headers_`.
   * Special headers like "Host" and "Content-Type" are stored in their respective
   * private member variables `host_` and `body_type_`.
   *
   * @param lines A vector of strings, each representing a line in the HTTP request.
   *
   * @note
   * - The method assumes that the first line of the HTTP request (the request line) is not included in the `lines` vector.
   * - If an empty line is encountered, it is assumed that the headers section has ended, and the remaining lines are considered as the body.
   *
   * @example
   * ```
   * std::vector<std::string> my_lines = {"Host: example.com", "Content-Type: text/html", "", "body content"};
   * Request req;
   * req.FindHeaders(my_lines);
   * ```
   */
  void FindHeaders(const std::vector<std::string> &lines);
 public:
  explicit Request(const std::string& http_text);

  /**
   * @brief Retrieves the HTTP method of the request.
   *
   * @return const std::string& The HTTP method as a string.
   */
  [[nodiscard]] const std::string &GetMethod() const;

  /**
   * @brief Retrieves the path of the request URL.
   *
   * @return const std::string& The path as a string.
   */
  [[nodiscard]] const std::string &GetPath() const;

  /**
   * @brief Retrieves the query parameters of the request.
   *
   * @return const std::map<std::string, std::string>& A map containing key-value pairs of the query parameters.
   */
  [[nodiscard]] const std::map<std::string, std::string> &GetQuery() const;

  /**
   * @brief Retrieves the headers of the request.
   *
   * @return const std::map<std::string, std::string>& A map containing header-value name pairs.
   */
  [[nodiscard]] const std::map<std::string, std::string> &GetHeaders() const;

  /**
   * @brief Retrieves the body of the request.
   *
   * @return const std::string& The body as a string without space characters.
   */
  [[nodiscard]] const std::string &GetBody() const;

  /**
   * @brief Retrieves the host of the request.
   *
   * @return const std::string& The host URL as a string.
   */
  [[nodiscard]] const std::string &GetHost() const;


  /**
   * @brief Retrieves the body format of the request.
   *
   * @return const std::string& The body format name as a string.
   */
  [[nodiscard]] const std::string &GetBodyType() const;
};

}

#endif  // AURORANEXUS_REQUEST_H
