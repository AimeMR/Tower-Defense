#pragma once

class Player {
public:
    //Acceso al singleton
    static Player& GetInstance() {
        static Player instance;
        return instance;
    }

    //Evitar copia de objetos Player
    Player(const Player&) = delete;
    void operator=(const Player&) = delete;

    //Tocad a partir de aquí

    //Setters
    void modifyMoney(int amount) { m_money += amount; }
    void modifyHealth(int amount) { m_health += amount; }
    void increaseRound() { m_nRound++; }

    //Getters
    int getMoney() { return m_money; }
    int getHealth() { return m_health; }

private:
    //Valores iniciales
    Player()
    {
        //Variables
        m_money = 0;
        m_health = 10;

        //Stats
        m_enemiesDefeated = 0;
    }

    //Variables
    int m_money, m_health, m_nRound;

    //Stats
    int m_enemiesDefeated;
};