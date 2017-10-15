//
// Created by frozenfoot on 12.10.17.
//

#include "../headers/http.h"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>

std::map<std::string, std::string> Http::mimeTypes = {{"js", "application/javascript"},
                                                      {"html", "text/html"}, {"htm", "text/html"},
                                                      {"css", "text/css"}, {"jpg", "image/jpeg"},
                                                      {"jpeg", "image/jpeg"}, {"png", "image/png"},
                                                      {"gif", "image/gif"},
                                                      {"txt", "text/plain"},
                                                      {"swf", "application/x-shockwave-flash"}};

void Http::parseRequest(const std::string &request, std::string &method, std::string &path) {
    auto i = 0;

    while((request[i] != ' ') && (i < request.length() - 1) && (request[i] != '\r') && (request[i] != '\n') ){
        method += request[i];
        ++i;
    }

    if(request[i] == '\0'){
        return;
    }

    ++i;

    char hex[3];
    int code;
    char currentChar;

    while((request[i] != ' ') && (i < request.length() - 1) && (request[i] != '\r')
          && (request[i] != '\n') && (request[i] != '?')) {
        currentChar = request[i];

        if(currentChar == '%'){
            hex[0] = request[i + 1];
            hex[1] = request[i + 2];
            hex[3] = '\0';
            sscanf(hex, "%X", &code);
            path += (char) code;
            i += 2;
        }
        else if(currentChar == '+'){
            path += ' ';
        }
        else{
            path += currentChar;
        }
        ++i;
    }

    std::cout << "Request:" << request << "--------\nMethod:" << method << "--------------\nPath:" << path;
}

std::string Http::makeResponseHead(const std::string &status, const std::string &date, const std::string contentType,
                                   ssize_t contentLength, const std::string &connection) {
    return (status + "Date: " + date + "\r\nServer: " + SERVER_NAME + "\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n");
}

std::string Http::makeResponse(const std::string &status, const std::string &date, const std::string &contentType,
                               ssize_t contentLength, const std::string &connection, const std::string &body) {
    return (status + "Date: " + date + "\r\nServer: " + SERVER_NAME + "\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n" + body);
}

std::string Http::getMimeType(std::string &filePath) {
    std::string result;
    auto position = filePath.find_last_of('.', filePath.length());
    if(position != std::string::npos){
        std::string extension = filePath.substr(position + 1, filePath.length() - 1);
        result = mimeTypes[extension];
        if(result.length() == 0){
            result = "application/octet-stream";
        }
    }
    return result;
}

std::string Http::getDate() {
    auto currentTime = time(nullptr);
    struct tm time = *localtime(&currentTime);
    char* buffer = (char*)malloc(sizeof(char)*64);
    std::string weekDay;
    switch (time.tm_wday) {
        case 1:
            weekDay = "Mon";
            break;
        case 2:
            weekDay = "Tue";
            break;;
        case 3:
            weekDay = "Wed";
            break;
        case 4:
            weekDay = "Thu";
            break;
        case 5:
            weekDay = "Fri";
            break;
        case 6:
            weekDay = "Sat";
            break;
        case 0:
            weekDay = "Sun";
            break;
    }
    std::string month;
    switch (time.tm_mon) {
        case 0:
            month = "Jan";
            break;
        case 1:
            month = "Feb";
            break;
        case 2:
            month = "Mar";
            break;
        case 3:
            month = "Apr";
            break;
        case 4:
            month = "May";
            break;
        case 5:
            month = "Jun";
            break;
        case 6:
            month = "Jul";
            break;
        case 7:
            month = "Aug";
            break;
        case 8:
            month = "Sep";
            break;
        case 9:
            month = "Oct";
            break;
        case 10:
            month = "Nov";
            break;
        case 11:
            month = "Dec";
            break;
    }

    sprintf(buffer, "%s, %d %s %d %d:%d:%d %s", weekDay.c_str(), time.tm_mday, month.c_str(), time.tm_year+1990, time.tm_hour, time.tm_min,
            time.tm_sec, time.tm_zone);
    std::cout << ("%s, %d %s %d %d:%d:%d %s", weekDay.c_str(), time.tm_mday, month.c_str(), time.tm_hour, time.tm_min, time.tm_sec, time.tm_zone);
    std::string result = buffer;
    delete buffer;
    return result;
}