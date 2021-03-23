/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SYSC_SC_COMM_SINGLETON_H_
#define _SYSC_SC_COMM_SINGLETON_H_

#include "seasocks/WebSocket.h"
#include <seasocks/PageHandler.h>
#include <sysc/kernel/sc_module.h>

#include <cstring>
#include <functional>
#include <memory>
#include <thread>

namespace vpvper {
namespace generic {

class WsHandler : public seasocks::WebSocket::Handler {
public:
    explicit WsHandler() {}

    void onConnect(seasocks::WebSocket *connection) override;

    void onData(seasocks::WebSocket *connection, const char *data) override;

    void onDisconnect(seasocks::WebSocket *connection) override;

    void send(std::string msg) {
        for (auto *con : _connections) con->send(msg);
    }

    void set_receive_callback(std::function<void(const char *data)> cb) { callback = cb; }

private:
    std::set<seasocks::WebSocket *> _connections;
    std::function<void(const char *data)> callback;
};

class sc_comm_singleton : public sc_core::sc_module {
    struct DefaultPageHandler : public seasocks::PageHandler {
        DefaultPageHandler(sc_comm_singleton &o)
        : owner(o) {}
        virtual std::shared_ptr<seasocks::Response> handle(const seasocks::Request &request);
        sc_comm_singleton &owner;
    };

public:
    sc_comm_singleton() = delete;

    sc_comm_singleton(const sc_comm_singleton &) = delete;

    sc_comm_singleton &operator=(sc_comm_singleton &o) = delete;

    virtual ~sc_comm_singleton();

    static sc_comm_singleton &inst() {
        static sc_comm_singleton i("__sc_singleton");
        return i;
    }

    seasocks::Server &get_server();

    void registerWebSocketHandler(const char *endpoint, std::shared_ptr<seasocks::WebSocket::Handler> handler,
                                  bool allowCrossOriginRequests = false);

    void execute(std::function<void()> f);

    void start_client();

protected:
    void start_of_simulation() override;
    void end_of_simulation() override;

private:
    sc_comm_singleton(sc_core::sc_module_name nm);
    std::unique_ptr<seasocks::Server> m_serv;
    std::thread t;
    void thread_func();
    bool needs_client, client_started;
    std::vector<std::string> endpoints;
};

} /* namespace sifive */
} /* namespace vpvper*/

#endif /* _SYSC_SC_COMM_SINGLETON_H_ */
