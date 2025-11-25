#ifndef OPEN_DATA_HUB_API_H
#define OPEN_DATA_HUB_API_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class OpenDataHubAPI {
private:
    std::string api_base = "https://mobility.api.opendatahub.com/v2";
    http_client_config client_config;
    
    http_request create_request(const std::string& endpoint, const std::string& method) {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("mobility.api.opendatahub.com"));
        request.headers().add(U("Content-Type"), U("application/json"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        return request;
    }
    
    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                auto encoded_value = web::uri::encode_data_string(utility::conversions::to_string_t(param.second));
                query += param.first + "=" + utility::conversions::to_utf8string(encoded_value);
                first = false;
            }
        }
        return query;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method) {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    OpenDataHubAPI() {
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> get_entry_points(const std::string& origin = "") {
        std::map<std::string, std::string> params;
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_categories(const std::string& representation = "flat,node", const std::string& origin = "") {
        std::map<std::string, std::string> params;
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_stations(
        const std::string& representation = "flat,node",
        const std::string& stationTypes = "*",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(stationTypes))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_edges(
        const std::string& representation = "flat,edge",
        const std::string& edgeTypes = "*",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(edgeTypes))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_stations_with_data_types(
        const std::string& representation = "flat,node",
        const std::string& stationTypes = "*",
        const std::string& dataTypes = "*",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(stationTypes))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(dataTypes))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_latest_measurements(
        const std::string& representation = "flat,node",
        const std::string& stationTypes = "*",
        const std::string& dataTypes = "*",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& timezone = "UTC",
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (timezone != "UTC") params["timezone"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(timezone)));
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(stationTypes))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(dataTypes))) + "/latest" + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_historical_measurements(
        const std::string& representation = "flat,node",
        const std::string& stationTypes = "*",
        const std::string& dataTypes = "*",
        const std::string& from = "",
        const std::string& to = "",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& timezone = "UTC",
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (timezone != "UTC") params["timezone"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(timezone)));
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(stationTypes))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(dataTypes))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_metadata_history(
        const std::string& representation = "flat,node",
        const std::string& stationTypes = "*",
        const std::string& from = "",
        const std::string& to = "",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& timezone = "UTC",
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (timezone != "UTC") params["timezone"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(timezone)));
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(stationTypes))) + "/metadata/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_events(
        const std::string& representation = "flat,event",
        const std::string& eventorigins = "*",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(eventorigins))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_latest_events(
        const std::string& representation = "flat,event",
        const std::string& eventorigins = "*",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(eventorigins))) + "/latest" + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_events_at_timepoint(
        const std::string& representation = "flat,event",
        const std::string& eventorigins = "*",
        const std::string& timepoint = "",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(eventorigins))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(timepoint))) + build_query_params(params), "GET");
    }

    pplx::task<json::value> get_events_in_interval(
        const std::string& representation = "flat,event",
        const std::string& eventorigins = "*",
        const std::string& from = "",
        const std::string& to = "",
        int limit = 200,
        int offset = 0,
        const std::string& select = "",
        const std::string& where = "",
        bool shownull = false,
        bool distinct = true,
        const std::string& origin = "") {
        
        std::map<std::string, std::string> params;
        if (limit != 200) params["limit"] = std::to_string(limit);
        if (offset != 0) params["offset"] = std::to_string(offset);
        if (!select.empty()) params["select"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(select)));
        if (!where.empty()) params["where"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(where)));
        if (shownull) params["shownull"] = "true";
        if (!distinct) params["distinct"] = "false";
        if (!origin.empty()) params["origin"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(origin)));
        
        return make_api_call("/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(representation))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(eventorigins))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))) + build_query_params(params), "GET");
    }
};

#endif
