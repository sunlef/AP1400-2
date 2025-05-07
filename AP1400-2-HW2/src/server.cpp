#include "server.h"
#include "client.h"
#include "crypto.h"
#include <chrono>
#include <random>
#include <string>
#include <vector>

Server::Server() {}

std::vector<std::string> pending_trxs;

std::shared_ptr<Client> Server::add_client(std::string id) {
  bool exsit = false;
  for (auto &[x, y] : clients) {
    if (x->get_id() == id) {
      exsit = true;
    }
  }

  if (exsit) {
    for (int i = 0; i < 4; i++) {
      id +=
          (std::mt19937(
               std::chrono::steady_clock::now().time_since_epoch().count())() %
           10) +
          '0';
    }
  }

  auto ptr = std::make_shared<Client>(id, *this);
  clients[ptr] = 5;
  return ptr;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
  for (auto &[x, y] : clients) {
    if (x->get_id() == id) {
      return x;
    }
  }
  return nullptr;
}

double Server::get_wallet(const std::string &id) const {
  for (auto &[x, y] : clients) {
    if (x->get_id() == id) {
      return y;
    }
  }
  return 0;
}

bool Server::parse_trx(std::string trx, std::string &sender,
                       std::string &receiver, double &value) {
  try {
    int p = trx.find_first_of('-');
    int q = trx.find_last_of('-');
    if (p == std::string::npos || q == std::string::npos || p == q) {
      throw std::runtime_error("I cannot understand this transaction.");
    }

    sender = trx.substr(0, p);
    receiver = trx.substr(p + 1, q - p - 1);
    value = std::stod(trx.substr(q + 1));

    return true;
  } catch (...) {
    throw;
  }
}

bool Server::add_pending_trx(const std::string &trx,
                             const std::string &signature) const {
  std::string sender;
  std::string receiver;
  double amount;
  parse_trx(trx, sender, receiver, amount);

  std::shared_ptr<Client> ptr;
  double this_guy_balance;

  for (auto &[x, y] : clients) {
    if (x->get_id() == sender) {
      ptr = x;
      this_guy_balance = y;
    }
  }

  auto who_receive_this_shit = get_client(receiver);

  bool result = who_receive_this_shit != nullptr &&
                this_guy_balance >= amount &&
                crypto::verifySignature(ptr->get_publickey(), trx, signature);

  if (result) {
    pending_trxs.push_back(trx);
  }

  return result;
}

size_t Server::mine() {
  std::string mempool;
  for (auto &str : pending_trxs) {
    mempool += str;
  }

  bool has_succeed = false;
  size_t the_luck_number;
  std::shared_ptr<Client> the_luck_guy;

  auto check_mining_status = [&]() {
    auto hash = crypto::sha256(mempool + std::to_string(the_luck_number));
    for (int i = 0; i < 8; i++) {
      if (hash.substr(i, 3) == "000") {
        return true;
      }
    }
    return false;
  };

  auto take_effect = [&]() {
    for (auto &str : pending_trxs) {
      std::string sender;
      std::string receiver;
      double amount;
      parse_trx(str, sender, receiver, amount);

      clients[get_client(sender)] -= amount;
      clients[get_client(receiver)] += amount;
    }
  };

  while (!has_succeed) {
    for (auto &[x, _] : clients) {
      if (has_succeed) {
        break;
      }
      the_luck_guy = x;
      the_luck_number = x->generate_nonce();
      has_succeed = check_mining_status();
    }
  }

  take_effect();
  pending_trxs.clear();
  std::cout << the_luck_guy->get_id() << " has successfully mined something!"<< std::endl;
  return the_luck_number;
}
