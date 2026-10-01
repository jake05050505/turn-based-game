#include <iostream>
#include <print>
#include <random>
#include <ctime>
#include <thread>
#include <chrono>
#include <limits>

#include "player.h"

#define sleep(time) std::this_thread::sleep_for(std::chrono::milliseconds(time))

// adds delay between cpu moves
void think() {
    std::cout << "." << std::flush;
    sleep(500);
    std::cout << "." << std::flush;
    sleep(500);
    std::cout << "." << std::flush;
    sleep(500);
    std::cout << "\r" << std::flush;
}

int main() {
    srand(time(nullptr)); rand(); // seeds random number generator and discards the first value

    Player user = Player::create("User", {
        .max_health = 20,
        .health = 20,
        .damage = 5
    });
    Player cpu  = Player::create("CPU", {
        .max_health = 50,
        .health = 50,
        .damage = 3
    });

    // Variable initialisation
    int user_input;
    int remaining_actions; // number of actions which the user can make sequentially in a single turn

    PlayerAction player_action;

    // Display initial stats
    user.stats();
    cpu.stats();

    // Gameplay loop
    while (user.get_health() > 0 && cpu.get_health() > 0) {

        std::println("Player's turn!");
        remaining_actions = 2;

        while (remaining_actions > 0 && cpu.get_health() > 0 && user.get_health() > 0) {
            std::print("Remaining actions: {}/2\nPlease select one of the following actions:\n[1] Attack\n[2] Buff\n[3] Heal\n[4] Check\n[5] End turn\n[-1] Exit Game\n\n>>> ", remaining_actions);
            while (!(std::cin >> user_input)) {
                std::cout << "Invalid input, please try again.\n>>> ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            player_action = static_cast<PlayerAction>(user_input);

            if (player_action == PlayerAction::Quit) {
                return 0;
            }

            do_action(player_action, user, cpu, &remaining_actions);
            sleep(500);
        }

        if (cpu.get_health() == 0) break;

        std::cout << std::endl;
        sleep(1000);
        std::println("CPU's turn!\n");
        remaining_actions = 2;

        while (remaining_actions > 0) {
            player_action = static_cast<PlayerAction>(1 + rand() % 3);
            think();
            do_action(player_action, cpu, user, &remaining_actions);
            sleep(500);
        }

        std::cout << std::endl;
        sleep(1000);
    }

    if (cpu.get_health() == 0) std::println("You Win!");
    if (user.get_health() == 0) std::println("Game Over!");

    return 0;
}