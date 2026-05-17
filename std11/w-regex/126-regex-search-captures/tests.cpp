#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

TEST(regex_search_captures, basic_localhost_8080)
{
    std::string host;
    int port = 0;
    EXPECT_TRUE(task126::parse_host_port("localhost:8080", host, port));
    EXPECT_EQ(host, "localhost");
    EXPECT_EQ(port, 8080);
}

TEST(regex_search_captures, dotted_host)
{
    std::string host;
    int port = 0;
    EXPECT_TRUE(task126::parse_host_port("example.com:443", host, port));
    EXPECT_EQ(host, "example.com");
    EXPECT_EQ(port, 443);
}

TEST(regex_search_captures, ip_address_host)
{
    std::string host;
    int port = 0;
    EXPECT_TRUE(task126::parse_host_port("127.0.0.1:5432", host, port));
    EXPECT_EQ(host, "127.0.0.1");
    EXPECT_EQ(port, 5432);
}

TEST(regex_search_captures, single_digit_port)
{
    std::string host;
    int port = 0;
    EXPECT_TRUE(task126::parse_host_port("a:1", host, port));
    EXPECT_EQ(host, "a");
    EXPECT_EQ(port, 1);
}

TEST(regex_search_captures, no_colon_fails)
{
    std::string host;
    int port = 0;
    EXPECT_FALSE(task126::parse_host_port("localhost", host, port));
}

TEST(regex_search_captures, empty_port_fails)
{
    std::string host;
    int port = 0;
    EXPECT_FALSE(task126::parse_host_port("localhost:", host, port));
}

TEST(regex_search_captures, empty_host_fails)
{
    std::string host;
    int port = 0;
    EXPECT_FALSE(task126::parse_host_port(":8080", host, port));
}

TEST(regex_search_captures, non_digit_port_fails)
{
    std::string host;
    int port = 0;
    EXPECT_FALSE(task126::parse_host_port("localhost:abc", host, port));
}

TEST(regex_search_captures, mixed_port_fails)
{
    std::string host;
    int port = 0;
    EXPECT_FALSE(task126::parse_host_port("localhost:80a", host, port));
}

TEST(regex_search_captures, trailing_text_fails)
{
    std::string host;
    int port = 0;
    EXPECT_FALSE(task126::parse_host_port("localhost:8080 extra", host, port));
}

TEST(regex_search_captures, empty_input_fails)
{
    std::string host;
    int port = 0;
    EXPECT_FALSE(task126::parse_host_port("", host, port));
}
