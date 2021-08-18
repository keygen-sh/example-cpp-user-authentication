#include "include/json.hpp"
#include <iostream>
#include <string>
#include <curl/curl.h>

using json = nlohmann::json;

size_t wfun(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);

    return size * nmemb;
}

std::string generate_token(std::string account_id, const char *email, const char *password)
{
  std::string buf;

  auto curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, (std::string("https://api.keygen.sh/v1/accounts/") + account_id + std::string("/tokens")).c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 2);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wfun);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

    curl_easy_setopt(curl, CURLOPT_USERNAME, email);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

    auto res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  return buf;
}

std::string get_current_user(std::string account_id, std::string token)
{
  std::string buf;

  auto curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, (std::string("https://api.keygen.sh/v1/accounts/") + account_id + std::string("/me")).c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wfun);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

    auto headers = curl_slist_append(NULL, (std::string("Authorization: Bearer ") + token).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    auto res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  return buf;
}

std::string list_licenses(std::string account_id, std::string token)
{
  std::string buf;

  auto curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, (std::string("https://api.keygen.sh/v1/accounts/") + account_id + std::string("/licenses")).c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wfun);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

    auto headers = curl_slist_append(NULL, (std::string("Authorization: Bearer ") + token).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    auto res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  return buf;
}

int main(void)
{
  curl_global_init(CURL_GLOBAL_ALL);

  std::string account_id = getenv("KEYGEN_ACCOUNT_ID");
  std::string token = "";

  // Authenticate and create an API token
  {
    auto buf = generate_token(account_id, "test@keygen.example", "secret");
    auto body = json::parse(buf);
    if (body.contains("data"))
    {
      token = body.at("data").at("attributes").at("token");
    }

    std::cout << "generate token response:" << "\n"
              << body.dump(4)
              << std::endl;
  }

  // Show the current user
  {
    auto buf = get_current_user(account_id, token);
    auto body = json::parse(buf);

    std::cout << "whoami response:" << "\n"
              << body.dump(4)
              << std::endl;
  }

  // List the current user's licenses
  {
    auto buf = list_licenses(account_id, token);
    auto body = json::parse(buf);

    std::cout << "list licenses response:" << "\n"
              << body.dump(4)
              << std::endl;
  }

  curl_global_cleanup();

  return 0;
}
