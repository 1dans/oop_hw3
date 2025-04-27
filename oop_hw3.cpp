#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

const int SIZE = 10;

enum ItemType { COIN, ENEMY, HP, DEFENSE, ATTACK, EMPTY };

struct Position {
    int x, y;
};

class Item {
public:
    Position pos;
    ItemType type;

    Item(int x, int y, ItemType t) : pos{ x, y }, type(t) {}

    char getSymbol() {
        switch (type) {
        case COIN: return 'C';
        case ENEMY: return 'E';
        case HP: return 'H';
        case DEFENSE: return 'D';
        case ATTACK: return 'A';
        default: return '.';
        }
    }
};

class Player {
public:
    Position pos;
    int health = 10, attack = 5, defense = 5, coins = 0;

    Player() { pos = { SIZE / 2, SIZE / 2 }; }

    void move(char direction) {
        switch (direction) {
        case 'w': if (pos.y > 0) pos.y--; break;
        case 's': if (pos.y < SIZE - 1) pos.y++; break;
        case 'a': if (pos.x > 0) pos.x--; break;
        case 'd': if (pos.x < SIZE - 1) pos.x++; break;
        }
    }

    void interact(Item& item) {
        switch (item.type) {
        case COIN: coins++; break;
        case HP: health += 3; break;
        case DEFENSE: defense += 3; break;
        case ATTACK: attack += 3; break;
        case ENEMY: health -= (5 - defense / 2); break;
        default: break;
        }
    }
};

class Game {
private:
    Player player;
    vector<Item> items;

public:
    Game() {
        generateItems();
    }

    void generateItems() {
        addItems(COIN, 5);
        addItems(ENEMY, 5);
        addItems(HP, 3);
        addItems(DEFENSE, 3);
        addItems(ATTACK, 3);
    }

    void addItems(ItemType type, int count) {
        for (int i = 0; i < count; i++) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            items.push_back(Item(x, y, type));
        }
    }

    void drawField() {
        cout << "\033[2J\033[1;1H";
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                if (player.pos.x == x && player.pos.y == y) {
                    cout << 'P' << ' ';
                }
                else {
                    bool found = false;
                    for (auto& item : items) {
                        if (item.pos.x == x && item.pos.y == y) {
                            cout << item.getSymbol() << ' ';
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << ". ";
                }
            }
            cout << endl;
        }
        cout << "Здоров'я: " << player.health << " | Атака: " << player.attack << " | Захист: " << player.defense << " | Коїни: " << player.coins << endl;
    }

    void play() {
        while (player.health > 0) {
            drawField();
            char move;
            cout << "Рух (w/a/s/d): ";
            cin >> move;
            player.move(move);

            for (auto it = items.begin(); it != items.end();) {
                if (player.pos.x == it->pos.x && player.pos.y == it->pos.y) {
                    player.interact(*it);
                    it = items.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        cout << "Гру закінчено! Ви загинули.\n";
    }
};

int main() {
    system("chcp 1251 > null");
    srand(time(nullptr));
    Game game;
    game.play();
    return 0;
}