#include "tikpp/api.hpp"
#include "tikpp/request.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <future>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <thread>
#include <vector>

struct api_error_handler {
    void on_error(const boost::system::error_code &err) {
        std::cout << "[!] An error occured: " << err.message() << std::endl;
    }
};

void api_handler(std::shared_ptr<tikpp::api<api_error_handler>> &&api) {
    std::vector<std::string> words {};
    std::string              line {};
    std::regex               patt {R"(([\.=][^=]+)=(.*))"};

    while (true) {
        words.clear();

        while (true) {
            std::cout << " <<< " << std::flush;
            std::getline(std::cin, line);

            if (words.empty() && line.empty()) {
                continue;
            }

            if (line.empty()) {
                break;
            }

            words.push_back(line);
        }

        auto req = api->make_request(words[0]);
        words.erase(words.begin());

        for (const auto &word : words) {
            std::smatch word_parts;

            if (!std::regex_match(word, word_parts, patt)) {
                std::cerr << "[!] Invalid word: " << word << std::endl;
                continue;
            }

            req->add_word(word_parts[1], word_parts[2]);
        }

        api->async_send(std::move(req), [](const auto &err, auto &&resp) {
            if (err) {
                std::cerr << "[!] Could not send request: " << err.message()
                          << std::endl;
                return false;
            }

            for (const auto &[key, value] : resp) {
                std::cout << " >>> " << key << " = " << value << std::endl;
            }

            return false;
        });
    }
}

auto main(int argc, char *argv[]) -> int {
    std::string   host {"127.0.0.1"};
    std::uint16_t port {8728};
    std::string   username {"admin"};
    std::string   password {""};

    // std::cout << "[+] Host: " << std::flush;
    // std::cin >> host;
    // std::cout << "[+] Port: " << std::flush;
    // std::cin >> port;
    // std::cout << "[+] Username: " << std::flush;
    // std::cin >> name;
    // std::cout << "[+] Password: " << std::flush;
    // std::cin >> password;

    boost::asio::io_context io {};
    api_error_handler       error_handler {};

    auto api = tikpp::api<api_error_handler>::create(io, error_handler);

    api->async_open(host, port, [&](const auto &err) mutable {
        if (err) {
            std::cerr << "[!] Could not connect: " << err.message()
                      << std::endl;
            return;
        }

        std::cout << "[+] Connected successfully" << std::endl;
        api->start();

        api->async_login(
            username, password, [&io, api {std::move(api)}](const auto &err) {
                if (err) {
                    std::cerr << "[!] Could not login: " << err.message()
                              << std::endl;
                    io.stop();
                    return;
                }

                std::cout << "[+] Logged in successfully" << std::endl;

                std::thread([api = std::move(api)]() mutable {
                    api_handler(std::move(api));
                }).detach();
            });
    });

    io.run();
    return 0;
}
