#pragma once

#include <string>
#include <algorithm>
#include <print>

enum class PlayerAction {
    Quit        =  -1,
    Attack      =   1,
    Buff        =   2,
    Heal        =   3,
    CheckStats  =   4,
    EndTurn     =   5
};

class Player {
    public:
        struct Stats {
            int max_health;
            int health;
            int damage;
        };

    private:
        std::string     name;

        int             max_health;
        int             health;
        int             damage;

        Player(std::string name, int max_health, int damage)
            : name(name),
            max_health(std::clamp(max_health, 1, 255)),
            health(std::clamp(max_health, 1, 255)),
            damage(std::clamp(damage, 1, 255)) {}

        Player(std::string name, Stats stats)
            : name(name),
            max_health(std::clamp(stats.max_health, 1, 255)),
            health(std::clamp(
                std::min(stats.health, stats.max_health), 1, 255
            )),
            damage(std::clamp(stats.damage, 1, 255)) {}

    public:
        static Player create(std::string name, Stats stats) {
            return Player(name, stats);
        }
        static Player create(std::string name, int max_health, int damage) {
            return Player(name, {
                .max_health = max_health,
                .health = max_health,
                .damage = damage
            });
        }

        int get_health() {
            return this->health;
        }
        int get_maxhealth() {
            return this->max_health;
        }
        int get_damage() {
            return this->damage;
        }

        void set_health(int health) {
            this->health = std::clamp(health, 0, max_health);
        }
        void set_maxhealth(int max_health) {
            this->max_health = std::clamp(max_health, 0, 255);
            this->health = std::clamp(this->health, 0, max_health);
        }
        void set_damage(int damage) {
            this->damage = std::clamp(damage, 0, 255);
        }

        void heal() {
            int old_health = health;
            health = std::clamp(health + 3, 0, max_health);

            std::println("{} healed! ({}->{})", name, old_health, health);
        }
        void attack(Player& target) {
            int old_health = target.health;
            target.health = std::max(0, target.health - damage);

            std::println("{} attacked {}! ({}hp -> {}hp)", name, target.name, old_health, target.health);

            if (target.health == 0) {
                std::println("{} killed {}!", name, target.name);
            }
        }
        void buff() {
            int old_damage = damage;
            damage *= 1.5;
            std::println("{} buffed! ({} DMG -> {})", name, old_damage, damage);
        }

        void stats() {
            std::println("Player: {}\n----------------\nHealth: {}/{}\nDamage: {}\n",name, health, max_health, damage);
        }
};

void do_action(PlayerAction player_action, Player &p1, Player &p2, int* remaining_actions) {
    switch (player_action) {
    case PlayerAction::Attack:
        p1.attack(p2);
        (*remaining_actions)--;
        break;

    case PlayerAction::Buff:
        p1.buff();
        (*remaining_actions)--;
        break;

    case PlayerAction::Heal:
        p1.heal();
        (*remaining_actions)--;
        break;

    case PlayerAction::CheckStats:
        p2.stats();
        break;

    case PlayerAction::EndTurn:
        *remaining_actions = 0;
        break;

    default:
        std::println("Invalid Option");
        break;
    }
}