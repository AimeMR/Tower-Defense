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
    void modifyMoney(int amount) { m_money += amount; if(amount > 0) m_totalMoneyEarned += amount; }
    void modifyHealth(int amount) { m_health += amount; }
    void increaseRound();

    //Getters
    int getMoney() { return m_money; }
    int getHealth() { return m_health; }
    int getRound() { return m_nRound; }
    float getDifficulty() { return m_difficulty; }

    //Stats
    void enemyDefeated() { m_enemiesDefeated++; }
    void turretPlaced() { m_turretsPlaced++; }
    void turretUpgraded() { m_UpgradesPurchased++; }

private:
    //Valores iniciales
    Player()
    {
        //Variables
        m_money = 20000;
        m_health = 10;
        m_nRound = 1;
        m_difficulty = 1;

        //Stats
        m_enemiesDefeated = 0;
        m_totalMoneyEarned = 0;
    }

    //Variables
    int m_money, m_health, m_nRound, m_weight;
    float m_difficulty;

    //Stats
    int m_enemiesDefeated, m_totalMoneyEarned, m_turretsPlaced, m_UpgradesPurchased;
};