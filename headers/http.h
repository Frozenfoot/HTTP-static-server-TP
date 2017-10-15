//
// Created by frozenfoot on 12.10.17.
//

#ifndef UNTITLED_HTTP_H
#define UNTITLED_HTTP_H

#include <string>
#include <map>

#define GET_METHOD "GET"
#define HEAD_METHOD "HEAD"

#define SERVER_NAME "Test-server"

#define STATUS_OK "HTTP/1.1 200 OK\r\n"
#define STATUS_NOT_FOUND "HTTP/1.1 404 Not Found\r\n"
#define STATUS_FORBIDDEN "HTTP/1.1 403 Forbidden\r\n"
#define STATUS_ERROR "HTTP/1.1 500 Internal Server Error\r\n"

#define HTTP_RAW_OK "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nHello, World!"
#define HTTP_RAW_BAD_REQUEST "HTTP/1.1 400 Bad Request\r\nServer: TP-Server\r\nContent-Length: 22\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nError parsing request!"
#define HTTP_RAW_FORBIDDEN "HTTP/1.1 403 Forbidden\r\nServer: TP-Server\r\nContent-Length: 9\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nForbidden"
#define HTTP_RAW_NOT_FOUND "HTTP/1.1 404 Not Found\r\nServer: TP-Server\r\nContent-Length: 9\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nNot Found"
#define HTTP_RAW_NOT_ALLOWED "HTTP/1.1 405 Method Not Allowed\r\nServer: TP-Server\r\nContent-Length: 19\r\nContent-Type: text/plain\r\nConnection: Closed\r\n\r\nMethod not allowed!"
#define HTTP_RAW_ERROR "HTTP/1.1 500 Internal Server Error\r\nServer: TP-Server\r\nContent-Length: 15\r\nContent-Type: text/plain\r\nConnection: Closed\r\nInternal error!"


class Http {
public:
    static std::string makeResponseHead(const std::string &status, const std::string &date,
            const std::string contentType, ssize_t contentLength, const std::string &connection);

    static std::string makeResponse (const std::string &status, const std::string &date,
            const std::string &contentType, ssize_t contentLength, const std::string &connection, const std::string &body);

    static void parseRequest(const std::string &request, std::string &method, std::string &path);

    static std::string getMimeType(std::string &filePath);

    static std::string getDate();

private:
    static std::map<std::string, std::string> mimeTypes;

};


#endif //UNTITLED_HTTP_H
