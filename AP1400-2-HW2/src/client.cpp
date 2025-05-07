#include "client.h"
#include "crypto.h"
#include "server.h"
#include <chrono>
#include <random>
#include <string>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

Client::Client(std::string id, const Server &server) : id(id), server(&server) {
  crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() const { return id; }

std::string Client::get_publickey() const { return public_key; }

double Client::get_wallet() const { return server->get_wallet(id); }

std::string Client::sign(std::string txt) const {
  return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value) {
  auto transaction = id + "-";
  transaction += receiver + "-";
  transaction += std::to_string(value);
  return server->add_pending_trx(transaction, sign(transaction));
}

size_t Client::generate_nonce() { return rng(); }