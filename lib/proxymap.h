#pragma once

#include "../src/utils/logger.h"
#include "../src/utils/Singleton.h"
#include "../src/factory/socks2c_factory.h"

#include <unordered_map>
#include <mutex>

template <class Protocol>
class ProxyMap : public Singleton<ProxyMap<Protocol>> {


public:


    bool IsProxyExist(int port)
    {
        std::lock_guard<std::mutex> lg(map_mutex);

        auto sit = server_map.find(port);
        auto cit = client_map.find(port);
        if (sit != server_map.end() || cit != client_map.end()) return true;

        return false;
    }


    bool Insert(int port, ServerProxy<Protocol> handle)
    {
        std::lock_guard<std::mutex> lg(map_mutex);

        return server_map.insert({port, handle}).second;
    }

    bool Insert(int port, ClientProxy<Protocol> handle)
    {
        std::lock_guard<std::mutex> lg(map_mutex);

        return client_map.insert({port, handle}).second;
    }


    bool StopProxy(int port)
    {
        std::lock_guard<std::mutex> lg(map_mutex);

        auto cit = client_map.find(port);
        auto sit = server_map.find(port);

        if (sit == server_map.end() && cit == client_map.end()) return false;

        if (sit != server_map.end() && cit == client_map.end()) return false;

        if (sit != server_map.end())
        {
            if (std::get<0>(sit->second)->Stopped()) return false;
            std::get<0>(sit->second)->StopProxy();
            std::get<1>(sit->second)->StopProxy();
            return true;
        }


        if (cit != client_map.end())
        {
            if (std::get<0>(cit->second)->Stopped()) return false;
            std::get<0>(cit->second)->StopProxy();
            std::get<1>(cit->second)->StopProxy();
            return true;
        }


    }

    bool ClearProxy(int port)
    {
        std::lock_guard<std::mutex> lg(map_mutex);

        auto cit = client_map.find(port);
        auto sit = server_map.find(port);

        if (sit == server_map.end() && cit == client_map.end()) return false;

        if (sit != server_map.end() && cit == client_map.end()) return false;

        if (sit != server_map.end())
        {
            if (std::get<1>(sit->second)->ShouldClose() && std::get<0>(sit->second)->ShouldClose())
            {
                std::get<0>(sit->second).reset();
                std::get<1>(sit->second).reset();
                server_map.erase(port);
                return true;
            }
            return false;
        }


        if (cit != client_map.end())
        {
            if (std::get<1>(cit->second)->ShouldClose() && std::get<0>(cit->second)->ShouldClose())
            {
                std::get<0>(cit->second).reset();
                std::get<1>(cit->second).reset();
                client_map.erase(port);
                return true;
            }
            return false;
        }



    }


private:
    std::mutex map_mutex;

    std::unordered_map<int, ServerProxy<Protocol>> server_map;
    std::unordered_map<int, ClientProxy<Protocol>> client_map;


};

